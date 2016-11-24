#include "nask_utility.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <CppUTest/TestHarness.h>

// Init stuff
std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("opennask", "debug.log");

TEST_GROUP(inst_suite)
{
};


TEST(inst_suite, MOV_with_bracket)
{
     // Found MOV_with_bracket
     nask_utility::Instructions inst;

     std::istringstream input_stream1("[INSTRSET \"i486p\"] \r\n");
     std::istringstream input_stream2("ECX,[ESP+4]          \r\n");
     TParaTokenizer tokenizer1(input_stream1, &inst.token_table);
     TParaTokenizer tokenizer2(input_stream2, &inst.token_table);

     std::vector<uint8_t> test;
     std::vector<uint8_t> answer = { 0x8b, 0x4e };

     inst.process_token_BRACKET(tokenizer1, test);
     inst.process_token_MOV(tokenizer2, test);

     for ( auto &el : test ) {
	  logger->info("0x{:02x}", el);
     }

     // static member data "support" causes memory leak :<
     EXPECT_N_LEAKS(12);
     CHECK(test == answer);
}
