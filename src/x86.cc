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
          pattern | TParaToken::ttImm   = [&] { return "imm"; },
          pattern | _ = [&] {
              throw std::runtime_error(
                  operand->to_string() + " Not implemented or not matched!!!"
              );
              return "";
          }
      );
    }

    const int Encoding::get_output_size() {
        int output_size = 0;

        if (prefix_) {
            std::cout << "prefix size: "
                      << prefix_->get_size()
                      << std::endl;
            output_size += prefix_->get_size();
        }
        if (rex_) {
            std::cout << "rex size: "
                      << rex_->get_size()
                      << std::endl;
            output_size += rex_->get_size();
        }
        if (vex_) {
            std::cout << "vex size: "
                      << vex_->get_size()
                      << std::endl;
            output_size += vex_->get_size();
        }
        std::cout << "opcode size: "
                  << opcode_.get_size()
                  << std::endl;
        output_size += opcode_.get_size();
        if (mod_rm_) {
            std::cout << "modrm size: "
                      << mod_rm_->get_size()
                      << std::endl;
            output_size += mod_rm_->get_size();
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

        return output_size;
    }

    const uint32_t Instruction::get_output_size(std::initializer_list<TParaToken> tokens) {

        for (int i = 0; i < tokens.size(); i++) {
            std::cout << (tokens.begin() + i)->AsString() << std::endl;
        }

        auto it = std::find_if(forms_.begin(), forms_.end(),
                               [&](InstructionForm &form) {
                               if (!form.operands().has_value()) {
                                   return false;
                               }

                               auto operands = form.operands().value();
                               if (operands.size() != tokens.size()) {
                                   return false;
                               }

                               bool detect = true;
                               for (int i = 0; i < tokens.size(); i++) {
                                   if ( token_to_x86_type(tokens.begin() + i) != operands[i].type()) {
                                       detect = false;
                                       break;
                                   }
                               }
                               return detect;
                               });

        std::cout << "starting !!!! " << std::endl;

        if (it != forms_.end()) {
            auto& found_form = *it;
            auto encodings = found_form.encodings();

            std::cout << "type(found_form) -> " << type(found_form)
                      << " \n"
                      << " ** -> "
                      << encodings[0].get_output_size()
                      << " "
                      << std::endl;

            const uint32_t size = encodings[0].get_output_size();
            return size;
        }
        std::cout << "end !!!! " << std::endl;

        return 0;
    }
};
