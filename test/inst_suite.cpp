#include "nask_utility.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <CppUTest/TestHarness.h>

// Init stuff
std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("opennask", "debug.log");

// "parasol" object can't use smartpointer
TParaCxxTokenTable* token_table = new TParaCxxTokenTable();

TEST_GROUP(inst_suite)
{
     void setup() {
	  token_table->AddCommentLimiter(";", "\n");
	  token_table->AddCommentLimiter("#", "\n");
     }

     void teardown() {
	  delete token_table;
     }
};


//TEST(inst_suite, LGDT)
//{
//     std::istringstream input_stream("	      LGDT    [GDTR0]	      ; 暫定GDTを設定");
//     TParaTokenizer tokenizer(input_stream, token_table);
//     std::vector<uint8_t> binout_container;
//
//     // Found LGDT
//     tokenizer.Next();
//     nask_utility::Instructions inst;
//     inst.process_token_LGDT(tokenizer, binout_container);
//     //CHECK_EQUAL(0x0f, binout_container.at(0));
//}
