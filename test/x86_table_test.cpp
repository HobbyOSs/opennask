#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "x86.hh"
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

// Init stuff
auto logger = spdlog::stdout_color_st("opennask");

TEST_GROUP(x86_table_test_suite)
{
};

TEST(x86_table_test_suite, check_json_schema)
{

    using namespace x86_64;

    try {
        InstructionSet iset = \
            jsoncons::decode_json<InstructionSet>(std::string(X86_64_JSON));

        CHECK_EQUAL("x86-64", iset.instruction_set());
        CHECK_EQUAL(1215, iset.instructions().size());

        CHECK_EQUAL(1, iset.instructions().count("ADC"));
        CHECK_EQUAL("Add with Carry", iset.instructions().at("ADC").summary());

    } catch(const jsoncons::conv_error& e) {
        std::cout << "Caught conv_error with category "
                  << e.code().category().name()
                  << ", code " << e.code().value()
                  << " and message " << e.what()
                  << " and line " << e.line()
                  << std::endl;
    }
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
