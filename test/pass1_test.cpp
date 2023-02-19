#include "front_end.hh"
#include "pass1_strategy.hh"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <CppUTest/MemoryLeakDetectorNewMacros.h>
#include <CppUTest/MemoryLeakDetectorMallocMacros.h>
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

auto logger = spdlog::stdout_color_st("opennask");

TEST_GROUP(pass1_suite)
{

};

TEST(pass1_suite, sym_table_1) {

    std::stringstream ss;
    const char nask_statements[] = R"(
		ORG		0x0100
		;LDA		#5

LOOP:
		;SUB		#1
		;COMP		#0
		;JGT		LOOP
		;RSUB		#1
)";

    //ss << nask_statements;
    //auto d = std::make_unique<FrontEnd>(true, true);
    //auto pt = d->Parse<Program>(ss);
    //auto pass1 = std::make_unique<Pass1Strategy>();
    //pass1->Eval(pt.get());

    //CHECK_EQUAL(1, pass1->sym_table.count("LOOP"));
    //CHECK_EQUAL(259, pass1->sym_table.at("LOOP"));
}


int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)
    //args.push_back("TEST(pass1_suite, xxx)");

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
