#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "x86.hh"
#include <gtest/gtest.h>

using namespace x86_64;
using namespace jsoncons;


class X86TableSuite : public ::testing::Test {

protected:
    // 試験開始時に一回だけ実行
    X86TableSuite() {
    }

    // 試験終了時に一回だけ実行
    ~X86TableSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
        // spdlog
        if(!spdlog::get("opennask")) {
            auto logger = spdlog::stdout_color_st("opennask");
        }
        spdlog::set_level(spdlog::level::trace);
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(X86TableSuite, CheckJsonSchema)
{
    try {
        json j = json::parse(std::string(X86_64_JSON));
        const json& inst = j["instructions"];
        for (const auto &item : inst.object_range()) {
            // debug code
            // std::string opcode = item.key();
            // std::cout << pretty_print(item.value()) << std::endl;
            // break;
            Instruction inst = \
                jsoncons::decode_json<Instruction>(item.value().to_string());
        }

        InstructionSet iset =                                           \
            jsoncons::decode_json<InstructionSet>(std::string(X86_64_JSON));

        EXPECT_EQ("x86-64", iset.instruction_set());
        EXPECT_EQ(1216, iset.instructions().size());
        EXPECT_EQ(1, iset.instructions().count("ADC"));
        EXPECT_EQ("Add with Carry", iset.instructions().at("ADC").summary());

    } catch(const jsoncons::conv_error& e) {
        std::cout << "Caught conv_error with category "
                  << e.code().category().name()
                  << ", code " << e.code().value()
                  << " and message " << e.what()
                  << " and line " << e.line()
                  << std::endl;
    }
}

//
// トークン読み取りパラメタライズテスト
//
struct TokenToX86TypeParam {
    const std::string token;
    const TParaToken para_token;
    std::string expected;

    TokenToX86TypeParam(
        const std::string& token,
        const TParaToken::TIdentiferAttribute attr,
        std::string expected
    ):
        para_token(TParaToken(token, TParaToken::ttUnknown, attr)),
        expected(expected)
    {
    }
};

void PrintTo(const TokenToX86TypeParam& param, ::std::ostream* os) {
    *os << param.para_token.to_string();
}

class TokenToX86Type : public testing::TestWithParam<TokenToX86TypeParam> {
};

TEST_P(TokenToX86Type, TokenToX86Type) {
    const auto p = GetParam();

    EXPECT_EQ(token_to_x86_type(p.para_token), p.expected);
}

INSTANTIATE_TEST_SUITE_P(X86TableSuite, TokenToX86Type,
    testing::Values(
        TokenToX86TypeParam("AL", TParaToken::ttReg8, "r8"),
        TokenToX86TypeParam("AX", TParaToken::ttReg16, "r16"),
        TokenToX86TypeParam("EAX", TParaToken::ttReg32, "r32"),
        TokenToX86TypeParam("RAX", TParaToken::ttReg64, "r64"),
        TokenToX86TypeParam("[AL]", TParaToken::ttMem8, "m8"),
        TokenToX86TypeParam("[AX]", TParaToken::ttMem16, "m16"),
        TokenToX86TypeParam("[EAX]", TParaToken::ttMem32, "m32"),
        TokenToX86TypeParam("[RAX]", TParaToken::ttMem64, "m64"),
        TokenToX86TypeParam("0x80", TParaToken::ttImm, "imm")
    )
);

//
// 命令ごとの機械語サイズ取得パラメタライズテスト
//
struct InstToMachineCodeSizeParam {

    // ここからはパラメーター
    const OPENNASK_MODES _bit_mode;
    const std::string _opcode;
    const std::vector<TParaToken> _tokens; // 「&」ではなく実体にしないとテスト実行時データが消える
    const size_t _expected;

    InstToMachineCodeSizeParam(
        const OPENNASK_MODES bit_mode,
        const std::string opcode,
        const std::vector<TParaToken>& tokens,
        const size_t expected
    ):
        _bit_mode(bit_mode),
        _opcode(opcode),
        _tokens(tokens),
        _expected(expected)
    {
    }
};

void PrintTo(const InstToMachineCodeSizeParam& param, ::std::ostream* os) {
    *os << param._opcode << "_";
    for (int i = 0; i < param._tokens.size(); i++ ) {
        auto t = param._tokens[i];
        *os << t.AsString();
        if (i != param._tokens.size() - 1) {
            *os << "_";
        }
    }
}

class InstToMachineCodeSize : public testing::TestWithParam<InstToMachineCodeSizeParam> {

public:
    // x86命令セット; データが大きいのでここで宣言
    std::unique_ptr<x86_64::InstructionSet> _iset;

protected:
    // 試験開始時に一回だけ実行
    InstToMachineCodeSize() {
        _iset = std::make_unique<x86_64::InstructionSet>(jsoncons::decode_json<x86_64::InstructionSet>(std::string(x86_64::X86_64_JSON)));
        if(!spdlog::get("opennask")) {
            auto logger = spdlog::stdout_color_st("opennask");
        }
    }

