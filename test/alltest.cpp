#include <vector>
#include <CppUTest/CommandLineTestRunner.h>

int main(int argc, char** argv)
{
     std::vector<const char*> args(argv, argv + argc); // Insert all arguments
     args.push_back("-v"); // Set verbose mode
     args.push_back("-c"); // Set color output (OPTIONAL)

     // Run all tests
     int i = RUN_ALL_TESTS(args.size(), &args[0]);
     return i;
}
