#include <iostream>
#include <sstream>
#include "x86.hh"
#include "matchit.h"
#include "demangle.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace matchit;

namespace x86_64 {

    extern "C" {
#include "incbin.h"
        // multiple definitionの対策でソースファイル側に定義している
        INCTXT(x86_json_, "json-x86-64/x86_64.json");
    };

    const char* X86_64_JSON = gx86_json_Data;

    const bool _require_67h(OPENNASK_MODES mode, const std::vector<TParaToken>& tokens) {
        bool require = false;

        for (int i = 0; i < tokens.size(); i++) {
            auto t = tokens[i];
            require = match(mode)(
                // ベースをもつメモリーアドレス表現を判定する ex) [EBX], [EBX+16]
                pattern | ID_16BIT_MODE | when(t.AsAttr() == TParaToken::ttMem32 && t.IsAsmJitGpd()) = true,
                pattern | ID_16BIT_MODE = false,
                pattern | ID_32BIT_MODE | when(t.AsAttr() == TParaToken::ttReg16) = true,
                pattern | ID_32BIT_MODE = false,
                pattern | _ = false
            );
            if (require) {
                return true;
            }
        }

        return false;
    }

    const bool _require_66h(OPENNASK_MODES mode, const std::vector<TParaToken>& tokens) {
        bool require = false;

        for (int i = 0; i < tokens.size(); i++) {
            auto t = tokens[i];

            require = match(mode)(
                pattern | ID_16BIT_MODE | when(t.AsAttr() == TParaToken::ttReg32) = true,
                pattern | ID_16BIT_MODE | when(t.AsAttr() == TParaToken::ttMem32) = true,
                pattern | ID_16BIT_MODE = false,
                pattern | ID_32BIT_MODE | when(t.AsAttr() == TParaToken::ttReg16) = true,
                pattern | ID_32BIT_MODE | when(t.AsAttr() == TParaToken::ttMem16) = true,
                pattern | ID_32BIT_MODE = false,
                pattern | _ = false
            );
            if (require) {
                return true;
            }
        }

        return false;
    }

    /**
     * メモリーアドレス表現にあるoffset値について機械語サイズの計算をする
     *   ベースを持たない直接のアドレス表現 ex) MOV CL,[0x0ff0]; の場合2byteを返す
     *   ベースがある場合のアドレス表現     ex) MOV ECX,[EBX+16]; の場合1byteを返す
     */
    const size_t _calc_offset_byte_size(const std::vector<TParaToken>& tokens) {

        size_t offset_byte_size = 0;

        for (auto t : tokens) {

            if (t.IsMem() && ! t.HasMemBase()) {
                offset_byte_size += t.GetOffsetSize();
                continue;
            }

            if (t.IsMem() && t.HasMemBase()) {
                auto offset = t.AsMem().offset();
                if (offset == 0) {
                    continue; // offsetなし
                }

                if (-0x80 <= offset && offset <= 0x7f) {
                    offset_byte_size += 1;
                    continue; // byte
                }
                if (-0x8000 <= offset && offset <= 0x7fff) {
                    offset_byte_size += 2;
                    continue; // word
                }
                offset_byte_size += 4; // dword
            }
        }

        return offset_byte_size;
    }

    /**
     * 32 ビットの " ベース + インデックス " および " スケール + インデックス " の
     * 両形式のアドレス指定には、SIB バイトが必要である, 必要な場合trueを返す
     *   ベースがある場合のアドレス表現     ex) MOV ECX,[EBX+16]; の場合trueを返す
     */
    const bool _require_sib_byte(const std::vector<TParaToken>& tokens) {

        for (auto t : tokens) {
            if (t.IsMem() && t.IsAsmJitGpd() && t.HasMemBase()) {
                auto offset = t.AsMem().offset();
                if (offset == 0) {
                    continue; // offsetなし ex) `[EBX]`
                } else {
                    return true;
                }
            }
        }

        return false;
    }

