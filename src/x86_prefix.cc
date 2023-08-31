#include "x86_prefix.hh"
#include "matchit.h"

using namespace matchit;

void PrefixInfo::set(OPENNASK_MODES bit_mode, const TParaToken& operand) {
    const auto operand_attr = operand.AsAttr();

    // Address size prefix
    // メモリーアドレス表現によるアドレスサイズプレフィックスが必要か判定する
    require_67h = match(bit_mode)(
        // ex) [EBX], [EBX+16]
        pattern | ID_16BIT_MODE | when(operand_attr == TParaToken::ttMem32 && operand.IsAsmJitGpd()) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | ID_32BIT_MODE | when(operand_attr == TParaToken::ttMem16 && operand.IsAsmJitGpw()) = true,
        pattern | ID_32BIT_MODE = false,
        pattern | _ = false
    );

    // Operand size prefix
    // オペランドサイズプレフィックスが必要かどうか判定する
    require_66h = match(bit_mode)(
        pattern | ID_16BIT_MODE | when(operand_attr == TParaToken::ttReg32) = true,
        pattern | ID_16BIT_MODE | when(operand.IsImmediate() && operand.AsInt32() > 0x7fff) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | ID_32BIT_MODE | when(operand_attr == TParaToken::ttReg16) = true,
        pattern | ID_32BIT_MODE = false,
        pattern | _ = false
    );
}

void PrefixInfo::set(OPENNASK_MODES bit_mode, const TParaToken& dst, const TParaToken& src) {

    const auto dst_attr = dst.AsAttr();
    const auto src_attr = src.AsAttr();

    // Address size prefix
    // メモリーアドレス表現によるアドレスサイズプレフィックスが必要か判定する
    require_67h = match(bit_mode)(
        // ex) [EBX], [EBX+16]
        pattern | ID_16BIT_MODE | when(dst_attr == TParaToken::ttMem32 && dst.IsAsmJitGpd()) = true,
        pattern | ID_16BIT_MODE | when(src_attr == TParaToken::ttMem32 && src.IsAsmJitGpd()) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | ID_32BIT_MODE | when(dst_attr == TParaToken::ttMem16 && dst.IsAsmJitGpw()) = true,
        pattern | ID_32BIT_MODE | when(src_attr == TParaToken::ttMem16 && src.IsAsmJitGpw()) = true,
        pattern | ID_32BIT_MODE = false,
        pattern | _ = false
    );

    // Operand size prefix
    // オペランドサイズプレフィックスが必要かどうか判定する
    // dst,srcがあるときは下記のパターンがある
    require_66h = match(bit_mode)(
        pattern | ID_16BIT_MODE | when(dst_attr == TParaToken::ttReg32) = true,
        pattern | ID_16BIT_MODE | when(src_attr == TParaToken::ttReg32 && dst_attr == TParaToken::ttReg32) = true,
        pattern | ID_16BIT_MODE | when(src_attr == TParaToken::ttReg32 && dst.IsMem()) = true,
        pattern | ID_16BIT_MODE | when(src.IsImmediate() && src.AsInt32() > 0x7fff) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | ID_32BIT_MODE | when(dst_attr == TParaToken::ttReg16) = true,
        pattern | ID_32BIT_MODE | when(src_attr == TParaToken::ttReg16 && dst_attr == TParaToken::ttReg16) = true,
        pattern | ID_32BIT_MODE | when(src_attr == TParaToken::ttReg16 && dst.IsMem()) = true,
        pattern | ID_32BIT_MODE = false,
        pattern | _ = false
    );
}
