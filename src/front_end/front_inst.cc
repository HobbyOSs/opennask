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


void FrontEnd::processADD(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsString(),
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsAttr()
    );
    auto dst = mnemonic_args[0];
    log()->debug("[pass2] processADD dst={}", dst.AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, dst);

        match(operands)(
            // 0x04 ib		ADD AL, imm8		imm8をALに加算する
            // 0x05 iw		ADD AX, imm16		imm16をAXに加算する
            // 0x05 id		ADD EAX, imm32		imm32をEAXに加算する
            pattern | ds("AL", TParaToken::ttReg8 , TParaToken::ttImm) = [&] {
                a.add(x86::al, mnemonic_args[1].AsInt32());
            },
            pattern | ds("AX", TParaToken::ttReg16, TParaToken::ttImm) = [&] {
                // 他のassemblerだと通常はこの場合0x83で処理するようだ
                a.db(0x05);
                a.dw(mnemonic_args[1].AsInt32());
            },
            pattern | ds("EAX", TParaToken::ttReg32, TParaToken::ttImm) = [&] {
                a.add(x86::eax, mnemonic_args[1].AsInt32());
            },
            // TODO: メモリーアドレッシング
            // 0x80 /0 ib	ADD r/m8, imm8		imm8をr/m8に加算する
            pattern | ds(_, TParaToken::ttReg8 , TParaToken::ttImm) | when(dst.IsAsmJitGpbLo()) = [&] {
                a.add(mnemonic_args[0].AsAsmJitGpbLo(), mnemonic_args[1].AsInt32());
            },
            pattern | ds(_, TParaToken::ttReg8 , TParaToken::ttImm) | when(dst.IsAsmJitGpbHi()) = [&] {
                a.add(mnemonic_args[0].AsAsmJitGpbHi(), mnemonic_args[1].AsInt32());
            },

            // 0x81 /0 iw	ADD r/m16, imm16	imm16をr/m16に加算する
            // 0x83 /0 ib	ADD r/m16, imm8		符号拡張imm8をr/m16に加算する
            pattern | ds(_, TParaToken::ttReg16, TParaToken::ttImm) = [&] {
                a.add(mnemonic_args[0].AsAsmJitGpw(), mnemonic_args[1].AsInt32());
            },
            // 0x81 /0 id	ADD r/m32, imm32	imm32をr/m32に加算する
            // 0x83 /0 ib	ADD r/m32, imm8		符号拡張imm8をr/m32に加算する
            pattern | ds(_, TParaToken::ttReg32, TParaToken::ttImm) = [&] {
                a.add(mnemonic_args[0].AsAsmJitGpd(), mnemonic_args[1].AsInt32());
            },

            // 0x00 /r		ADD r/m8, r8		r8をr/m8に加算する
            // 0x01 /r		ADD r/m16, r16		r16をr/m16に加算する
            // 0x01 /r		ADD r/m32, r32		r32をr/m32に加算する
            // 0x02 /r		ADD r8, r/m8		r/m8をr8に加算する
            // 0x03 /r		ADD r16, r/m16		r/m16をr16に加算する
            // 0x03 /r		ADD r32, r/m32		r/m32をr32に加算する
            pattern | _ = [&] {
                throw std::runtime_error("ADD, Not implemented or not matched!!!");
            }
        );
    });
}

void FrontEnd::processCLI() {
    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        a.cli();
    });
}

