#include "driver.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

// Init stuff
auto logger = spdlog::basic_logger_mt("opennask", "debug.log");

TEST_GROUP(day01_suite)
{

};

TEST(day01_suite, helloos) {

    const char* nask_statements = R"(	DB	0xeb;, 0x4e, 0x90, 0x48, 0x45, 0x4c, 0x4c, 0x4f
	;DB	0x49, 0x50, 0x4c, 0x00, 0x02, 0x01, 0x01, 0x00
	;DB	0x02, 0xe0, 0x00, 0x40, 0x0b, 0xf0, 0x09, 0x00
	;DB	0x12, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
	;DB	0x40, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x29, 0xff
	;DB	0xff, 0xff, 0xff, 0x48, 0x45, 0x4c, 0x4c, 0x4f
	;DB	0x2d, 0x4f, 0x53, 0x20, 0x20, 0x20, 0x46, 0x41
	;DB	0x54, 0x31, 0x32, 0x20, 0x20, 0x20, 0x00, 0x00
	;RESB	16
	;DB	0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
	;DB	0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
	;DB	0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
	;DB	0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
	;DB	0xee, 0xf4, 0xeb, 0xfd, 0x0a, 0x0a, 0x68, 0x65
	;DB	0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72
	;DB	0x6c, 0x64, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00
	;RESB	368
	;DB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
	;DB	0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	;RESB	4600
	;DB	0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	;RESB	1469432
)";

    std::unique_ptr<Driver> d(new Driver(false, false));
    d->Parse<Program>(nask_statements, "test.img");

    //std::vector<uint8_t> answer = {
  //};
  //std::vector<uint8_t> test; // output
  //CHECK(test == answer);
}

int main(int argc, char** argv) {
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