    const std::string token_to_x86_type(const TParaToken& operand) {

        return match(operand.AsAttr())(
            pattern | TParaToken::ttReg8  = [&] { return "r8"; },
            pattern | TParaToken::ttReg16 = [&] { return "r16"; },
            pattern | TParaToken::ttReg32 = [&] { return "r32"; },
            pattern | TParaToken::ttReg64 = [&] { return "r64"; },
            pattern | TParaToken::ttMem8  = [&] { return "m8"; },
            pattern | TParaToken::ttMem16 = [&] { return "m16"; },
            pattern | TParaToken::ttMem32 = [&] { return "m32"; },
            pattern | TParaToken::ttMem64 = [&] { return "m64"; },
            pattern | TParaToken::ttMem64 = [&] { return "m64"; },
            pattern | TParaToken::ttRel8  = [&] { return "rel8"; },
            pattern | TParaToken::ttRel16 = [&] { return "rel16"; },
            pattern | TParaToken::ttRel32 = [&] { return "rel32"; },
            pattern | TParaToken::ttImm   = [&] { return "imm"; },
            pattern | TParaToken::ttLabel = [&] { return "imm"; },
            pattern | _ = [&] {
                throw std::runtime_error(operand.to_string() + " Not implemented or not matched!!!");
                return "";
            }
        );
    }

    const bool _starts_with(std::string const &full_string, std::string const &begining) {
        return full_string.substr(0, begining.size()) == begining;
    }

    const std::string _to_lower(const std::string &in) {
        std::string s(in);
        std::transform(s.begin(),
                       s.end(),
                       s.begin(),
                       [](unsigned char const &c) {
                           return ::tolower(c);
                       });
        return s;
    }

    const int Encoding::get_output_size(OPENNASK_MODES mode) const {

        auto logger = spdlog::get("opennask");
        int output_size = 0;

        // 0x66, 0x67はここで計算できない
        logger->trace("[pass1] --- get_output_size ---");
        if (REX_ && REX_->mandatory()) {
            logger->trace("[pass1] bytes rex {}", REX_->get_size());
            output_size += REX_->get_size();
        }
        if (VEX_) {
            logger->trace("[pass1] bytes vex {}", VEX_->get_size());
            output_size += VEX_->get_size();
        }

        logger->trace("[pass1] bytes opcode {}", opcode_.get_size());
        output_size += opcode_.get_size();

        if (ModRM_) {
            logger->trace("[pass1] bytes modrm {}", ModRM_->get_size());
            output_size += ModRM_->get_size();
        }
        if (immediate_) {
            logger->trace("[pass1] bytes immediate {}", immediate_->size());
            output_size += immediate_->size();
        }
        if (data_offset_) {
            logger->trace("[pass1] bytes data offset {}", data_offset_->size());
            output_size += data_offset_->size();
        }
        if (code_offset_) {
            logger->trace("[pass1] bytes code offset {}", code_offset_->size());
            output_size += code_offset_->size();
        }

        return output_size;
    }

    Encoding InstructionForm::find_encoding() const {
        if (encodings_.size() == 1) {
            return encodings_[0];
        }

        // TODO: MOV r16, imm16という命令は
        // 0xC7 /0
        // 0xB8+rw
        // の２つの機械語が使用可能、とりあえず後者を選んでいる。本当はもっときっちり判定するべき。
        // * 「0xC7 /0」は、オペランドのうち、第1オペランドがレジスターで、第2オペランドがメモリーやレジスターの場合に使用されます。つまり、第1オペランドがr16で、第2オペランドがimm16の場合には、このオペコードを使用することができる
        // * 「0xB8+rw」は、オペランドのうち、第1オペランドがレジスターで、第2オペランドが即値の場合に使用される
        return encodings_[1];
    }

    const bool Instruction::find_greedy(OPENNASK_MODES mode,
                                        const std::vector<TParaToken>& tokens,
                                        const InstructionForm &form) const {

        auto logger = spdlog::get("opennask");

        if (!form.operands().has_value()) {
            logger->trace("[pass1] form has no value");
            return false;
        }

        auto operands = form.operands().value();
        if (operands.size() != tokens.size()) {
            return false;
        }

        for (int i = 0; i < tokens.size(); i++) {
            auto actual_token_type = token_to_x86_type(tokens[i]);
            auto table_token_type = operands[i].type();

            std::stringstream ss;
            ss << "actual_token_type[" + std::to_string(i) + "]: "
               << actual_token_type
               << ", "
               << "table_token_type[" + std::to_string(i) + "]: "
               << table_token_type;
            logger->trace("[pass1] find_greedy => {}", ss.str());

            // 素直にマッチしたら次へ
            if (actual_token_type==table_token_type)
                continue;

            // x86_64.jsonの中に直接レジスタ名が書いてある場合でもマッチさせたい
            // al,ax,eax,rax,dx... など
            bool reg_name_matched  = match( std::make_tuple(tokens[i].AsString(), table_token_type) )(
                pattern | ds("AL" , "al")  = true,
                pattern | ds("AX" , "ax")  = true,
                pattern | ds("EAX", "eax") = true,
                pattern | ds("RAX", "rax") = true,
                pattern | ds("DX", "dx")   = true,
                pattern | ds(_,_)          = false
            );

            if (reg_name_matched) {
                // 直接レジスタ名がマッチ
                continue;
            }

            if (_starts_with(table_token_type, "imm") && tokens[i].AsAttr()==TParaToken::ttLabel) {
                // x86_64.json側が即値表記、tokenはラベルの場合マッチ
                continue;
            }

            // "imm"でも"imm8"とマッチさせたいので前方一致で比較する
            // しかしながら、token側のサイズが既定に満たないならば除外
            if (_starts_with(table_token_type, "imm") && tokens[i].IsImmediate() ) {
                const int32_t actual_imm = tokens[i].AsInt32();

                if (table_token_type == "imm8" && -0x80 <= actual_imm && actual_imm <= 0x7f) {
                    continue;
                }
                if (table_token_type == "imm16" && -0x8000 <= actual_imm && actual_imm <= 0x7fff) {
                    continue;
                }
                if (table_token_type == "imm32") {
                    continue;
                }
            }

            return false;
        }

        return true;
    }

