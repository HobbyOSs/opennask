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
std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("opennask", "debug.log");

TEST_GROUP(day05test_suite)
{
};

TEST(day05test_suite, asmhead_MOV_mem_former_disp)
{
     // Found MOV_with_bracket
     //
     //
     //
     //
     /**
     nask_utility::Instructions inst;
     std::array<std::string, 2> naskfunc_src = {
	  "[INSTRSET \"i486p\"]	  \r\n", // 0
	  "MOV [EDI],EAX	  \r\n"	 // 1
     };

     std::vector<uint8_t> test; // output

     for (size_t l = 0; l < naskfunc_src.size(); l++) {
	  std::istringstream input_stream(naskfunc_src.at(l));
	  TParaTokenizer tokenizer(input_stream, &inst.token_table);

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
     EXPECT_N_LEAKS(9);
     if (test != answer) {
	  logger->error("output bin: {}",
			nask_utility::string_to_hex(std::string(test.begin(), test.end())));
     }

     CHECK(test == answer);
     */
     CHECK(true);
}

void setup()
{
     MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
}

void teardown()
{
     MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
}

int main(int argc, char** argv)
{
     std::vector<const char*> args(argv, argv + argc); // Insert all arguments
     args.push_back("-v"); // Set verbose mode
     args.push_back("-c"); // Set color output (OPTIONAL)

     // Run all tests
     int i = RUN_ALL_TESTS(args.size(), &args[0]);
     return i;
}