void FrontEnd::processCMP(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;
    auto operands = std::make_tuple(
        mnemonic_args[0].AsString(),
        mnemonic_args[0].AsAttr(),
        mnemonic_args[1].AsAttr()
    );
    auto dst = mnemonic_args[0];
    log()->debug("[pass2] processCMP dst={}", dst.AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;
        pp.set(bit_mode, dst);

        match(operands)(
            // 0x3C ib		CMP AL, imm8		imm8をALと比較します
            // 0x3D iw		CMP AX, imm16		imm16をAXと比較します
            // 0x3D id		CMP EAX, imm32		imm32をEAXと比較します
            pattern | ds("AL", TParaToken::ttReg8, TParaToken::ttImm) = [&] {
                a.cmp(x86::al, mnemonic_args[1].AsInt32());
            },
            pattern | ds("AX", TParaToken::ttReg16, TParaToken::ttImm) = [&] {
                a.db(0x3d);
                a.dw(mnemonic_args[1].AsInt32());
            },
            pattern | ds("EAX", TParaToken::ttReg32, TParaToken::ttImm) = [&] {
                a.db(0x3d);
                a.dd(mnemonic_args[1].AsInt32());
            },
            // 0x80 /7 ib	CMP r/m8, imm8		imm8をr/m8と比較します
            // 0x81 /7 iw	CMP r/m16, imm16	imm16をr/m16と比較します <-- ?
            // 0x80 /7 id	CMP r/m32, imm32	imm32をr/m32と比較します <-- ?
            // 0x83 /7 ib	CMP r/m16, imm8		imm8をr/m16と比較します
            // 0x83 /7 ib	CMP r/m32, imm8		imm8をr/m32と比較します
            pattern | ds(_, TParaToken::ttReg8 , TParaToken::ttImm) | when(dst.IsAsmJitGpbLo()) = [&] {
                a.cmp(mnemonic_args[0].AsAsmJitGpbLo(), mnemonic_args[1].AsInt32());
            },
            pattern | ds(_, TParaToken::ttReg8 , TParaToken::ttImm) | when(dst.IsAsmJitGpbHi()) = [&] {
                a.cmp(mnemonic_args[0].AsAsmJitGpbHi(), mnemonic_args[1].AsInt32());
            },
            pattern | ds(_, TParaToken::ttReg16, TParaToken::ttImm) = [&] {
                a.cmp(mnemonic_args[0].AsAsmJitGpw(), mnemonic_args[1].AsInt32());
            },
            pattern | ds(_, TParaToken::ttReg32, TParaToken::ttImm) = [&] {
                a.cmp(mnemonic_args[0].AsAsmJitGpd(), mnemonic_args[1].AsInt32());
            },
            // 0x38 /r		CMP r/m8, r8		r8をr/m8と比較します
            // 0x39 /r		CMP r/m16, r16		r16をr/m16と比較します
            // 0x39 /r		CMP r/m32, r32		r32をr/m32と比較します
            // 0x3A /r		CMP r8, r/m8		r/m8をr8と比較します
            // 0x3B /r		CMP r16, r/m16		r/m16をr16と比較します
            // 0x3B /r		CMP r32, r/m32		r/m32をr32と比較します

            pattern | _ = [&] {
                throw std::runtime_error("CMP, Not implemented or not matched!!!");
            }
        );
    });
}

