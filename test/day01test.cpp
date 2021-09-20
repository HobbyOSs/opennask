#include "driver.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

auto logger = spdlog::stdout_logger_mt("opennask", "console");

TEST_GROUP(day01_suite)
{

};

TEST(day01_suite, helloos) {

    char nask_statements[] = R"(
DB	0xeb
)";

    std::unique_ptr<Driver> d(new Driver(true, true));
    FILE* stream = fmemopen(nask_statements, strlen(nask_statements), "r");
    d->Parse<Program>(stream, "test.img");
    fclose(stream);

    std::vector<uint8_t> expected = {
        0xeb
	};
    CHECK_EQUAL(expected.size(), d->binout_container.size());
    CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));

}

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
