#include <cmath>
#include <CppUTest/TestHarness.h>
#include "tinyexpr.h"

TEST_GROUP(tinyexpr_suite)
{
};

TEST(tinyexpr_suite, testInt)
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
