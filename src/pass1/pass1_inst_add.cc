#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "pass1_strategy.hh"
#include "matchit.h"
#include "demangle.hpp"
#include <jsoncons/json.hpp>
#include "x86.hh"

template <asmjit::x86::Inst::Id id>
void Pass1Strategy::processCalcAdd(std::vector<TParaToken>& mnemonic_args) {

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );

    asmjit::String tmp;
    asmjit::InstAPI::instIdToString(asmjit::Arch::kX86, id, tmp);
    std::string op = to_upper(std::string(tmp.data()));
    auto inst = iset->instructions().at(op);

    using namespace matchit;
    uint32_t l = match(operands)(
        pattern | ds(TParaToken::ttReg8, "AL", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg8, _, TParaToken::ttMem8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, "AX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg16, _, TParaToken::ttMem16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, "EAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, "RAX", or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem8, _, TParaToken::ttReg8, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            return inst.get_output_size(bit_mode, mnemonic_args);
        },
        pattern | _ = [&] {
            std::stringstream ss;
            ss << "[pass1] " << op << ", Not implemented or not matched!!! \n"
               << mnemonic_args[0].AsString()
               << ", "
               << mnemonic_args[1].AsString()
               << "\n"
               << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
               << ", "
               << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
               << std::endl;

            throw std::runtime_error(ss.str());
            return 0;
        }
    );

    loc += l;
    log()->debug("[pass1] LOC = {}({:x}) +{}", std::to_string(loc), loc, l);
}


template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdAdc>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdAdd>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdAnd>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdCmp>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdOr>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdSbb>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdSub>(std::vector<TParaToken>& mnemonic_args);
template void Pass1Strategy::processCalcAdd<asmjit::x86::Inst::kIdXor>(std::vector<TParaToken>& mnemonic_args);
