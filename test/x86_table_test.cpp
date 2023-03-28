#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "x86.hh"
#include <gtest/gtest.h>
#include "nask_defs.hpp"

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

// テストデータクラス
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

    EXPECT_EQ(token_to_x86_type(&p.para_token), p.expected);
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


TEST_F(X86TableSuite, ADDGetOutputSize)
{
    auto iset = decode_json<InstructionSet>(std::string(X86_64_JSON));
    auto inst = iset.instructions().at("ADD");
    auto size = inst.get_output_size(
        ID_16BIT_MODE,
        {
            TParaToken("[BX]", TParaToken::ttIdentifier, TParaToken::ttMem16),
            TParaToken("AX", TParaToken::ttIdentifier, TParaToken::ttReg16)
        }
    );
    EXPECT_EQ(2, size);
}

TEST_F(X86TableSuite, INTGetOutputSize)
{
    auto iset = decode_json<InstructionSet>(std::string(X86_64_JSON));
    auto inst = iset.instructions().at("INT");
    auto size = inst.get_output_size(
        ID_16BIT_MODE,
        {
            TParaToken("0x10", TParaToken::ttHex, TParaToken::ttImm)
        }
    );
    EXPECT_EQ(2, size);
}

TEST_F(X86TableSuite, CALLGetOutputSize)
{
    auto iset = decode_json<InstructionSet>(std::string(X86_64_JSON));
    auto inst = iset.instructions().at("CALL");
    auto size = inst.get_output_size(
        ID_16BIT_MODE,
        {
            TParaToken("waitkbdout", TParaToken::ttIdentifier, TParaToken::ttRel32)
        }
    );
    EXPECT_EQ(5, size);
}

TEST_F(X86TableSuite, MOVGetOutputSize)
{
    auto iset = decode_json<InstructionSet>(std::string(X86_64_JSON));
    auto inst = iset.instructions().at("MOV");

    auto size1 = inst.get_output_size(
        ID_16BIT_MODE,
        {
            TParaToken("AL", TParaToken::ttIdentifier, TParaToken::ttReg8),
            TParaToken("SI", TParaToken::ttIdentifier, TParaToken::ttMem8)
        }
    );
    EXPECT_EQ(2, size1);

    auto size2 = inst.get_output_size(
        ID_16BIT_MODE,
        {
            TParaToken("AX", TParaToken::ttIdentifier, TParaToken::ttReg16),
            TParaToken("0", TParaToken::ttIdentifier, TParaToken::ttImm)
        }
    );
    EXPECT_EQ(3, size2);

}
