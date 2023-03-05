#include "x86.hh"
#include "matchit.h"

using namespace matchit;

namespace x86_64 {

    extern "C" {
#include "incbin.h"
        // multiple definitionの対策でソースファイル側に定義している
        INCTXT(x86_json_, "json-x86-64/x86_64.json");
    };

    const char* X86_64_JSON = gx86_json_Data;

    const std::string token_to_x86_type(TParaToken& operand) {

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
          pattern | TParaToken::ttImm   = [&] { return "imm"; }
      );
    }

    const uint32_t Instruction::get_output_size(TParaToken& opr1,
                                                TParaToken& opr2) {

        token_to_x86_type(opr1);
        token_to_x86_type(opr2);
        return 0;
    }
};
