#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "nask_utility.hpp"
#include "mod_rm.hpp"
#include "string_util.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

// Init stuff
std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_st("opennask", "debug.log");

TEST_GROUP(modrmtest_suite)
{
};

TEST(modrmtest_suite, check_32bit_reg_brackets)
{
     using namespace nask_utility;

     CHECK(ModRM::get_rm_from_reg("[EAX]") == "000");
     CHECK(ModRM::get_rm_from_reg("[EAX+]") == "000");
     CHECK(ModRM::get_rm_from_reg("[EAX+4]") == "000");
     CHECK(ModRM::get_rm_from_reg("[EAX+12]") == "000");

     CHECK(ModRM::get_rm_from_reg("[EXX-12]") != "000");
     CHECK(ModRM::get_rm_from_reg("[EAX-12]") != "000");
     CHECK(ModRM::get_rm_from_reg("[EAXx12]") != "000");
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
