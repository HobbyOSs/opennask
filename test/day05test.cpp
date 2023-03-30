#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "nask_utility.hpp"
#include "string_util.hpp"
#include "bracket_utility.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <gtest/gtest.h>

class Day05Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Day05Suite() {
        // TODO: nask_parseに切り替わったら削除
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // 試験終了時に一回だけ実行
    ~Day05Suite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(Day05Suite, AsmheadMovMemFormerDisp) {
    // Found MOV_with_bracket
    //
    // MOV  AX,[ESP+4]
    // MOV  [ESP+6],AX
    // LGDT [ESP+6]
    // RET
    //
    // MOV  AX,[ESP+4]
    // MOV  [ESP+6],AX
    // LIDT [ESP+6]
    // RET
    //
    nask_utility::Instructions inst;
    std::array<std::string, 10> naskfunc_src = {
        "[INSTRSET \"i486p\"]	  \r\n", // 0
        "[BITS 32]	          \r\n", // 1
        "MOV  AX,[ESP+4]        \r\n", // 2
        "MOV  [ESP+6],AX	  \r\n", // 3
        "LGDT [ESP+6]		  \r\n", // 4
        "RET			  \r\n", // 5
        "MOV  AX,[ESP+4]	  \r\n", // 6
        "MOV  [ESP+6],AX	  \r\n", // 7
        "LIDT [ESP+6]		  \r\n", // 8
        "RET	  		  \r\n"  // 9
    };

    std::vector<uint8_t> answer = {
        0x66, 0x8B, 0x44, 0x24, 0x04, // 0x66 0x8B 0x44 0x24 0x04
        0x66, 0x89, 0x44, 0x24, 0x06,	// 0x66 0x89 0x44 0x24 0x06
        0x0f, 0x01, 0x54, 0x24, 0x06,	// 0x0F 0x01 0x54 0x24 0x06
        0xc3,				// 0xC3
        0x66, 0x8b, 0x44, 0x24, 0x04,	// 0x66 0x8B 0x44 0x24 0x04
        0x66, 0x89, 0x44, 0x24, 0x06,	// 0x66 0x89 0x44 0x24 0x06
        0x0f, 0x01, 0x5c, 0x24, 0x06,	//
        0xc3				// 0xc3
    };
    std::vector<uint8_t> test; // output

    for (size_t l = 0; l < naskfunc_src.size(); l++) {
        std::istringstream input_stream(naskfunc_src.at(l));
        TParaTokenizer tokenizer(input_stream, inst.token_table);

        switch (l) {
        case 0:
        case 1:
            inst.process_token_BRACKET(tokenizer, test);
            break;
        case 5:
        case 9:
            tokenizer.Next();
            inst.process_token_RET(tokenizer, test);
            break;
        case 4:
            tokenizer.Next();
            inst.process_token_LGDT(tokenizer, test);
            break;
        case 8:
            tokenizer.Next();
            inst.process_token_LIDT(tokenizer, test);
            break;
        case 2:
        case 3:
        case 6:
        case 7:
            tokenizer.Next();
            inst.process_token_MOV(tokenizer, test);
            break;
        default:
            break;
        };
    }

    if (test != answer) {
        spdlog::get("opennask")->error("output bin: {}",
                                       nask_utility::string_to_hex(std::string(test.begin(), test.end())));
    }
    EXPECT_TRUE(test == answer);
}
