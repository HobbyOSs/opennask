#include <fstream>
#include <typeinfo>
#include <type_traits>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "matchit.h"
#include "front_end.hh"
#include "driver.hh"
#include "parser.hh"
#include "demangle.hpp"
#include "mod_rm.hpp"


using namespace std::placeholders;
using namespace matchit;


void FrontEnd::processIMUL(std::vector<TParaToken>& mnemonic_args) {

    // オペランドが可変長のためoptionalで宣言
    auto get_optional = [&](std::size_t index) -> std::optional<TParaToken::TIdentiferAttribute> {
        return index < mnemonic_args.size() ? std::make_optional(mnemonic_args[index].AsAttr()) : std::nullopt ;
    };
    auto operands = std::make_tuple(
        get_optional(0),
        get_optional(1),
        get_optional(2)
    );

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, mnemonic_args[0]);

        match(operands)(
            // 0xF6 /5		IMUL r/m8		ALをr/m8で符号付き乗算し、結果をAXに格納します
            // 0xF7 /5		IMUL r/m16		AXをr/m16で符号付き乗算し、結果をDX:AXに格納します
            // 0xF7 /5		IMUL r/m32		EAXをr/m8で符号付き乗算し、結果をEDX:EAXに格納します
            pattern | ds(TParaToken::ttReg8, std::nullopt, std::nullopt) = [&] {
                auto reg = mnemonic_args[0];
                if ( reg.IsAsmJitGpbLo() ) {
                    a.imul(reg.AsAsmJitGpbLo());
                } else if ( reg.IsAsmJitGpbHi() ) {
                    a.imul(reg.AsAsmJitGpbHi());
                }
            },
            pattern | ds(TParaToken::ttReg16, std::nullopt, std::nullopt) = [&] {
                a.imul(mnemonic_args[0].AsAsmJitGpw());
            },
            pattern | ds(TParaToken::ttReg32, std::nullopt, std::nullopt) = [&] {
                a.imul(mnemonic_args[0].AsAsmJitGpd());
            },
            pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), std::nullopt, std::nullopt) = [&] {
                a.imul(mnemonic_args[0].AsMem());
            },
            pattern | ds(TParaToken::ttReg16, TParaToken::ttImm, std::nullopt) = [&] {
                a.imul(mnemonic_args[0].AsAsmJitGpw(), mnemonic_args[1].AsInt32());
            },
            pattern | ds(TParaToken::ttReg32, TParaToken::ttImm, std::nullopt) = [&] {
                pp.require_66h = true;
                a.imul(mnemonic_args[0].AsAsmJitGpd(), mnemonic_args[1].AsInt32());
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] IMUL, Not implemented or not matched!!!");
            }
        );
    });
}


void FrontEnd::processIN(std::vector<TParaToken>& mnemonic_args) {

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );
    auto dst = mnemonic_args[0];
    auto src = mnemonic_args[1];

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, dst);

        match(operands)(
            // 0xE4 ib | IN AL, imm8 | I/Oポートアドレスimm8からALにバイトを入力します
            // 0xE5 ib | IN AX, imm8 | I/Oポートアドレスimm8からAXにワードを入力します
            // 0xE5 ib | IN EAX, imm8 | I/Oポートアドレスimm8からEAXにダブルワードを入力します
            pattern | ds(_, "AL", TParaToken::ttImm, _) = [&] {
                a.db(0xe4);
                a.db(src.AsInt32());
            },
            pattern | ds(_, or_(std::string("AX"), std::string("EAX")), TParaToken::ttImm, _) = [&] {
                a.db(0xe5);
                a.db(src.AsInt32());
            },
            // 0xEC | IN AL, DX | DXで指定するI/OポートアドレスからALにバイトを入力します
            // 0xED | IN AX, DX | DXで指定するI/OポートアドレスからAXにワードを入力します
            // 0xED | IN EAX, DX | DXで指定するI/OポートアドレスからEAXにダブルワードを入力します
            pattern | ds(_, "AL", _, "DX") = [&] {
                pp.require_67h = false;
                a.db(0xec);
            },
            pattern | ds(_, or_(std::string("AX"), std::string("EAX")), _, "DX") = [&] {
                pp.require_67h = false;
                a.db(0xed);
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] IN, Not implemented or not matched!!!");
            }
        );
    });
}


