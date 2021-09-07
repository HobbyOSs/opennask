#include <cmath>
#include <CppUTest/TestHarness.h>
#include "tinyexpr.h"
#include "driver.hh"

TEST_GROUP(exp_suite)
{
};

TEST(exp_suite, testInt)
{
     int error;

     /* Returns 10. */
     const double a = te_interp("(5+5)", 0);
     CHECK_EQUAL(10, a);

     /* Returns 10, error is set to 0. */
     const double b = te_interp("(5+5)", &error);
     CHECK_EQUAL(10, b);
     CHECK_EQUAL(0, error);

     /* Returns NaN, error is set to 4. */
     const double c = te_interp("(5+5", &error);
     CHECK(std::isnan(c));
     CHECK_EQUAL(4, error);
}

TEST(exp_suite, testNask)
{
     int error;

     const double ecx1 = te_interp("512*1024/4", &error);
     CHECK(!std::isnan(ecx1));
     CHECK_EQUAL(131072, ecx1);

     const double ecx2 = te_interp("512/4", &error);
     CHECK(!std::isnan(ecx2));
     CHECK_EQUAL(128, ecx2);

     const double dw = te_interp("8*3-1", &error);
     CHECK(!std::isnan(dw));
     CHECK_EQUAL(23, dw);
}
