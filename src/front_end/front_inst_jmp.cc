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

    // label: (label_dstと呼ぶ)
    // 1) label_dstの位置を記録する → label_dst_list
    // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了
    LabelJmp::store_label_dst(label, label_dst_list, binout_container);
    LabelJmp::update_label_dst_offset(label, label_src_list, dollar_position, binout_container);

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

    auto operands = std::make_tuple(
        mnemonic_args[0].AsAttr(),
        mnemonic_args[0].GetImmSize()
    );
    auto arg = mnemonic_args[0];

    std::vector<uint8_t> machine_codes = match(operands)(
        // 0xE8 cw 	CALL rel16 	相対ニアコール、次の命令とディスプレースメント相対
        // 0xE8 cd 	CALL rel32 	相対ニアコール、次の命令とディスプレースメント相対
        // 0xFF /2 	CALL r/m16 	絶対間接ニアコール、r/m16でアドレスを指定
        // 0xFF /2 	CALL r/m32 	絶対間接ニアコール、r/m32でアドレスを指定
        // 0x9A cd 	CALL ptr16:16 	絶対ファーコール、オペランドでアドレスを指定
        // 0x9A cp 	CALL ptr16:32 	絶対ファーコール、オペランドでアドレスを指定
        // 0xFF /3 	CALL m16:16 	絶対間接ファーコール、m16:16でアドレスを指定
        // 0xFF /3 	CALL m16:32 	絶対間接ファーコール、m16:32でアドレスを指定

        pattern | ds(TParaToken::ttLabel, _) = [&] {
            log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());
            std::string label = arg.AsString();

            if (LabelJmp::dst_is_stored(label, label_dst_list)) {
                LabelJmp::update_label_src_offset(label, label_dst_list, 0xe8, binout_container);
            } else {
                // TODO: rel16/rel32の２通りの値が入る可能性があるが、動的に出力に機械語を入れる仕組みがないので
                // とりあえずrel16として処理している
                LabelJmp::store_label_src(label, label_src_list, binout_container);
                binout_container.push_back(0xe8);
                binout_container.push_back(0x00);
                binout_container.push_back(0x00);
            }

            return std::vector<uint8_t>();
        },
        pattern | _ = [&] {
            throw std::runtime_error("CALL, Not implemented or not matched!!!");
            return std::vector<uint8_t>();
        }
    );

    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(machine_codes),
                            std::end(machine_codes));
    return;
}

