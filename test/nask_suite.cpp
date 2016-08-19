#include "nask_utility.hpp"
#include <CppUTest/TestHarness.h>

TEST_GROUP(nask_suite)
{
};


TEST(nask_suite, testInt)
{
     int i = 3;

     CHECK_EQUAL(3, i);
}

TEST(nask_suite, testString)
{
     const char* str = "example";

     STRCMP_EQUAL("example", str);
     STRCMP_NOCASE_EQUAL("EXAMPLE", str);
}

TEST(nask_suite, testDouble)
{
     double d = 3.67;

     DOUBLES_EQUAL(3.64, d, 0.1);
}

TEST(nask_suite, testPointer)
{
     int i = 3;
     int *p2 = &i;

     POINTERS_EQUAL(&i, p2);
}