void FrontEnd::processINT(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttHex);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());
    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        a.db(0xcd);
        a.db(arg.AsInt32());
    });
}

void FrontEnd::processLIDT(std::vector<TParaToken>& mnemonic_args) {
    // 0x0F 01 /3	LIDT m16& 32mをIDTRにロードします
    auto src = mnemonic_args[0];
    log()->debug("[pass2] processLIDT src={}", src.AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;

        match(src.AsAttr())(
            pattern | or_(TParaToken::ttMem16, TParaToken::ttMem32) = [&] {
                a.lidt(src.AsMem());
            },
            pattern | TParaToken::ttImm = [&] {
                auto src_mem = "[" + src.AsString() + "]";
                a.db(0x0f);
                a.db(0x01);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, src_mem, ModRM::SLASH_3));
                a.dw(src.AsInt32());
            },
            pattern | TParaToken::ttLabel = [&] {
                CodeBuffer& buf = code_.textSection()->buffer();
                const std::string label = src.AsString();
                const auto label_address = static_cast<uint16_t>(sym_table.at(label));
                auto src_mem = "[" + int_to_hex(label_address) + "]";
                a.db(0x0f);
                a.db(0x01);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, src_mem, ModRM::SLASH_3));
                a.dw(label_address);
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] LIDT, Not implemented or not matched!!!");
            }
        );
    });
}

void FrontEnd::processLGDT(std::vector<TParaToken>& mnemonic_args) {
    // 0x0F 01 /2	LGDT m16& 32	mをGDTRにロードします
    auto src = mnemonic_args[0];
    log()->debug("[pass2] processLGDT src={}", src.AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;

        match(src.AsAttr())(
            pattern | or_(TParaToken::ttMem16, TParaToken::ttMem32) = [&] {
                a.lgdt(src.AsMem());
            },
            pattern | TParaToken::ttImm = [&] {
                auto src_mem = "[" + src.AsString() + "]";
                a.db(0x0f);
                a.db(0x01);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, src_mem, ModRM::SLASH_2));
                a.dw(src.AsInt32());
            },
            pattern | TParaToken::ttLabel = [&] {
                CodeBuffer& buf = code_.textSection()->buffer();
                const std::string label = src.AsString();
                const auto label_address = static_cast<uint16_t>(sym_table.at(label));
                auto src_mem = "[" + int_to_hex(label_address) + "]";
                a.db(0x0f);
                a.db(0x01);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, src_mem, ModRM::SLASH_2));
                a.dw(label_address);
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] LGDT, Not implemented or not matched!!!");
            }
        );
    });
}

