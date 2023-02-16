#include "spdlog/sinks/basic_file_sink.h"
#include "nask_utility.hpp"
#include "string_util.hpp"
#include "bracket_utility.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

// Init stuff
auto logger = spdlog::basic_logger_st("opennask", "debug.log");

TEST_GROUP(day04test_suite) {};

TEST(day04test_suite, asmhead_MOV_disp) {
    // Found MOV_with_bracket
    //
    // MOV CL,BYTE [0x0ff0]
    //
    // Opecode: 0x8a
    // ModR/M : 0x0e => 00001110
    //
    // src_reg => [0x0ff0]
    // dst_reg => CL
    //
    // [mod]: 00
    // [reg]: 001 <-- dst(CL)
    // [r/m]: 110 <-- src(0x0ff0)
    //
    nask_utility::Instructions inst;
    std::array<std::string, 2> naskfunc_src = {
        "CYLS	EQU 0x0ff0               \r\n", //  0
        "MOV  CL,BYTE [CYLS]           \r\n"	//  1
    };

    std::vector<uint8_t> test; // output

    for (size_t l = 0; l < naskfunc_src.size(); l++) {
        std::istringstream input_stream(naskfunc_src.at(l));
        TParaTokenizer tokenizer(input_stream, inst.token_table);

        switch (l) {
        case 0:
            inst.process_token_EQU(tokenizer, test);
            break;
        case 1:
            tokenizer.Next();
            inst.process_token_MOV(tokenizer, test);
            break;
        default:
            break;
        };
    }

    std::vector<uint8_t> answer = { 0x8a, 0x0e, 0xf0, 0x0f };
    if (test != answer) {
        logger->error("output bin: {}",
                      nask_utility::string_to_hex(std::string(test.begin(), test.end())));
    }

    CHECK(test == answer);
}

TEST(day04test_suite, asmhead_MOV_mem_former) {
    // Found MOV_with_bracket
    //
    // [INSTRSET "i486p"]
    // MOV [EDI],EAX
    //
    // Prefix : 0x67 0x66
    // Opecode: 0x89
    // ModR/M : 0x07
    //
    nask_utility::Instructions inst;
    std::array<std::string, 2> naskfunc_src = {
        "[INSTRSET \"i486p\"]	  \r\n", // 0
        "MOV [EDI],EAX	  \r\n"	 // 1
    };

    std::vector<uint8_t> test; // output

    for (size_t l = 0; l < naskfunc_src.size(); l++) {
        std::istringstream input_stream(naskfunc_src.at(l));
        TParaTokenizer tokenizer(input_stream, inst.token_table);

        switch (l) {
        case 0:
            inst.process_token_BRACKET(tokenizer, test);
            break;
        case 1:
            tokenizer.Next();
            inst.process_token_MOV(tokenizer, test);
            break;
        default:
            break;
        };
    }

    std::vector<uint8_t> answer = { 0x67, 0x66, 0x89, 0x07 };
    if (test != answer) {
        logger->error("output bin: {}",
                      nask_utility::string_to_hex(std::string(test.begin(), test.end())));
    }

    CHECK(test == answer);
}

TEST(day04test_suite, asmhead_MOV_disp_plus) {
    // Found MOV_with_bracket
    //
    // [INSTRSET "i486p"]
    // MOV ESI,[EBX+20]
    // MOV EDI,[EBX+12]
    // ---
    // 1) MOV ESI,[EBX+20]
    // Prefix : 0x67 0x66
    // Opecode: 0x8b
    // ModR/M : 0x73
    //
    // [mod]: 01
    // [reg]: 110
    // [r/m]: 011
    // ---
    // 2) MOV EDI,[EBX+12]
    // Prefix : 0x67 0x66
    // Opecode: 0x8b
    // ModR/M : 0x7b
    //
    // [mod]: 01
    // [reg]: 111
    // [r/m]: 011
    //
    nask_utility::Instructions inst;
    std::array<std::string, 3> naskfunc_src = {
        "[INSTRSET \"i486p\"]	  \r\n", // 0
        "MOV ESI,[EBX+20]	  \r\n", // 1
        "MOV EDI,[EBX+12]	  \r\n"	 // 2
    };

    std::vector<uint8_t> test; // output

    for (size_t l = 0; l < naskfunc_src.size(); l++) {
        std::istringstream input_stream(naskfunc_src.at(l));
        TParaTokenizer tokenizer(input_stream, inst.token_table);

        switch (l) {
        case 0:
            inst.process_token_BRACKET(tokenizer, test);
            break;
        case 1:
        case 2:
            tokenizer.Next();
            inst.process_token_MOV(tokenizer, test);
            break;
        default:
            break;
        };
    }

    std::vector<uint8_t> answer = {
        0x67, 0x66, 0x8b, 0x73, 0x14,
        0x67, 0x66, 0x8b, 0x7b, 0x0c
    };

    if (test != answer) {
        logger->error("output bin: {}",
                      nask_utility::string_to_hex(std::string(test.begin(), test.end())));
    }

    CHECK(test == answer);
}

