#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "x86.hh"
#include <gtest/gtest.h>

class TParaTokenSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    TParaTokenSuite() {
    }

    // 試験終了時に一回だけ実行
    ~TParaTokenSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

// テストデータクラス
struct TokenAsInt32Param {
    const std::string token;
    const TParaToken para_token;
    int32_t expected;

    TokenAsInt32Param(
        const std::string& token,
        const TParaToken::TTokenType ttype,
        const TParaToken::TIdentiferAttribute attr,
        int32_t expected
    ):
        para_token(TParaToken(token, ttype, attr)),
        expected(expected)
    {
    }
};

void PrintTo(const TokenAsInt32Param& param, ::std::ostream* os) {
    *os << param.para_token.to_string();
}

class TokenAsInt32 : public testing::TestWithParam<TokenAsInt32Param> {};

TEST_P(TokenAsInt32, TokenAsInt32) {
    const auto p = GetParam();

    EXPECT_EQ(p.para_token.AsInt32(), p.expected);
}

INSTANTIATE_TEST_SUITE_P(TParaTokenSuite, TokenAsInt32,
    testing::Values(
        // hex
        TokenAsInt32Param("0x80", TParaToken::ttHex, TParaToken::ttImm, -128),
        TokenAsInt32Param("0xE2", TParaToken::ttHex, TParaToken::ttImm, -30),
        TokenAsInt32Param("0xC200", TParaToken::ttHex, TParaToken::ttImm, -15872),
        TokenAsInt32Param("0x7C00", TParaToken::ttHex, TParaToken::ttImm, 31744),
        TokenAsInt32Param("0x7FFFFFFF", TParaToken::ttHex, TParaToken::ttImm, 2147483647),
        TokenAsInt32Param("0xffffffff", TParaToken::ttHex, TParaToken::ttImm, -1),
        TokenAsInt32Param("0xFFFFFFFF", TParaToken::ttHex, TParaToken::ttImm, -1),
        // numeric
        TokenAsInt32Param("-128", TParaToken::ttInteger, TParaToken::ttImm, -128),
        TokenAsInt32Param("-30", TParaToken::ttInteger, TParaToken::ttImm, -30),
        TokenAsInt32Param("-15872", TParaToken::ttInteger, TParaToken::ttImm, -15872),
        TokenAsInt32Param("31744", TParaToken::ttInteger, TParaToken::ttImm, 31744),
        TokenAsInt32Param("2147483647", TParaToken::ttInteger, TParaToken::ttImm, 2147483647)
    )
);