void FrontEnd::processJAE(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();
    auto label_address = sym_table.at(label);
    std::vector<uint8_t> bytes = {};

    // pass1のシンボルテーブルを使う
    auto jmp_offset = label_address - dollar_position - binout_container.size();
    match(static_cast<int64_t>(jmp_offset))(
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = [&] {
            bytes.push_back(0x73);
            auto b = IntAsByte(jmp_offset - (1 + NASK_BYTE));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x83);
            auto b = IntAsWord(jmp_offset - (1 + NASK_WORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x83);
            auto b = LongAsDword(jmp_offset - (1 + NASK_DWORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        }
    );
    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(bytes),
                            std::end(bytes));
    return;
}

void FrontEnd::processJB(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();
    auto label_address = sym_table.at(label);
    std::vector<uint8_t> bytes = {};

    // pass1のシンボルテーブルを使う
    auto jmp_offset = label_address - dollar_position - binout_container.size();
    match(static_cast<int64_t>(jmp_offset))(
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = [&] {
            bytes.push_back(0x72);
            auto b = IntAsByte(jmp_offset - (1 + NASK_BYTE));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x82);
            auto b = IntAsWord(jmp_offset - (1 + NASK_WORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x82);
            auto b = LongAsDword(jmp_offset - (1 + NASK_DWORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        }
    );
    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(bytes),
                            std::end(bytes));
    return;
}

void FrontEnd::processJBE(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();
    auto label_address = sym_table.at(label);
    std::vector<uint8_t> bytes = {};

    // pass1のシンボルテーブルを使う
    auto jmp_offset = label_address - dollar_position - binout_container.size();
    match(static_cast<int64_t>(jmp_offset))(
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = [&] {
            bytes.push_back(0x76);
            auto b = IntAsByte(jmp_offset - (1 + NASK_BYTE));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x86);
            auto b = IntAsWord(jmp_offset - (1 + NASK_WORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x86);
            auto b = LongAsDword(jmp_offset - (1 + NASK_DWORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        }
    );
    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(bytes),
                            std::end(bytes));
    return;
}

void FrontEnd::processJC(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());
    std::string label = arg.AsString();
    auto label_address = sym_table.at(label);
    std::vector<uint8_t> bytes = {};

    // pass1のシンボルテーブルを使う
    auto jmp_offset = label_address - dollar_position - binout_container.size();
    match(static_cast<int64_t>(jmp_offset))(
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = [&] {
            bytes.push_back(0x72);
            auto b = IntAsByte(jmp_offset - (1 + NASK_BYTE));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x82);
            auto b = IntAsWord(jmp_offset - (1 + NASK_WORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x82);
            auto b = LongAsDword(jmp_offset - (1 + NASK_DWORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        }
    );
    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(bytes),
                            std::end(bytes));
    return;
}

void FrontEnd::processJE(std::vector<TParaToken>& mnemonic_args) {

    // 0x74 0xCB        JE rel8       (ZF=1で)等しい場合ショートジャンプする
    // 0x0F 0x84 cw/cd  JE rel16/32   (ZF=1で)等しい場合ニアジャンプする
    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttIdentifier);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsString());

    std::string label = arg.AsString();
    auto label_address = sym_table.at(label);
    std::vector<uint8_t> machine_codes = {0x74};

    auto jmp_offset = label_address - dollar_position - binout_container.size();
    match(static_cast<int64_t>(jmp_offset))(
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = [&] {
            // ショートジャンプ
            auto b = IntAsByte(jmp_offset - (1 + NASK_BYTE));
            std::copy(b.begin(), b.end(), std::back_inserter(machine_codes));
        },
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = [&] {
            // ニアジャンプ
            auto b = IntAsWord(label_address);
            std::copy(b.begin(), b.end(), std::back_inserter(machine_codes));
        },
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = [&] {
            // ニアジャンプ
            auto b = LongAsDword(label_address);
            std::copy(b.begin(), b.end(), std::back_inserter(machine_codes));
        }
    );

    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(machine_codes),
                            std::end(machine_codes));
    return;
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
        // 即値処理
        pattern | ds(TParaToken::ttImm, 1) = [&] {
            with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                const auto imm = (arg.AsInt32() - dollar_position - binout_container.size()) + 1;
                a.short_().jmp(imm);
            });
        },
        pattern | ds(TParaToken::ttImm, or_(2, 4)) = [&] {
            with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& pp) {
                const auto imm = (arg.AsInt32() - dollar_position - binout_container.size()) + 1;
                a.long_().jmp(imm);
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
    std::string label = arg.AsString();
    auto label_address = sym_table.at(label);
    std::vector<uint8_t> bytes = {};

    // pass1のシンボルテーブルを使う
    auto jmp_offset = label_address - dollar_position - binout_container.size();
    match(static_cast<int64_t>(jmp_offset))(
        pattern | (std::numeric_limits<int8_t>::min() <= _ && _ <= std::numeric_limits<int8_t>::max()) = [&] {
            bytes.push_back(0x73);
            auto b = IntAsByte(jmp_offset - (1 + NASK_BYTE));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int16_t>::min() <= _ && _ <= std::numeric_limits<int16_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x83);
            auto b = IntAsWord(jmp_offset - (1 + NASK_WORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        },
        pattern | (std::numeric_limits<int32_t>::min() <= _ && _ <= std::numeric_limits<int32_t>::max()) = [&] {
            bytes.push_back(0x0F);
            bytes.push_back(0x83);
            auto b = LongAsDword(jmp_offset - (1 + NASK_DWORD));
            std::copy(b.begin(), b.end(), std::back_inserter(bytes));
        }
    );
    // 結果を投入
    binout_container.insert(binout_container.end(),
                            std::begin(bytes),
                            std::end(bytes));
    return;
}
