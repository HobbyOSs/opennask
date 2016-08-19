#include <CppUTest/TestHarness.h>

TEST_GROUP(example_suite)
{
};


TEST(example_suite, testInt)
{
     int i = 3;

     CHECK_EQUAL(3, i);
}

TEST(example_suite, testString)
{
     const char* str = "example";

     STRCMP_EQUAL("example", str);
     STRCMP_NOCASE_EQUAL("EXAMPLE", str);
}

TEST(example_suite, testDouble)
{
     double d = 3.67;

     DOUBLES_EQUAL(3.64, d, 0.1);
}

TEST(example_suite, testPointer)
{
     int i = 3;
     int *p2 = &i;

     POINTERS_EQUAL(&i, p2);
}

//TEST(example_suite, testWillFail)
//{
//     int wrong = 3;
//
//     CHECK_EQUAL(762, wrong); // This will fail!
//}