    const uint32_t Instruction::get_output_size(OPENNASK_MODES mode,
                                                const std::vector<TParaToken>& tokens) const {

        auto logger = spdlog::get("opennask");

        // 条件に一致したオペランドを集める
        std::vector<InstructionForm> matching_forms;
        std::copy_if(forms_.begin(), forms_.end(), std::back_inserter(matching_forms), [&](const InstructionForm& form) {
            return find_greedy(mode, tokens, form);
        });

        // 最小の機械語サイズを見つけるための初期値を設定(番兵)
        uint32_t min_size = std::numeric_limits<uint32_t>::max();
        const InstructionForm* min_size_form = nullptr;
        logger->trace("[pass1] matching_forms size:{}", matching_forms.size());

        // 条件に合致するフォームの中から機械語サイズが最小のものを選ぶ
        for (const InstructionForm& form : matching_forms) {

            std::stringstream ss;
            auto operands = form.operands().value();
            ss << "[";
            std::for_each(operands.begin(),
                          operands.end(),
                          [&](const Operand& o) -> void { ss << o.type() << ","; });
            ss << "]";
            logger->trace("[pass1] operands {}", ss.str());

            const auto e = form.find_encoding();
            const int size = e.get_output_size(mode);

            // 0x66,0x67も考慮に入れたいところだがロジックがおかしくなるのでここでは入れない
            // 最終結果には0x66, 0x67も考慮したサイズを計算して返す
            if (size < min_size) {
                min_size = size;
                min_size_form = &form;
            }
        }

        if (min_size_form != nullptr) {
            auto& found_form = *min_size_form;

            std::stringstream ss;
            auto operands = found_form.operands().value();

            ss << "[";
            std::for_each(operands.begin(),
                          operands.end(),
                          [&](const Operand& o) -> void { ss << o.type() << ","; });
            ss << "]";
            logger->trace("[pass1] detected operands {}", ss.str());

            int require_66h = 0;
            int require_67h = 0;
            int require_sib_byte = 0;

            if (_require_66h(mode, tokens)) {
                logger->trace("[pass1] {} bytes 0x66h 1", OPENNASK_MODES_NAMES[mode]);
                require_66h = 1;
            }
            if (_require_67h(mode, tokens)) {
                logger->trace("[pass1] {} bytes 0x67h 1", OPENNASK_MODES_NAMES[mode]);
                require_67h = 1;
            }
            if (mode == ID_32BIT_MODE && _require_sib_byte(tokens)) {
                logger->trace("[pass1] {} sib-byte 1", OPENNASK_MODES_NAMES[mode]);
                require_sib_byte = 1;
            }
            const auto offset_byte_size = _calc_offset_byte_size(tokens); // 直接アドレス表現等で使用されるバイト数計算
            logger->trace("[pass1] bytes offset byte size {}", offset_byte_size);

            logger->debug("[pass1] selected form with minimum machine code size: {}",
                          min_size + require_66h + require_67h + offset_byte_size + require_sib_byte);
            return min_size + require_66h + require_67h + offset_byte_size + require_sib_byte;
        }

        // エラー処理
        std::stringstream no_matching_message;
        no_matching_message << " [";
        std::for_each(tokens.begin(),
                      tokens.end(),
                      [&](const TParaToken& t) -> void { no_matching_message << t.to_string() << ","; });
        no_matching_message << "]";

        logger->error("[pass1] {}", no_matching_message.str());
        throw std::runtime_error(no_matching_message.str());

        return 0;
    }
};
