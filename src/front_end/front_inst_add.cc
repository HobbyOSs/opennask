#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "matchit.h"
#include "front_end.hh"

using namespace std::placeholders;
using namespace matchit;

template <asmjit::x86::Inst::Id id, uint8_t acc_opcode>
void FrontEnd::processEmitAdd(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsString(),
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsAttr()
    );
    auto dst = mnemonic_args[0];
    auto src = mnemonic_args[1];
    log()->debug("[pass2] processADD dst={}", dst.AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, dst, src);

        match(operands)(
            // 0x04 ib		ADD AL, imm8		imm8をALに加算する
            // 0x05 iw		ADD AX, imm16		imm16をAXに加算する
            // 0x05 id		ADD EAX, imm32		imm32をEAXに加算する
            pattern | ds("AL", TParaToken::ttReg8 , TParaToken::ttImm) = [&] {
                a.emit(id, x86::al, mnemonic_args[1].AsInt32());
            },
            pattern | ds("AX", TParaToken::ttReg16, TParaToken::ttImm) = [&] {
                // asmjitはこの場合最適化されてしまうのでdbで直接機械語を出力
                a.db(acc_opcode);
                a.dw(mnemonic_args[1].AsInt32());
            },
            pattern | ds("EAX", TParaToken::ttReg32, TParaToken::ttImm) = [&] {
                a.emit(id, x86::eax, mnemonic_args[1].AsInt32());
            },
            // 0x80 /0 ib	ADD r/m8, imm8		imm8をr/m8に加算する
            // 0x81 /0 iw	ADD r/m16, imm16	imm16をr/m16に加算する
            // 0x83 /0 ib	ADD r/m16, imm8		符号拡張imm8をr/m16に加算する
            // 0x81 /0 id	ADD r/m32, imm32	imm32をr/m32に加算する
            // 0x83 /0 ib	ADD r/m32, imm8		符号拡張imm8をr/m32に加算する
            pattern | ds(_, or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32), TParaToken::ttImm) = [&] {
                a.emit(id, mnemonic_args[0].AsAsmJitGp(), mnemonic_args[1].AsInt32());
            },
            pattern | ds(_, or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), TParaToken::ttImm) = [&] {
                a.emit(id, mnemonic_args[0].AsMem(), mnemonic_args[1].AsInt32());
            },
            // 0x00 /r		ADD r/m8, r8		r8をr/m8に加算する
            // 0x01 /r		ADD r/m16, r16		r16をr/m16に加算する
            // 0x01 /r		ADD r/m32, r32		r32をr/m32に加算する
            pattern | ds(_,
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32),
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32)) = [&] {
                a.emit(id, dst.AsAsmJitGp(), src.AsAsmJitGp());
            },
            pattern | ds(_,
                         or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32),
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32)) = [&] {
                             a.emit(id, mnemonic_args[0].AsMem(), mnemonic_args[1].AsAsmJitGp());
            },
            // 0x02 /r		ADD r8, r/m8		r/m8をr8に加算する
            // 0x03 /r		ADD r16, r/m16		r/m16をr16に加算する
            // 0x03 /r		ADD r32, r/m32		r/m32をr32に加算する
            pattern | ds(_,
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32),
                         or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32)) = [&] {
                             a.emit(id, mnemonic_args[0].AsAsmJitGp(), mnemonic_args[1].AsMem());
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] ADD, Not implemented or not matched!!!");
            }
        );
    });

}

template void FrontEnd::processEmitAdd<asmjit::x86::Inst::kIdAdd, 0x05>(std::vector<TParaToken>& mnemonic_args);

/**
void FrontEnd::processADD(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsString(),
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsAttr()
    );
    auto dst = mnemonic_args[0];
    auto src = mnemonic_args[1];
    log()->debug("[pass2] processADD dst={}", dst.AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, dst, src);

        match(operands)(
            // 0x04 ib		ADD AL, imm8		imm8をALに加算する
            // 0x05 iw		ADD AX, imm16		imm16をAXに加算する
            // 0x05 id		ADD EAX, imm32		imm32をEAXに加算する
            pattern | ds("AL", TParaToken::ttReg8 , TParaToken::ttImm) = [&] {
                a.add(x86::al, mnemonic_args[1].AsInt32());
            },
            pattern | ds("AX", TParaToken::ttReg16, TParaToken::ttImm) = [&] {
                // asmjitはこの場合最適化されてしまうのでdbで直接機械語を出力
                a.db(0x05);
                a.dw(mnemonic_args[1].AsInt32());
            },
            pattern | ds("EAX", TParaToken::ttReg32, TParaToken::ttImm) = [&] {
                a.add(x86::eax, mnemonic_args[1].AsInt32());
            },
            // 0x80 /0 ib	ADD r/m8, imm8		imm8をr/m8に加算する
            // 0x81 /0 iw	ADD r/m16, imm16	imm16をr/m16に加算する
            // 0x83 /0 ib	ADD r/m16, imm8		符号拡張imm8をr/m16に加算する
            // 0x81 /0 id	ADD r/m32, imm32	imm32をr/m32に加算する
            // 0x83 /0 ib	ADD r/m32, imm8		符号拡張imm8をr/m32に加算する
            pattern | ds(_, or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32), TParaToken::ttImm) = [&] {
                a.add(mnemonic_args[0].AsAsmJitGp(), mnemonic_args[1].AsInt32());
            },
            pattern | ds(_, or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), TParaToken::ttImm) = [&] {
                a.add(mnemonic_args[0].AsMem(), mnemonic_args[1].AsInt32());
            },
            // 0x00 /r		ADD r/m8, r8		r8をr/m8に加算する
            // 0x01 /r		ADD r/m16, r16		r16をr/m16に加算する
            // 0x01 /r		ADD r/m32, r32		r32をr/m32に加算する
            pattern | ds(_,
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32),
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32)) = [&] {
                a.add(dst.AsAsmJitGp(), src.AsAsmJitGp());
            },
            pattern | ds(_,
                         or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32),
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32)) = [&] {
                             a.add(mnemonic_args[0].AsMem(), mnemonic_args[1].AsAsmJitGp());
            },
            // 0x02 /r		ADD r8, r/m8		r/m8をr8に加算する
            // 0x03 /r		ADD r16, r/m16		r/m16をr16に加算する
            // 0x03 /r		ADD r32, r/m32		r/m32をr32に加算する
            pattern | ds(_,
                         or_(TParaToken::ttReg8, TParaToken::ttReg16, TParaToken::ttReg32),
                         or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32)) = [&] {
                             a.add(mnemonic_args[0].AsAsmJitGp(), mnemonic_args[1].AsMem());
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] ADD, Not implemented or not matched!!!");
            }
        );
    });
}
*/