void FrontEnd::processMOV(std::vector<TParaToken>& mnemonic_args) {

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        mnemonic_args[1].AsAttr(),
        mnemonic_args[1].AsString()
    );
    auto dst = mnemonic_args[0];
    auto src = mnemonic_args[1];

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, dst, src);

        match(operands)(
            // C6      r8      imm8 (TODO: こっちは実装していない)
            // B0+rb   r8      imm8
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttImm, _) | when(dst.IsAsmJitGpbLo()) = [&] {
                a.mov(dst.AsAsmJitGpbLo(), src.AsInt32());
            },
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttImm, _) | when(dst.IsAsmJitGpbHi()) = [&] {
                a.mov(dst.AsAsmJitGpbHi(), src.AsInt32());
            },
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttLabel, _) | when(dst.IsAsmJitGpbLo()) = [&] {
                std::string label = src.AsString();
                auto label_address = sym_table.at(label);
                auto jmp_offset = label_address - dollar_position - binout_container.size();
                auto offset = jmp_offset - (1 + NASK_BYTE);

                if ( dst.IsAsmJitGpbLo() ) {
                    a.mov(dst.AsAsmJitGpbLo(), offset);
                } else if ( dst.IsAsmJitGpbHi() ) {
                    a.mov(dst.AsAsmJitGpbHi(), offset);
                }
            },
            // 88      r8      r8
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, _) = [&] {
                match( std::make_tuple( dst.IsAsmJitGpbLo(), src.IsAsmJitGpbLo() ))(
                    pattern | ds(true, true)   = [&] { a.mov(dst.AsAsmJitGpbLo(), src.AsAsmJitGpbLo() ); },
                    pattern | ds(true, false)  = [&] { a.mov(dst.AsAsmJitGpbLo(), src.AsAsmJitGpbHi() ); },
                    pattern | ds(false, true)  = [&] { a.mov(dst.AsAsmJitGpbHi(), src.AsAsmJitGpbLo() ); },
                    pattern | ds(false, false) = [&] { a.mov(dst.AsAsmJitGpbHi(), src.AsAsmJitGpbHi() ); }
                );
            },
            // 8A      r8      m8
            pattern | ds(TParaToken::ttReg8, _, or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), _) = [&] {
                match( std::make_tuple( dst.IsAsmJitGpbLo(), src.AsMem().hasBase() ))(
                    // "MOV AL,[SI]", "MOV AL,[ESP+8]" のようにベースレジスタがあるパターン
                    pattern | ds(true , true) = [&] { a.mov(dst.AsAsmJitGpbLo(), src.AsMem() ); },
                    pattern | ds(false, true) = [&] { a.mov(dst.AsAsmJitGpbHi(), src.AsMem() ); },
                    // TODO: asmjit使用時ベースレジスタがないパターンは機械語が想定と違う
                    // "MOV CL,BYTE [0x0ff0]" のようなパターン
                    pattern | ds(_ , false)  = [&] {
                        a.db(0x8a);
                        a.db(ModRM::generate_modrm(0x8a, ModRM::REG_REG, src.AsString(), dst.AsString()));
                        a.dw(src.AsInt32());
                    }
                );
            },
            // C7      r16     imm16 (TODO: こっちは実装していない)
            // B8+rw   r16     imm16
            pattern | ds(TParaToken::ttReg16, _, TParaToken::ttImm, _) = [&] {
                a.mov(dst.AsAsmJitGpw(), src.AsInt32());
            },
            pattern | ds(TParaToken::ttSreg, _, TParaToken::ttImm, _) = [&] {
                a.mov(dst.AsAsmJitSReg(), x86::word_ptr(src.AsInt32()));
            },
            pattern | ds(TParaToken::ttReg16, _, TParaToken::ttLabel, _) = [&] {
                std::string label = src.AsString();
                auto label_address = sym_table.at(label);
                a.mov(dst.AsAsmJitGpw(), label_address);
            },
            pattern | ds(TParaToken::ttSreg, _, TParaToken::ttLabel, _) = [&] {
                std::string label = src.AsString();
                auto label_address = sym_table.at(label);
                a.mov(dst.AsAsmJitSReg(), x86::word_ptr(label_address));
            },
            // 89      r16     r16
            pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg16, _) = [&] {
                a.mov(dst.AsAsmJitGpw(), src.AsAsmJitGpw());
            },
            // 8B      r16     m16
            pattern | ds(TParaToken::ttReg16, _, or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), _) = [&] {
                match( src.AsMem().hasBase() )(
                    // "MOV AL,[SI]", "MOV AL,[ESP+8]" のようにベースレジスタがあるパターン
                    pattern | true = [&] { a.mov(dst.AsAsmJitGpw(), src.AsMem() ); },
                    // TODO: asmjit使用時ベースレジスタがないパターンは機械語が想定と違う
                    // "MOV CL,BYTE [0x0ff0]" のようなパターン
                    pattern | false  = [&] {
                        a.db(0x8b);
                        a.db(ModRM::generate_modrm(0x8b, ModRM::REG_REG, src.AsString(), dst.AsString()));
                        a.dw(src.AsInt32());
                    }
                );
            },
            // A1      eax     moffs32
            // C7      r32     imm32 (TODO: こっちは実装していない)

            // 0xB8+rd r32     imm32
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttImm, _) = [&] {
                a.mov(dst.AsAsmJitGpd(), src.AsInt32());
            },
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttLabel, _) = [&] {
                std::string label = src.AsString();
                auto label_address = sym_table.at(label);
                a.mov(dst.AsAsmJitGpd(), label_address);
            },
            // 89      r32     r32
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
                a.mov(dst.AsAsmJitGpd(), src.AsAsmJitGpd());
            },
            // 8B      r32     m32
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
                a.mov(dst.AsAsmJitGpd(), src.AsMem() );
            },
            // A1      rax     moffs64
            // C7      r64     imm32
            // B8      r64     imm64
            // 89      r64     r64
            // 8B      r64     m64

            // C6      m8      imm8
            pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
                const std::string dst_mem = "[" + dst.AsString() + "]";
                // TODO: asmjit使用時ベースレジスタがないパターンは機械語が想定と違う
                a.db(0xc6);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, dst_mem, ModRM::SLASH_0));
                a.dw(dst.AsInt32());
                a.db(src.AsInt32());
            },
            // A2      moffs8  al
            pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16), _, TParaToken::ttReg8, "AL") = [&] {
                a.db(0xa2);
                auto addr = mnemonic_args[0].AsUInt32();
                a.dw(addr);
            },
            // A3      moffs16 ax
            // A3      moffs32 eax

            // 88      m8      r8
            // 88      m16     r8
            // 88      m32     r8
            pattern | ds(or_(TParaToken::ttMem8,TParaToken::ttMem16,TParaToken::ttMem32), _, TParaToken::ttReg8, _) = [&] {
                if (dst.HasMemBase()) {
                    if (src.IsAsmJitGpbLo()) {
                        a.mov(dst.AsMem(), src.AsAsmJitGpbLo() );
                    } else if (src.IsAsmJitGpbHi()) {
                        a.mov(dst.AsMem(), src.AsAsmJitGpbHi() );
                    }
                    return;
                }
                if (dst.IsImmediate()) {
                    // asmjitでMOV [offset],Reg の場合独自実装が必要
                    // asmjit使用時にメモリーアドレッシング時にオフセットのみのMOVは機械語が想定と違う
                    a.db(0x88);
                    a.db(ModRM::generate_modrm(0x88,
                                               ModRM::REG_REG,
                                               std::string("[" + dst.AsString() + "]"),
                                               std::string(src.AsString())));
                    a.dw(dst.AsInt32());
                    return;
                }
            },
            // C7      m16     imm16
            pattern | ds(TParaToken::ttMem16, _, TParaToken::ttImm, _) = [&] {
                // TODO: test & メモリーアドレッシング
                // asmjit使用時にメモリーアドレッシング時にオフセットのみのMOVは機械語が想定と違う
                if (dst.IsAsmJitGpw()) {
                    a.mov(x86::word_ptr(dst.AsAsmJitGpw()), src.AsInt32());
                    return;
                }
                a.db(0xc7);
                a.db(ModRM::generate_modrm(ModRM::REG_REG,
                                           std::string("[" + dst.AsString() + "]"),
                                           ModRM::SLASH_0));
                a.dw(dst.AsInt32());
                a.dw(src.AsInt32());
            },
            pattern | ds(TParaToken::ttMem16, _, TParaToken::ttLabel, _) = [&] {
                // TODO: test & メモリーアドレッシング
                std::string label = src.AsString();
                auto label_address = sym_table.at(label);
                a.mov(x86::word_ptr(dst.AsAsmJitGpw()), label_address);
            },
            // 89      m16     r16
            pattern | ds(or_(TParaToken::ttMem16, TParaToken::ttMem32), _, TParaToken::ttReg16, _) = [&] {
                a.mov(dst.AsMem(), src.AsAsmJitGpw());
            },
            // C7      m32     imm32
            pattern | ds(TParaToken::ttMem32, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
                // TODO: test & メモリーアドレッシング
                // asmjit使用時にメモリーアドレッシング時にオフセットのみのMOVは機械語が想定と違う
                if (dst.IsAsmJitGpd()) {
                    a.mov(x86::dword_ptr(dst.AsAsmJitGpd()), src.AsInt32());
                    return;
                }

                pp.require_66h = true;
                a.db(0xc7);
                a.db(ModRM::generate_modrm(ModRM::REG_REG,
                                           std::string("[" + dst.AsString() + "]"),
                                           ModRM::SLASH_0));
                a.dw(dst.AsInt32());
                a.dd(src.AsInt32());
            },
            // 89      m32     r32
            pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
                a.mov(dst.AsMem(), src.AsAsmJitGpd());
            },
            // C7      m64     imm32
            // 89      m64     r64
            // A3      moffs64 rax
            // 8E /r      Sreg, r/m16
            pattern | ds(TParaToken::ttSreg, _, TParaToken::ttReg16, _) = [&] {
                a.mov(dst.AsAsmJitSReg(), src.AsAsmJitGpw());
            },
            // 8C /r      r/m16, Sreg
            pattern | ds(TParaToken::ttReg16, _, TParaToken::ttSreg, _) = [&] {
                a.mov(dst.AsAsmJitGpw(), src.AsAsmJitSReg());
            },
            // 0F 22 /r   Creg, r32
            pattern | ds(TParaToken::ttCreg, _, TParaToken::ttReg32, _) = [&] {
                pp.require_67h = false;
                pp.require_66h = false;
                a.mov(dst.AsAsmJitCReg(), src.AsAsmJitGpd());
            },
            // 0F 20 /r   r32, Creg
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttCreg, _) = [&] {
                pp.require_67h = false;
                pp.require_66h = false;
                a.mov(dst.AsAsmJitGpd(), src.AsAsmJitCReg());
            },
            pattern | _ = [&] {
                std::stringstream ss;
                ss << "[pass2] MOV, Not implemented or not matched!!! \n"
                   << TParaToken::TIAttributeNames[mnemonic_args[0].AsAttr()]
                   << ", "
                   << TParaToken::TIAttributeNames[mnemonic_args[1].AsAttr()]
                   << std::endl;

                throw std::runtime_error(ss.str());
            }
        );
    });
}