    // 試験終了時に一回だけ実行
    ~InstToMachineCodeSize() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
        //spdlog::set_level(spdlog::level::debug);
        spdlog::set_level(spdlog::level::trace);
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_P(InstToMachineCodeSize, InstToMachineCodeSize) {

    const auto p = GetParam();
    auto inst = _iset->instructions().at(p._opcode);
    auto size = inst.get_output_size(p._bit_mode, p._tokens);

    EXPECT_EQ(size, p._expected);
}

// パラメタライズテスト；
// 引数:
// * 16bit/32bitモード
// * オペコード
// * 機械語サイズを出したいオペランドのTParaTokenの可変長引数
// * 期待される機械語サイズ
INSTANTIATE_TEST_SUITE_P(X86TableSuite, InstToMachineCodeSize,
    testing::Values(
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "ADD",
                                   {
                                       TParaToken("[BX]", TParaToken::ttIdentifier, TParaToken::ttMem16).SetMem(asmjit::x86::ptr(asmjit::x86::bx)),
                                       TParaToken("AX", TParaToken::ttIdentifier, TParaToken::ttReg16)
                                   },
                                   2),
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "INT",
                                   {
                                       TParaToken("0x10", TParaToken::ttHex, TParaToken::ttImm)
                                   },
                                   2),
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "CALL",
                                   {
                                       TParaToken("waitkbdout", TParaToken::ttIdentifier, TParaToken::ttRel32)
                                   },
                                   5),
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       TParaToken("AL", TParaToken::ttIdentifier, TParaToken::ttReg8),
                                       TParaToken("[SI]", TParaToken::ttIdentifier, TParaToken::ttMem8).SetMem(asmjit::x86::ptr(asmjit::x86::si))
                                   },
                                   2),
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       TParaToken("AX", TParaToken::ttIdentifier, TParaToken::ttReg16),
                                       TParaToken("0", TParaToken::ttInteger, TParaToken::ttImm)
                                   },
                                   3),
        // 0xc6, 0x06, 0xf2, 0x0f, 0x08
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       TParaToken("0x0ff2", TParaToken::ttHex, TParaToken::ttMem8),
                                       TParaToken("8", TParaToken::ttInteger, TParaToken::ttImm)
                                   },
                                   5),
        // 0xc7, 0x06, 0xf4, 0x0f, 0x40, 0x01
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       TParaToken("0x0ff4", TParaToken::ttHex, TParaToken::ttMem16),
                                       TParaToken("320", TParaToken::ttInteger, TParaToken::ttImm)
                                   },
                                   6),
        // MOV DWORD [VRAM],0x000a0000  ; VRAM=0x0ff8
        // 0x66, 0xc7, 0x06, 0xf8, 0x0f, 0x00, 0x00, 0x0a, 0x00
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       TParaToken("0x0ff8", TParaToken::ttHex, TParaToken::ttMem32),
                                       TParaToken("0x000a0000", TParaToken::ttHex, TParaToken::ttImm)
                                   },
                                   9),
        // 0x8A, 0x0E, 0xF00F = 4byte
        // 通常の機械語サイズにメモリーアドレス表現で示されるオフセット部分のサイズを足す
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       TParaToken("CL", TParaToken::ttIdentifier, TParaToken::ttReg8),
                                       TParaToken("0x0ff0", TParaToken::ttHex, TParaToken::ttMem8)
                                       // ↑本来 BYTE [0x0ff0] と設定されるが単体テスト上こうなる
                                       // parseされる中でtokenがこの値を保持するようになる
                                   },
                                   4),
        // 0x0D id    = 1(prefix) + 1 + 4 byte     = 6byte
        // 0x83 /1 ib = 1(prefix) + 1 + 1 + 1 byte = 4byte
        // 出力される機械語が少ない方を優先的に選ぶ
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "OR",
                                   {
                                       TParaToken("EAX", TParaToken::ttIdentifier, TParaToken::ttReg32),
                                       TParaToken("0x00000001", TParaToken::ttHex, TParaToken::ttImm)
                                   },
                                   4),
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "IMUL",
                                   {
                                       TParaToken("ECX", TParaToken::ttIdentifier, TParaToken::ttReg32),
                                       TParaToken("4608", TParaToken::ttInteger, TParaToken::ttImm)
                                   },
                                   7),
        // 0x88,0x2e,0xf0,0x0f
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       // pass1でttMem16->ttMem8に変換される
                                       // 本来 BYTE [0x0ff0] と設定されるが単体テスト上こうなる
                                       // parseされる中でtokenがこの値を保持するようになる
                                       TParaToken("0x0ff0", TParaToken::ttHex, TParaToken::ttMem8),
                                       TParaToken("CH", TParaToken::ttIdentifier, TParaToken::ttReg8)
                                   },
                                   4),
        // 0x66, 0x81, 0xe9, 0x80, 0x00, 0x00, 0x00
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "SUB",
                                   {
                                       TParaToken("ECX", TParaToken::ttIdentifier, TParaToken::ttReg32),
                                       TParaToken("128", TParaToken::ttInteger, TParaToken::ttImm)
                                   },
                                   7),
        // 0x67, 0x66, 0x8b, 0x4b, 0x10
        InstToMachineCodeSizeParam(ID_16BIT_MODE, "MOV",
                                   {
                                       TParaToken("ECX", TParaToken::ttIdentifier, TParaToken::ttReg32),
                                       TParaToken("EBX", TParaToken::ttIdentifier, TParaToken::ttMem32).SetMem(asmjit::x86::ptr(asmjit::x86::ebx, 16))
                                   },
                                   5),
        InstToMachineCodeSizeParam(ID_32BIT_MODE, "MOV",
                                   {
                                       TParaToken("AX", TParaToken::ttIdentifier, TParaToken::ttReg16),
                                       TParaToken("SS", TParaToken::ttIdentifier, TParaToken::ttSreg)
                                   },
                                   3)
    )
);
