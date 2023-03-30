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