void FrontEnd::processOUT(std::vector<TParaToken>& mnemonic_args) {

    auto operands = std::make_tuple(
        mnemonic_args[0].AsString(),
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsString(),
        mnemonic_args[1].AsAttr()
    );
    auto dst = mnemonic_args[0];
    auto src = mnemonic_args[1];

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, src);

        match(operands)(
            // 0xE6 ib	OUT imm8, AL	ALのバイト値をI/Oポートアドレスimm8に出力します
            // 0xE7 ib	OUT imm8, AX	AXのワード値をI/Oポートアドレスimm8に出力します
            // 0xE7 ib	OUT imm8, EAX	EAXのダブルワード値をI/Oポートアドレスimm8に出力します
            pattern | ds(_, TParaToken::ttImm, "AL",  _) = [&] {
                a.out(mnemonic_args[0].AsInt32(), x86::al);
            },
            pattern | ds(_, TParaToken::ttImm, "AX",  _) = [&] {
                a.out(mnemonic_args[0].AsInt32(), x86::ax);
            },
            pattern | ds(_, TParaToken::ttImm, "EAX", _) = [&] {
                a.out(mnemonic_args[0].AsInt32(), x86::eax);
            },
            // 0xEE	OUT DX, AL	ALのバイト値をDXの値にあるI/Oポートアドレスに出力します
            // 0xEF	OUT DX, AX	AXのワード値をDXの値にあるI/Oポートアドレスに出力します
            // 0xEF	OUT DX, EAX	EAXのダブルワード値をDXの値にあるI/Oポートアドレスに出力します
            pattern | ds("DX", _, "AL", _) = [&] {
                pp.require_67h = false;
                a.out(x86::dx, x86::al);
            },
            pattern | ds("DX", _, "AX", _) = [&] {
                pp.require_67h = false;
                a.out(x86::dx, x86::ax);
            },
            pattern | ds("DX", _, "EAX", _) = [&] {
                pp.require_67h = false;
                a.out(x86::dx, x86::eax);
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] OUT, Not implemented or not matched!!!");
            }
        );
    });
}


