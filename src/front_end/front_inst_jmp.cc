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


void FrontEnd::visitLabel(Label x) {

    std::string label = x.substr(0, x.find(":", 0));
    log()->debug("[pass2] label='{}' binout_container[{}]",
                 label, std::to_string(this->binout_container.size()));

    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        auto asmjit_label = code_.labelByName(label.c_str());
        if( ! asmjit_label.isValid() ) {
            asmjit_label = a.newNamedLabel(label.c_str());
        }
        a.bind(asmjit_label);
    });
}

void FrontEnd::processCALL(std::vector<TParaToken>& mnemonic_args) {

    // 0xE8 cw 	CALL rel16 	相対ニアコール、次の命令とディスプレースメント相対
    // 0xE8 cd 	CALL rel32 	相対ニアコール、次の命令とディスプレースメント相対
    // 0xFF /2 	CALL r/m16 	絶対間接ニアコール、r/m16でアドレスを指定
    // 0xFF /2 	CALL r/m32 	絶対間接ニアコール、r/m32でアドレスを指定
    // 0x9A cd 	CALL ptr16:16 	絶対ファーコール、オペランドでアドレスを指定
    // 0x9A cp 	CALL ptr16:32 	絶対ファーコール、オペランドでアドレスを指定
    // 0xFF /3 	CALL m16:16 	絶対間接ファーコール、m16:16でアドレスを指定
    // 0xFF /3 	CALL m16:32 	絶対間接ファーコール、m16:32でアドレスを指定

    auto arg = mnemonic_args[0];
    auto attr = mnemonic_args[0].AsAttr();

    match(attr)(
        pattern | TParaToken::ttLabel = [&] {
            log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());

            with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                using namespace asmjit;
                CodeBuffer& buf = code_.textSection()->buffer();
                const std::string label = arg.AsString();

                // map[] で要素が存在しない場合0となる, pass1でラベルが存在しないということは
                // 呼び出し先のシンボルがEXTERNされているために不定ということである
                // EXTERNの一覧になければエラー
                if (sym_table.count(label) == 0) {
                    if (!o_writer_->has_extern_symbol(label)) {
                        throw std::runtime_error(std::string("CALL, symbol " + label + " is undefined."));
                    }
                    a.db(0xe8);
                    a.dd(0x00000000);
                    sym_table[label] = buf.size() - 4;
                    // あとはリンカが実際のアドレスをCALL命令に埋め込む（シンボル解決）
                    return;
                }

                // 相対ジャンプのオフセット =
                //   対象の絶対アドレス - ( ORGのポジション + ここまでで生成した機械語サイズ ) - CALL命令自体の機械語サイズ
                const auto label_address = sym_table[label];
                const int32_t jmp_offset = label_address - (dollar_position + buf.size()) - 3;
                auto asmjit_label = code_.labelByName(label.c_str());
                if( ! asmjit_label.isValid() ) {
                    a.newNamedLabel(label.c_str());
                }
                match(jmp_offset)(
                    pattern | (-0x8000 <= jmp_offset && jmp_offset <= 0x7fff) = [&] {
                        a.db(0xe8);
                        a.dw(jmp_offset);
                    },
                    pattern | _ = [&] {
                        a.db(0xe8);
                        a.dd(jmp_offset);
                    }
                );
            });
        },
        pattern | _ = [&] {
            throw std::runtime_error("CALL, Not implemented or not matched!!!");
        }
    );

}

template <asmjit::x86::Inst::Id id>
void FrontEnd::processEmitJcc(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
        using namespace asmjit;

        CodeBuffer& buf = code_.textSection()->buffer();
        const std::string label = arg.AsString();
        const auto label_address = sym_table.at(label);
        const int32_t jmp_offset = label_address - (dollar_position + buf.size());

        auto asmjit_label = code_.labelByName(label.c_str());
        if( ! asmjit_label.isValid() ) {
            asmjit_label = a.newNamedLabel(label.c_str());
        }

        match(jmp_offset)(
            pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = [&] {
                a.short_().emit(id, asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().emit(id, asmjit_label);
            }
        );
    });
}

template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJa>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJae>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJb>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJbe>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJc>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJe>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJg>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJge>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJl>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJle>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJna>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnae>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnb>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnbe>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnc>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJne>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJng>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnge>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnl>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnle>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJno>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnp>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJns>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJnz>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJo>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJp>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJpe>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJpo>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJs>(std::vector<TParaToken>& mnemonic_args);
template void FrontEnd::processEmitJcc<asmjit::x86::Inst::kIdJz>(std::vector<TParaToken>& mnemonic_args);