void FrontEnd::processHLT() {
    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        a.hlt();
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

void FrontEnd::processLGDT(std::vector<TParaToken>& mnemonic_args) {
    // 0x0F 01 /2	LGDT m16& 32	mをGDTRにロードします
    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);

    const std::string src_mem = "[" + arg.AsString() + "]";

    std::vector<uint8_t> b = {0x0f, 0x01};

    binout_container.insert(binout_container.end(),
                            std::begin(b),
                            std::end(b));

    std::string label = arg.AsString();
    const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, src_mem, ModRM::SLASH_2);

    if (LabelJmp::dst_is_stored(label, label_dst_list)) {
        LabelJmp::update_label_src_offset(label, label_dst_list, modrm, binout_container);
    } else {
        LabelJmp::store_label_src(label, label_src_list, binout_container);
        binout_container.push_back(modrm);
        binout_container.push_back(0x00);
        binout_container.push_back(0x00);
    }

    return;
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
               match( std::make_tuple( dst.IsAsmJitGpbLo() ))(
                   // "MOV AL,[SI]", "MOV AL,[ESP+8]" のようなパターン
                   pattern | ds(true)  = [&] { a.mov(dst.AsAsmJitGpbLo(), src.AsMem() ); },
                   pattern | ds(false) = [&] { a.mov(dst.AsAsmJitGpbHi(), src.AsMem() ); }
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
            //pattern | ds(TParaToken::ttReg16, dst, TParaToken::ttMem16, src) = [&] {
            //    const std::string src_mem = "[" + *src + "]";
            //    const std::string dst_reg = *dst;
            //
            //    const uint8_t base = 0x8b;
            //    const uint8_t modrm = ModRM::generate_modrm(0x8e, ModRM::REG_REG, src_mem, dst_reg);
            //    std::vector<uint8_t> b = {base, modrm};
            //    //return b;
            //},
            //// A1      eax     moffs32
            //pattern | ds(TParaToken::ttReg32, "EAX", _, _) = [&] {
            //
            //},
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

            //// 89      r32     r32
            //pattern | ds(TParaToken::ttReg32, _, TParaToken::ttReg32, _) = [&] {
            //},
            // 8B      r32     m32
            pattern | ds(TParaToken::ttReg32, _, TParaToken::ttMem32, _) = [&] {
                a.mov(dst.AsAsmJitGpd(), src.AsMem() );
            },
            //// A1      rax     moffs64
            //pattern | ds(TParaToken::ttReg64, "RAX", _, _) = [&] {
            //},
            //// C7      r64     imm32
            //// B8      r64     imm64
            //pattern | ds(TParaToken::ttReg64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            //},
            //// 89      r64     r64
            //pattern | ds(TParaToken::ttReg64, _, TParaToken::ttReg64, _) = [&] {
            //},
            //// 8B      r64     m64
            //pattern | ds(TParaToken::ttReg64, _, TParaToken::ttMem64, _) = [&] {
            //},
            // C6      m8      imm8
            pattern | ds(TParaToken::ttMem8, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
                std::string dst_mem = "[" + dst.AsString() + "]";
                const uint8_t modrm = ModRM::generate_modrm(ModRM::REG_REG, dst_mem, ModRM::SLASH_0);
                // TODO: かなり間に合わせな実装、できればasmtkなどを使ってカスタマイズしたい
                // asmjit使用時にメモリーアドレッシング時にオフセットのみのMOVは機械語が想定と違う
                a.db(0xc6);
                a.db(modrm);
                a.dw(mnemonic_args[0].AsInt32());
                a.db(mnemonic_args[1].AsInt32());
            },
            // A2      moffs8  al
            pattern | ds(or_(TParaToken::ttMem8, TParaToken::ttMem16), _, TParaToken::ttReg8, "AL") = [&] {
                a.db(0xa2);
                auto addr = mnemonic_args[0].AsUInt32();
                a.dw(addr);
            },
            // A3      moffs16 ax
            //pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, "AX") = [&] {
            //    const uint8_t base = 0xa3;
            //    std::vector<uint8_t> b = {base};
            //    auto addr = mnemonic_args[0].AsUInt16t();
            //    std::copy(addr.begin(), addr.end(), std::back_inserter(b));
            //    //return b;
            //},
            // A3      moffs32 eax
            //pattern | ds(_, _, TParaToken::ttReg32, "EAX") = [&] {
            //    const uint8_t base = 0xa3;
            //    std::vector<uint8_t> b = {base};
            //    auto addr = mnemonic_args[0].AsUInt32t();
            //    std::copy(addr.begin(), addr.end(), std::back_inserter(b));
            //    //return b;
            //},
            // 88      m8      r8
            pattern | ds(TParaToken::ttMem8, _, TParaToken::ttReg8, _) = [&] {
                // TODO: test & メモリーアドレッシング
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

                if (src.IsAsmJitGpbLo()) {
                    a.mov(x86::byte_ptr(dst.AsInt32()), src.AsAsmJitGpbLo() );
                } else if (src.IsAsmJitGpbHi()) {
                    a.mov(x86::byte_ptr(dst.AsInt32()), src.AsAsmJitGpbHi() );
                }
            },
            // 88      m16     r8 (m16の場合下位8ビットが使われる)
            pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg8, _) = [&] {
                // TODO: test & メモリーアドレッシング
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

                // TODO: 実装がとても雑
                if (src.IsAsmJitGpbLo()) {
                    a.mov(x86::byte_ptr(dst.AsInt32()), src.AsAsmJitGpbLo() );
                } else if (src.IsAsmJitGpbHi()) {
                    a.mov(x86::byte_ptr(dst.AsInt32()), src.AsAsmJitGpbHi() );
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
            pattern | ds(TParaToken::ttMem16, _, TParaToken::ttReg16, _) = [&] {
                // TODO: test & メモリーアドレッシング
                a.mov(x86::word_ptr(dst.AsAsmJitGpw()), src.AsAsmJitGpw());
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
            //// 89      m32     r32
            //pattern | ds(TParaToken::ttMem32, _, TParaToken::ttReg32, _) = [&] {
            //},
            //// C7      m64     imm32
            //pattern | ds(TParaToken::ttMem64, _, or_(TParaToken::ttImm, TParaToken::ttLabel), _) = [&] {
            //},
            //// 89      m64     r64
            //pattern | ds(TParaToken::ttMem64, _, TParaToken::ttReg64, _) = [&] {
            //},
            //// A3      moffs64 rax
            //pattern | ds(_, _, TParaToken::ttReg64, "RAX") = [&] {
            //},
            // 8E/r      Sreg, r/m16
            pattern | ds(TParaToken::ttSreg, _, TParaToken::ttReg16, _) = [&] {
                a.mov(dst.AsAsmJitSReg(), src.AsAsmJitGpw());
            },
            // 8C/r      r/m16, Sreg
            pattern | ds(TParaToken::ttReg16, _, TParaToken::ttSreg, _) = [&] {
                a.mov(dst.AsAsmJitGpw(), src.AsAsmJitSReg());
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

void FrontEnd::processNOP() {
    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        a.nop();
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
        pp.set(bit_mode, dst, src);

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
                a.out(x86::dx, x86::al);
            },
            pattern | ds("DX", _, "AX", _) = [&] {
                a.out(x86::dx, x86::ax);
            },
            pattern | ds("DX", _, "EAX", _) = [&] {
                a.out(x86::dx, x86::eax);
            },
            pattern | _ = [&] {
                throw std::runtime_error("OUT, Not implemented or not matched!!!");
            }
        );
    });
}