void FrontEnd::processPOP(std::vector<TParaToken>& mnemonic_args) {
    // 0x8F /0 	POP m16 	スタックのトップからm16をPOPし、スタックポインタをインクリメントします
    // 0x8F /0 	POP m32 	スタックのトップからm32をPOPし、スタックポインタをインクリメントします
    // 0x58+rw 	POP r16 	スタックのトップからr16をPOPし、スタックポインタをインクリメントします
    // 0x58+rd 	POP r32 	スタックのトップからr32をPOPし、スタックポインタをインクリメントします
    // 0x1F 	POP DS 	スタックのトップからDSをPOPし、スタックポインタをインクリメントします
    // 0x07 	POP ES 	スタックのトップからESをPOPし、スタックポインタをインクリメントします
    // 0x17 	POP SS 	スタックのトップからSSをPOPし、スタックポインタをインクリメントします
    // 0x0F 0xA1 	POP FS 	スタックのトップからFSをPOPし、スタックポインタをインクリメントします
    // 0x0F 0xA9 	POP GS 	スタックのトップからGSをPOPし、スタックポインタをインクリメントします
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString()
    );
    auto src = mnemonic_args[0];

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, src);

        match(operands)(
            pattern | ds(TParaToken::ttReg16, _) = [&] {
                a.pop(src.AsAsmJitGpw());
            },
            pattern | ds(TParaToken::ttReg32, _) = [&] {
                a.pop(src.AsAsmJitGpd());
            },
            pattern | ds(or_(TParaToken::ttMem16, TParaToken::ttMem32), _) = [&] {
                a.pop(src.AsMem());
            },
            pattern | ds(TParaToken::ttSreg, or_(std::string("DS"), std::string("ES"), std::string("SS"))) = [&] {
                a.pop(src.AsAsmJitSReg());
            },
            pattern | ds(TParaToken::ttSreg, or_(std::string("FS"), std::string("GS"))) = [&] {
                a.pop(src.AsAsmJitSReg());
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] POP, Not implemented or not matched!!!");
            }
        );
    });
}