void FrontEnd::processJMP(std::vector<TParaToken>& mnemonic_args) {

    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].GetImmSize()
    );
    auto arg = mnemonic_args[0];
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());

    match(operands)(
        // 0xEB cb  JMP rel8   次の命令との相対オフセットだけ相対ショートジャンプする
        // 0xE9 cw  JMP rel16  次の命令との相対オフセットだけ相対ニアジャンプする
        // 0xE9 cd  JMP rel32  次の命令との相対オフセットだけ相対ニアジャンプする
        // ---
        // asmjitでのJMP即値処理はうまい方法がないので下記のように実装しておく
        // そもそも即値へのジャンプというのが一般的ではないのかもしれない
        // MEMO: https://stackoverflow.com/a/63500826/2565527
        pattern | ds(TParaToken::ttImm, 1) = [&] {
            with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                a.db(0xeb);
                a.db(arg.AsInt32() - dollar_position - code_.codeSize() + 2);
            });
        },
        pattern | ds(TParaToken::ttImm, 2) = [&] {
            with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                a.db(0xe9);
                a.dw(arg.AsInt32() - dollar_position - code_.codeSize() + 2);
            });
        },
        pattern | ds(TParaToken::ttImm, 4) = [&] {
            with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                a.db(0xe9);
                a.dd(arg.AsInt32() - dollar_position - code_.codeSize() + 2);
            });
        },
        // ラベル処理
        pattern | ds(TParaToken::ttLabel, _) = [&] {
            with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                using namespace asmjit;

                CodeBuffer& buf = code_.textSection()->buffer();
                const std::string label = arg.AsString();

                // map[] で要素が存在しない場合0となる, pass1でラベルが存在しないということは
                // 呼び出し先のシンボルがEXTERNされているために不定ということである
                // EXTERNの一覧になければエラー
                if (sym_table.count(label) == 0) {
                    if (!o_writer_->has_extern_symbol(label)) {
                        throw std::runtime_error(std::string("JMP, symbol " + label + " is undefined."));
                    }
                    a.db(0xe9);
                    a.dd(0);
                    sym_table[label] = buf.size() - 4;
                    // あとはリンカが実際のアドレスをJMP命令に埋め込む（シンボル解決）
                    return;
                }

                const auto label_address = sym_table.at(label);
                const int32_t jmp_offset = label_address - (dollar_position + buf.size());

                auto asmjit_label = code_.labelByName(label.c_str());
                if( ! asmjit_label.isValid() ) {
                    asmjit_label = a.newNamedLabel(label.c_str());
                }

                match(jmp_offset)(
                    pattern | _ | when(-0x8000 <= jmp_offset && jmp_offset <= 0x7fff) = [&] {
                        a.short_().jmp(asmjit_label);
                    },
                    pattern | _ = [&] {
                        a.long_().jmp(asmjit_label);
                    }
                );
            });
        },
        pattern | ds(or_(TParaToken::ttMem16, TParaToken::ttMem32), _) = [&] {
            match(std::make_tuple(arg.AsAttr(), arg.AsMem().hasBase()))(
                // 0xEA cd | JMP ptr16:16 | オペランドで指定されるアドレスに絶対ファージャンプする
                // 0xEA cp | JMP ptr16:32 | オペランドで指定されるアドレスに絶対ファージャンプする
                // asmjitではアドレスへの絶対ファージャンプはサポートされてないので, ベースアドレスがあるかないかで分岐させる
                pattern | ds(TParaToken::ttMem16, false) = [&] {
                    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                        auto mem = arg.AsMem();
                        a.db(0xea);
                        a.dw(mem.offset());
                        a.dw(arg.AsSegment());
                    });
                },
                pattern | ds(TParaToken::ttMem32, false) = [&] {
                    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                        pp.require_66h = true;
                        auto mem = arg.AsMem();
                        a.db(0xea);
                        a.dd(mem.offset());
                        a.dw(arg.AsSegment());
                    });
                },
                // 0xFF /5 | JMP m16:16 | m16:16で指定されるアドレスに絶対間接ファージャンプする
                // 0xFF /5 | JMP m16:32 | m16:32で指定されるアドレスに絶対間接ファージャンプする
                pattern | ds(_, true) = [&] {
                    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                        a.jmp(arg.AsMem());
                    });
                },
                pattern | _ = [&] {
                    throw std::runtime_error("JMP, Not implemented or not matched!!!");
                }
            );
        },
        pattern | _ = [&] {
            throw std::runtime_error("JMP, Not implemented or not matched!!!");
        }
    );
}