void PrefixInfo::set(OPENNASK_MODES bit_mode, TParaToken& dst) {
    // 16bit命令モードで32bitのアドレッシング・モードを使うときこれが必要
    // 32bit命令モードで16bit命令が現れるのであれば16bitレジスタを選択するためこれが必要
    require_67h = match(bit_mode)(
        pattern | ID_16BIT_MODE | when(dst.IsAsmJitGpd()) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | ID_32BIT_MODE | when(dst.IsAsmJitGpw()) = true,
        pattern | ID_32BIT_MODE = false,
        pattern | _ = false
    );

    // 16bit命令モードで32bitレジスタが使われていればこれが必要
    require_66h = match(bit_mode)(
        pattern | ID_16BIT_MODE | when(dst.IsAsmJitGpd()) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | _ = false
    );
}

void PrefixInfo::set(OPENNASK_MODES bit_mode, TParaToken& dst, TParaToken& src) {

    require_67h = match(bit_mode)(
        pattern | ID_16BIT_MODE | when(dst.IsAsmJitGpd()||src.IsAsmJitGpd()) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | ID_32BIT_MODE | when(dst.IsAsmJitGpw()||src.IsAsmJitGpw()) = true,
        pattern | ID_32BIT_MODE = false,
        pattern | _ = false
    );
    require_66h = match(bit_mode)(
        pattern | ID_16BIT_MODE | when(dst.IsAsmJitGpd()||src.IsAsmJitGpd()) = true,
        pattern | ID_16BIT_MODE = false,
        pattern | _ = false
    );
}
