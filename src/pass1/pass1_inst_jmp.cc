#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "pass1_strategy.hh"
#include "matchit.h"
#include "demangle.hpp"
#include <jsoncons/json.hpp>
#include "x86.hh"


void Pass1Strategy::visitLabel(Label x) {

    std::string label = x.substr(0, x.find(":", 0));

    // ラベルが存在するので、シンボルテーブルのラベルのレコードに現在のLCを設定
    sym_table[label] = loc;
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Pass1Strategy::processCALL(std::vector<TParaToken>& mnemonic_args) {
    // cat json-x86-64/x86_64.json | \
    // jq -r '.instructions["CALL"].forms[] | [.encodings[0].opcode.byte, .operands[0].type, .operands[1].type ] | @tsv'
    // --
    // E8      rel32
    // FF      r64
    // FF      m64
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString()
    );

    auto inst = iset->instructions().at("CALL");
    using namespace matchit;

    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return 3; // opcode + iw
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] CALL, Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


template <asmjit::x86::Inst::Id id>
void Pass1Strategy::processCalcJcc(std::vector<TParaToken>& mnemonic_args) {
    auto t = mnemonic_args[0];

    if (t.AsAttr() == TParaToken::ttLabel) {
        // ラベルの場合はとりあえずrel8として処理する, どちらになるかはpass2で判断する
        loc += 2;
        log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, 2);
        return;
    }

    using namespace matchit;
    uint32_t l = match(t.AsInt32())(
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = 1 + NASK_BYTE,
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = 2 + NASK_WORD,
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = 2 + NASK_DWORD
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJa>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJae>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJb>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJbe>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJc>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJe>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJg>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJge>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJl>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJle>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJna>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnae>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnb>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnbe>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnc>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJne>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJng>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnge>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnl>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnle>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJno>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnp>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJns>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJnz>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJo>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJp>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJpe>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJpo>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJs>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcJcc<asmjit::x86::Inst::kIdJz>(std::vector<TParaToken>& mnemonic_args);
