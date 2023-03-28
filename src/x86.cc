#include "x86.hh"
#include "matchit.h"
#include "demangle.hpp"

using namespace matchit;

namespace x86_64 {

    extern "C" {
#include "incbin.h"
        // multiple definitionの対策でソースファイル側に定義している
        INCTXT(x86_json_, "json-x86-64/x86_64.json");
    };

    const char* X86_64_JSON = gx86_json_Data;

    const std::string token_to_x86_type(const TParaToken* operand) {

      return match(operand->AsAttr())(
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
              throw std::runtime_error(
                  operand->to_string() + " Not implemented or not matched!!!"
              );
              return "";
          }
      );
    }

    const int Encoding::get_output_size(OPENNASK_MODES mode) {
        int output_size = 0;

        if (prefix_ && prefix_->mandatory()) {
            output_size += prefix_->get_size();
        }
        if (REX_ && REX_->mandatory()) {
            output_size += REX_->get_size();
        }
        if (VEX_) {
            output_size += VEX_->get_size();
        }

        output_size += opcode_.get_size();

        if (ModRM_) {
            output_size += ModRM_->get_size();
        }
        if (immediate_) {
            output_size += immediate_->size();
        }
        if (data_offset_) {
            output_size += data_offset_->size();
        }
        if (code_offset_) {
            output_size += code_offset_->size();
        }
;
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

    const uint32_t Instruction::get_output_size(OPENNASK_MODES mode, std::initializer_list<TParaToken> tokens) {

        auto it = std::find_if(forms_.begin(), forms_.end(),
                               [&](InstructionForm &form) {
                               if (!form.operands().has_value()) {
                                   return false;
                               }

                               auto operands = form.operands().value();
                               if (operands.size() != tokens.size()) {
                                   return false;
                               }

                               for (int i = 0; i < tokens.size(); i++) {

                                   // "imm"でも"imm8"とマッチさせたいので前方一致で比較する
                                   auto actual_token_type = token_to_x86_type(tokens.begin() + i);
                                   auto table_token_type = operands[i].type();

                                   //std::cout << (tokens.begin() + i)->to_string() << std::endl;
                                   //std::cout << "actual_token_type: "
                                   //          << actual_token_type
                                   //          << "\n"
                                   //          << "table_token_type: "
                                   //          << table_token_type
                                   //          << std::endl;
                                   if (table_token_type.size() >= actual_token_type.size() &&
                                       std::equal(std::begin(actual_token_type),
                                                  std::end(actual_token_type),
                                                  std::begin(table_token_type))) {
                                       continue;
                                   }

                                   return false;
                               }

                               return true;
                               });

        if (it != forms_.end()) {
            auto& found_form = *it;
            auto encoding = found_form.find_encoding();
            const uint32_t size = encoding.get_output_size(mode);
            return size;
        }

        return 0;
    }
};