void FrontEnd::processPUSH(std::vector<TParaToken>& mnemonic_args) {
    // 0xFF /6 	PUSH r/m16 	r/m16をPUSHします
    // 0xFF /6 	PUSH r/m32 	r/m32をPUSHします
    // 0x50+rw 	PUSH r16 	r16をPUSHします
    // 0x50+rd 	PUSH r32 	r32をPUSHします
    // 0x6A 	PUSH imm8 	imm8をPUSHします
    // 0x68 	PUSH imm16 	imm16をPUSHします
    // 0x68 	PUSH imm32 	imm32をPUSHします
    // 0x0E 	PUSH CS 	CSをPUSHします
    // 0x16 	PUSH SS 	SSをPUSHします
    // 0x1E 	PUSH DS 	DSをPUSHします
    // 0x06 	PUSH ES 	ESをPUSHします
    // 0x0F 0xA0 	PUSH FS 	FSをPUSHします
    // 0x0F 0xA8 	PUSH GS 	GSをPUSHします
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString()
    );
    auto src = mnemonic_args[0];

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, src);

        match(operands)(
            pattern | ds(TParaToken::ttReg16, _) = [&] {
                a.push(src.AsAsmJitGpw());
            },
            pattern | ds(TParaToken::ttReg32, _) = [&] {
                a.push(src.AsAsmJitGpd());
            },
            pattern | ds(or_(TParaToken::ttMem16, TParaToken::ttMem32), _) = [&] {
                a.push(src.AsMem());
            },
            pattern | ds(TParaToken::ttImm, _) = [&] {
                a.push(src.AsInt32());
            },
            pattern | ds(TParaToken::ttSreg, or_(std::string("CS"), std::string("DS"), std::string("ES"), std::string("SS"))) = [&] {
                a.push(src.AsAsmJitSReg());
            },
            pattern | ds(TParaToken::ttSreg, or_(std::string("FS"), std::string("GS"))) = [&] {
                a.push(src.AsAsmJitSReg());
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] PUSH, Not implemented or not matched!!!");
            }
        );
    });
}

void FrontEnd::processRET(std::vector<TParaToken>& mnemonic_args) {
    // 0xC3 	RET 	呼び出し元にニアリターンします
    // 0xCB 	RET 	呼び出し元にファーリターンします
    // 0xC2 iw 	RET imm16 	呼び出し元にニアリターンし、imm16バイトをスタックからPOPします
    // 0xCA iw 	RET imm16 	呼び出し元にファーリターンし、imm16バイトをスタックからPOPします
    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        a.ret();
    });
}

