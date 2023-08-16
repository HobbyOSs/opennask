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
                pattern | ID_16BIT_MODE = false,
                pattern | _ = false
            );
            if (require) {
                return true;
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

    const int Encoding::get_output_size(OPENNASK_MODES mode) {
        int output_size = 0;

        // 0x66, 0x67はここで計算できない
        //if (prefix_ && prefix_->mandatory()) {
        //    output_size += prefix_->get_size();
        //}
        if (REX_ && REX_->mandatory()) {
            spdlog::get("opennask")->debug("[pass1] bytes rex {}", REX_->get_size());
            output_size += REX_->get_size();
        }
        if (VEX_) {
            spdlog::get("opennask")->debug("[pass1] bytes vex {}", VEX_->get_size());
            output_size += VEX_->get_size();
        }

        spdlog::get("opennask")->debug("[pass1] bytes opcode {}", opcode_.get_size());
        output_size += opcode_.get_size();

        if (ModRM_) {
            spdlog::get("opennask")->debug("[pass1] bytes modrm {}", ModRM_->get_size());
            output_size += ModRM_->get_size();
        }
        if (immediate_) {
            spdlog::get("opennask")->debug("[pass1] bytes immediate {}", immediate_->size());
            output_size += immediate_->size();
        }
        if (data_offset_) {
            spdlog::get("opennask")->debug("[pass1] bytes data offset {}", data_offset_->size());
            output_size += data_offset_->size();
        }
        if (code_offset_) {
            spdlog::get("opennask")->debug("[pass1] bytes code offset {}", code_offset_->size());
            output_size += code_offset_->size();
        }

        return output_size;
    }

    const Encoding InstructionForm::find_encoding() {
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
                                        InstructionForm &form) {

        if (!form.operands().has_value()) {
            spdlog::get("opennask")->error("*** form has no value ***");
            return false;
        }

        auto operands = form.operands().value();
        if (operands.size() != tokens.size()) {
            spdlog::get("opennask")->error("*** operands size(={}) != tokens size(={}) ***", operands.size(), tokens.size());
            return false;
        }

        for (int i = 0; i < tokens.size(); i++) {

            auto actual_token_type = token_to_x86_type(tokens[i]);
            auto table_token_type = operands[i].type();

            std::cerr << "actual_token_type[" + std::to_string(i) + "]: "
                      << actual_token_type
                      << "\n"
                      << "table_token_type[" + std::to_string(i) + "]: "
                      << table_token_type
                      << "\n"
                      << std::endl;

            // "imm"でも"imm8"とマッチさせたいので前方一致で比較する
            if (_starts_with(table_token_type, _to_lower(actual_token_type))) {
                continue;
            }
            // al,ax,eax,raxでもマッチさせたい
            auto tup = std::make_tuple(tokens[i].AsString(), table_token_type);
            bool need_to_continue = match(tup)(
                pattern | ds("AL" , "al")  = true,
                pattern | ds("AX" , "ax")  = true,
                pattern | ds("EAX", "eax") = true,
                pattern | ds("RAX", "rax") = true,
                pattern | ds(_,_)          = false
            );

            if (need_to_continue) {
                continue;
            }

            return false;
        }

        return true;
    }

    const uint32_t Instruction::get_output_size(OPENNASK_MODES mode,
                                                const std::vector<TParaToken>& tokens) {

        auto it = std::find_if(forms_.begin(), forms_.end(), [&](InstructionForm& form) {
            return this->find_greedy(mode, tokens, form);
        });

        if (it != forms_.end()) {
            auto& found_form = *it;

            std::stringstream ss {"["};
            auto operands = found_form.operands().value();
            std::for_each(operands.begin(),
                          operands.end(),
                          [&](const Operand& o) -> void { ss << o.type() << ","; });
            ss << "]";
            spdlog::get("opennask")->debug("[pass1] detected operands {}", ss.str());

            auto encoding = found_form.find_encoding();
            uint32_t size = encoding.get_output_size(mode);

            if (_require_66h(mode, tokens)) {
                spdlog::get("opennask")->debug("[pass1] bytes 66h 1");
                size += 1;
            }
            if (_require_67h(mode, tokens)) {
                spdlog::get("opennask")->debug("[pass1] bytes 67h 1");
                size += 1;
            }

            if (size == 0) {
                spdlog::get("opennask")->error("*** machine code size is zero ***");
            }
            return size;
        }

        spdlog::get("opennask")->error("*** machine code size is zero ***");
        return 0;
    }
};
