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
                const auto label_address = sym_table.at(label);
                const int32_t jmp_offset = label_address - (dollar_position + buf.size());
                auto asmjit_label = code_.labelByName(label.c_str());
                if( ! asmjit_label.isValid() ) {
                    a.newNamedLabel(label.c_str());
                }
                match(jmp_offset)(
                    pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = [&] {
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

void FrontEnd::processJAE(std::vector<TParaToken>& mnemonic_args) {

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
                a.short_().jae(asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().jae(asmjit_label);
            }
        );
    });
}

void FrontEnd::processJB(std::vector<TParaToken>& mnemonic_args) {

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
                a.short_().jb(asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().jb(asmjit_label);
            }
        );
    });
}

void FrontEnd::processJBE(std::vector<TParaToken>& mnemonic_args) {

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
                a.short_().jbe(asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().jbe(asmjit_label);
            }
        );
    });
}

void FrontEnd::processJC(std::vector<TParaToken>& mnemonic_args) {

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
                a.short_().jc(asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().jc(asmjit_label);
            }
        );
    });
}

void FrontEnd::processJE(std::vector<TParaToken>& mnemonic_args) {

    // 0x74 0xCB        JE rel8       (ZF=1で)等しい場合ショートジャンプする
    // 0x0F 0x84 cw/cd  JE rel16/32   (ZF=1で)等しい場合ニアジャンプする
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
                a.short_().je(asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().je(asmjit_label);
            }
        );
    });
}

void FrontEnd::processJMP(std::vector<TParaToken>& mnemonic_args) {

    // 0xEB cb  JMP rel8   次の命令との相対オフセットだけ相対ショートジャンプする
    // 0xE9 cw  JMP rel16  次の命令との相対オフセットだけ相対ニアジャンプする
    // 0xE9 cd  JMP rel32  次の命令との相対オフセットだけ相対ニアジャンプする
    using namespace matchit;
    using Attr = TParaToken::TIdentiferAttribute;

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].GetImmSize()
    );
    auto arg = mnemonic_args[0];

    match(operands)(
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
                        a.short_().jmp(asmjit_label);
                    },
                    pattern | _ = [&] {
                        a.long_().jmp(asmjit_label);
                    }
                );
            });
        },
        pattern | _ = [&] {
            throw std::runtime_error("JMP, Not implemented or not matched!!!");
        }
    );
}

void FrontEnd::processJNC(std::vector<TParaToken>& mnemonic_args) {

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
                a.short_().jnc(asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().jnc(asmjit_label);
            }
        );
    });
}

void FrontEnd::processJZ(std::vector<TParaToken>& mnemonic_args) {

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
                a.short_().jz(asmjit_label);
            },
            pattern | _ = [&] {
                a.long_().jz(asmjit_label);
            }
        );
    });
}
