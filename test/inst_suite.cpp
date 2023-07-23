#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "nask_utility.hpp"
#include "string_util.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <gtest/gtest.h>
#include "front_end.hh"
#include "diff.hh"

class InstSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    InstSuite() {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // 試験終了時に一回だけ実行
    ~InstSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

// テストデータクラス
struct StatementToMachineCodeParam {
    const std::string _statement;
    const std::vector<uint8_t> _expected;

    StatementToMachineCodeParam(
        const std::string& statement,
        const std::vector<uint8_t> expected
    ): _statement(statement),
       _expected(expected)
    {
    }
};

void PrintTo(const StatementToMachineCodeParam& param, ::std::ostream* os) {
    *os << param._statement;
}

class StatementToMachineCode : public testing::TestWithParam<StatementToMachineCodeParam> {};

TEST_P(StatementToMachineCode, StatementToMachineCode) {
    const auto p = GetParam();

    std::stringstream ss;
    ss << p._statement;

    auto front = std::make_unique<FrontEnd>(true, true);
    auto pt = front->Parse<Program>(ss);
    front->Eval<Program>(pt.get(), "test.img");

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF, p._expected, front->binout_container);
}

INSTANTIATE_TEST_SUITE_P(InstSuite, StatementToMachineCode,
    testing::Values(
        // DB,DW,DD---
        StatementToMachineCodeParam("DB 1,2,3", std::vector<uint8_t>{0x01, 0x02, 0x03}),
        StatementToMachineCodeParam("DB \"Hello\"", std::vector<uint8_t>{0x48,0x65,0x6c,0x6c,0x6f}),
        StatementToMachineCodeParam("DW 512", std::vector<uint8_t>{0x00, 0x02}),
        StatementToMachineCodeParam("DD 0xffffffff", std::vector<uint8_t>{0xff, 0xff, 0xff, 0xff}),
        // CLI,HLT,NOP
        StatementToMachineCodeParam("CLI", std::vector<uint8_t>{0xfa}),
        StatementToMachineCodeParam("HLT", std::vector<uint8_t>{0xf4}),
        StatementToMachineCodeParam("NOP", std::vector<uint8_t>{0x90}),
        // ADD---
        StatementToMachineCodeParam("ADD AL,1", std::vector<uint8_t>{0x04, 0x01}),
        StatementToMachineCodeParam("ADD AX,0x0020", std::vector<uint8_t>{0x05, 0x20, 0x00}),
        StatementToMachineCodeParam("ADD EAX,3", std::vector<uint8_t>{0x83, 0xc0, 0x03}),
        StatementToMachineCodeParam("ADD BL,1", std::vector<uint8_t>{0x80, 0xc3, 0x01}),
        StatementToMachineCodeParam("ADD BH,1", std::vector<uint8_t>{0x80, 0xc7, 0x01}),
        StatementToMachineCodeParam("ADD SI,300", std::vector<uint8_t>{0x81, 0xc6, 0x2c, 0x01}),
        StatementToMachineCodeParam("ADD SI,1", std::vector<uint8_t>{0x83, 0xc6, 0x01}),
        StatementToMachineCodeParam("ADD EBX,1", std::vector<uint8_t>{0x83, 0xc3, 0x01}),
        StatementToMachineCodeParam("ADD EBX,300", std::vector<uint8_t>{0x81, 0xc3, 0x2c, 0x01, 0x00, 0x00}),
        // CMP---
        StatementToMachineCodeParam("CMP AL,0x01", std::vector<uint8_t>{0x3c, 0x01}),
        StatementToMachineCodeParam("CMP AX,0x2000", std::vector<uint8_t>{0x3d, 0x00, 0x20 }),
        StatementToMachineCodeParam("CMP EAX,0x3000", std::vector<uint8_t>{0x3d, 0x00, 0x30, 0x00, 0x00}),
        StatementToMachineCodeParam("CMP BL,1", std::vector<uint8_t>{0x80, 0xfb, 0x01}),
        StatementToMachineCodeParam("CMP BH,1", std::vector<uint8_t>{0x80, 0xff, 0x01}),
        StatementToMachineCodeParam("CMP SI,300", std::vector<uint8_t>{0x81, 0xfe, 0x2c, 0x01}),
        StatementToMachineCodeParam("CMP SI,1", std::vector<uint8_t>{0x83, 0xfe, 0x01}),
        StatementToMachineCodeParam("CMP EBX,1", std::vector<uint8_t>{0x83, 0xfb, 0x01}),
        StatementToMachineCodeParam("CMP EBX,300", std::vector<uint8_t>{0x81, 0xfb, 0x2c, 0x01, 0x00, 0x00}),

        // OUT---
        StatementToMachineCodeParam("OUT 0x01,AL", std::vector<uint8_t>{0xe6, 0x01}),
        StatementToMachineCodeParam("OUT 0x01,AX", std::vector<uint8_t>{0xe7, 0x01}),
        StatementToMachineCodeParam("OUT 0x01,EAX", std::vector<uint8_t>{0xe7, 0x01}),
        StatementToMachineCodeParam("OUT DX,AL", std::vector<uint8_t>{0xee}),
        StatementToMachineCodeParam("OUT DX,AX", std::vector<uint8_t>{0xef}),
        StatementToMachineCodeParam("OUT DX,EAX", std::vector<uint8_t>{0xef}),

        // MOV---
        StatementToMachineCodeParam("MOV AL,0x0e", std::vector<uint8_t>{0xb0, 0x0e}),
        StatementToMachineCodeParam("MOV AH,0x0e", std::vector<uint8_t>{0xb4, 0x0e})


    )
);

/**
TEST_F(InstSuite, MovWithBracket)
{
     // Found MOV_with_bracket
     nask_utility::Instructions inst;

     std::istringstream input_stream0("[BITS 32]            \r\n");
     std::istringstream input_stream1("[INSTRSET \"i486p\"] \r\n");
     std::istringstream input_stream2("ECX,[ESP+4]          \r\n");
     std::istringstream input_stream3("AL,[ESP+8]           \r\n");
     TParaTokenizer tokenizer0(input_stream0, &inst.token_table);
     TParaTokenizer tokenizer1(input_stream1, &inst.token_table);
     TParaTokenizer tokenizer2(input_stream2, &inst.token_table);
     TParaTokenizer tokenizer3(input_stream3, &inst.token_table);

     std::vector<uint8_t> test;
     std::vector<uint8_t> answer = { 0x8b, 0x4c, 0x24, 0x04, 0x8a, 0x44, 0x24, 0x08 };

     inst.process_token_BRACKET(tokenizer0, test);
     inst.process_token_BRACKET(tokenizer1, test);
     inst.process_token_MOV(tokenizer2, test);
     inst.process_token_MOV(tokenizer3, test);

     logger->info("output bin: {}", nask_utility::string_to_hex(std::string(test.begin(), test.end())));

     // static member data "support" causes memory leak :<
     //EXPECT_N_LEAKS(12);
     EXPECT_TRUE(test == answer);
}
*/