void FrontEnd::processSHR(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].AsString(),
        (mnemonic_args.size() >= 2) ? std::make_optional(mnemonic_args[1].AsAttr()) : std::nullopt,
        (mnemonic_args.size() >= 2) ? std::make_optional(mnemonic_args[1].AsString()) : std::nullopt
    );
    auto dst = mnemonic_args[0];
    auto src = (mnemonic_args.size() >= 2) ? std::make_optional(mnemonic_args[1]) : std::nullopt;
    log()->debug("[pass2] processSHR dst={}, src={}", dst.AsString(), src.value().AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, dst);

        match(operands)(
            // 0xD2 /5	SHR r/m8, CL		r/m8を2でCL回符号なし除算します※
            // 0xD3 /5	SHR r/m16, CL		r/m16を2でCL回符号なし除算します※
            // 0xD3 /5	SHR r/m32, CL		r/m32を2でCL回符号なし除算します※
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, "CL") | when(dst.IsAsmJitGpbLo()) = [&] {
                a.shr(dst.AsAsmJitGpbLo(), src.value().AsAsmJitGpbLo());
            },
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttReg8, "CL") | when(dst.IsAsmJitGpbHi()) = [&] {
                a.shr(dst.AsAsmJitGpbHi(), src.value().AsAsmJitGpbLo());
            },
            pattern | ds(TParaToken::ttReg16, _, TParaToken::ttReg8, "CL") = [&] {
                a.shr(dst.AsAsmJitGpw(), src.value().AsAsmJitGpbLo());
            },
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg8, "CL") = [&] {
                a.shr(dst.AsAsmJitGpd(), src.value().AsAsmJitGpbLo());
            },
            pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), _, TParaToken::ttReg8, "CL") = [&] {
                a.shr(dst.AsMem(), src.value().AsAsmJitGpbLo());
            },
            // 0xC0 /5 ib	SHR r/m8, imm8	r/m8を2でimm8回符号なし除算します※
            // 0xC1 /5 ib	SHR r/m16, imm8	r/m16を2でimm8回符号なし除算します※
            // 0xC1 /5 ib	SHR r/m32, imm8	r/m32を2でimm8回符号なし除算します※
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttImm, _) | when(dst.IsAsmJitGpbLo()) = [&] {
                a.shr(dst.AsAsmJitGpbLo(), src.value().AsInt32());
            },
            pattern | ds(TParaToken::ttReg8, _, TParaToken::ttImm, _) | when(dst.IsAsmJitGpbHi()) = [&] {
                a.shr(dst.AsAsmJitGpbHi(), src.value().AsInt32());
            },
            pattern | ds(TParaToken::ttReg16, _, TParaToken::ttImm, _) = [&] {
                a.shr(dst.AsAsmJitGpw(), src.value().AsInt32());
            },
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttImm, _) = [&] {
                a.shr(dst.AsAsmJitGpd(), src.value().AsInt32());
            },
            pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), _, TParaToken::ttImm, _) = [&] {
                a.shr(dst.AsMem(), src.value().AsInt32());
            },
            // 0xD0 /5	SHR r/m8		r/m8を2で1回符号なし除算します※
            // 0xD1 /5	SHR r/m16		r/m16を2で1回符号なし除算します※
            // 0xD1 /5	SHR r/m32		r/m32を2で1回符号なし除算します※
            pattern | ds(TParaToken::ttReg8, _, std::nullopt, std::nullopt) = [&] {
                a.db(0xd0);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, dst.AsString(), ModRM::SLASH_5));
            },
            pattern | ds(TParaToken::ttReg16, _, std::nullopt, std::nullopt) = [&] {
                a.db(0xd1);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, dst.AsString(), ModRM::SLASH_5));
            },
            pattern | ds(TParaToken::ttReg32, _, std::nullopt, std::nullopt) = [&] {
                a.db(0xd1);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, dst.AsString(), ModRM::SLASH_5));
            },
            pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16, TParaToken::ttMem32), _, std::nullopt, std::nullopt) = [&] {
                a.db(0xd1);
                a.db(ModRM::generate_modrm(ModRM::REG_REG, "[" + dst.AsString() + "]", ModRM::SLASH_5));
            },
            pattern | _ = [&] {
                throw std::runtime_error("[pass2] SHR, Not implemented or not matched!!!");
            }
        );
    });
}