TEST(day04test_suite, asmhead_MOV_later) {
    // Found MOV_with_bracket
    //
    // [INSTRSET "i486p"]
    // MOV EAX,[ESI]
    //
    // Prefix : 0x67 0x66
    // Opecode: 0x8b
    // ModR/M : 0x06
    //
    nask_utility::Instructions inst;
    inst.OPENNASK_MODES = ID_16BIT_MODE;

    std::array<std::string, 2> naskfunc_src = {
        "[INSTRSET \"i486p\"]	  \r\n", // 0
        "MOV EAX,[ESI]	  \r\n"	 // 1
    };
    std::vector<uint8_t> test; // output

    for (size_t l = 0; l < naskfunc_src.size(); l++) {
        std::istringstream input_stream(naskfunc_src.at(l));
        TParaTokenizer tokenizer(input_stream, inst.token_table);

        switch (l) {
        case 0:
            inst.process_token_BRACKET(tokenizer, test);
            break;
        case 1:
            tokenizer.Next();
            inst.process_token_MOV(tokenizer, test);
            break;
        default:
            break;
        };
    }

    std::vector<uint8_t> answer = { 0x67, 0x66, 0x8b, 0x06 };

    if (test != answer) {
        logger->error("output bin: {}",
                      nask_utility::string_to_hex(std::string(test.begin(), test.end())));
    }
    CHECK(test == answer);
}

TEST(day04test_suite, day04) {
    // Found MOV_with_bracket
    nask_utility::Instructions inst;
    inst.OPENNASK_MODES = ID_16BIT_MODE;
    std::array<std::string, 17> naskfunc_src = {
        "[FORMAT \"WCOFF\"]            \r\n", //  0
        "[INSTRSET \"i486p\"]          \r\n",	//  1
        "[BITS 32]                     \r\n",	//  2
        "[FILE \"naskfunc.nas\"]       \r\n",	//  3
        "                              \r\n",	//  4
        "  GLOBAL _io_hlt,_write_mem8  \r\n",	//  5
        "                              \r\n",	//  6
        "[SECTION .text]               \r\n",	//  7
        "                              \r\n",	//  8
        "_io_hlt:                      \r\n",	//  9
        "  HLT                         \r\n",	// 10
        "  RET			 \r\n",	// 11
        "_write_mem8:                  \r\n",	// 12
        "  MOV ECX,[ESP+4] 		 \r\n",	// 13
        "  MOV AL,[ESP+8]		 \r\n",	// 14
        "  MOV [ECX],AL		 \r\n",	// 15
        "  RET 			 \r\n"	// 16
    };

    std::vector<uint8_t> test; // output

    for (size_t l = 0; l < naskfunc_src.size(); l++) {
        std::istringstream input_stream(naskfunc_src.at(l));
        TParaTokenizer tokenizer(input_stream, inst.token_table);

        switch (l) {
        case 0:
        case 1:
        case 2:
        case 3:
            inst.process_token_BRACKET(tokenizer, test);
            break;
        case 5:
            //tokenizer.Next();
            inst.process_token_GLOBAL(tokenizer, test);
            break;
        case 10:
            tokenizer.Next();
            inst.process_token_HLT(tokenizer, test);
            break;
        case 11:
        case 16:
            tokenizer.Next();
            inst.process_token_RET(tokenizer, test);
            break;
        case 13:
        case 14:
        case 15:
            tokenizer.Next();
            inst.process_token_MOV(tokenizer, test);
            break;
        default:
            break;
        };
    }

    // sectionテーブルを追加する
    if (inst.exists_section_table) {
        nask_utility::process_section_table(inst, test);
    }

    std::vector<uint8_t> answer = {
        0x4C, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x8C, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x10, 0x60, 0x2E, 0x64, 0x61, 0x74,
        0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x40, 0x00, 0x10, 0xC0, 0x2E, 0x62, 0x73, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x10, 0xC0, 0xF4, 0xC3, 0x8B, 0x4C,
        0x24, 0x04, 0x8A, 0x44, 0x24, 0x08, 0x88, 0x01, 0xC3, 0x2E, 0x66, 0x69, 0x6C, 0x65, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x67, 0x01, 0x6E, 0x61, 0x73, 0x6B, 0x66,
        0x75, 0x6E, 0x63, 0x2E, 0x6E, 0x61, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 0x74, 0x65,
        0x78, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x01, 0x0D,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x2E, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
        0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 0x62, 0x73, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x68, 0x6C, 0x74,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
        //                                        0x01 <-- 本来の環境では 0x01
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00,
        0x00, 0x5F, 0x77, 0x72, 0x69, 0x74, 0x65, 0x5F, 0x6D, 0x65, 0x6D, 0x38, 0x00
    };

    if (test != answer && test.size() == answer.size()) {
        //logger->error("output bin: {}", nask_utility::string_to_hex(std::string(test.begin(), test.end())));

        for (size_t i = 0; i < answer.size(); i=i+16) {
            std::array<uint8_t, 16> expect = {};
            std::array<uint8_t, 16> actual = {};
            std::copy(answer.begin() +i, answer.begin() +i +16, expect.begin());
            std::copy(test.begin()   +i, test.begin()   +i +16, actual.begin());

            const std::string hex = nask_utility::string_to_hex(std::string(expect.begin(), expect.end()));
            logger->error("OK {}", hex);

            if (expect != actual) {
                const std::string hex = nask_utility::string_to_hex(std::string(actual.begin(), actual.end()));
                logger->error("NG {}", hex);
            }
        }
    }

    CHECK(test == answer);
}

void setup() {
    //MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
}

void teardown() {
    //MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
}

int main(int argc, char** argv) {
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
