#include "nask_utility.hpp"
#include "string_util.hpp"
#include <CppUTest/TestHarness.h>

TEST_GROUP(nask_suite)
{
};


TEST(nask_suite, testNaskUtilityContains)
{
     const std::string line = "		MOV		AX,1*8			;  読み書き可能セグメント32bit";
     const bool a = nask_utility::contains(line, "*");
     CHECK(a);
}

TEST(nask_suite, testNaskUtilityIsContainsMathOp)
{
     const std::string line1 = "		MOV		AX,1*8	;  読み書き可能セグメント32bit";
     const bool a = nask_utility::is_contains_math_op(line1);
     CHECK(a);

     const std::string line2 = "		MOV		ECX,512*1024/4";
     const bool b = nask_utility::is_contains_math_op(line2);
     CHECK(b);

     const std::string line3 = "		DW		83-1";
     const bool c = nask_utility::is_contains_math_op(line3);
     CHECK(c);

     const std::string line4 = "		DW		83+1";
     const bool d = nask_utility::is_contains_math_op(line4);
     CHECK(d);

     const std::string line5 = "		DW		83";
     const bool e = nask_utility::is_contains_math_op(line5);
     CHECK(!e);
}

TEST(nask_suite, testNaskUtilityExprMathOp)
{
     const std::string line1 = "		MOV		AX,1*8";
     const std::string a = nask_utility::expr_math_op(line1);
     CHECK_EQUAL("		MOV		AX,8", a);

     const std::string line2 = "		MOV		ECX,512*1024/4";
     const std::string b = nask_utility::expr_math_op(line2);
     CHECK_EQUAL("		MOV		ECX,131072", b);
}

TEST(nask_suite, testNaskUtilityIsImm8)
{
     const bool will_true1 = nask_utility::is_imm8("0xff");
     CHECK(will_true1);
     const bool will_true2 = nask_utility::is_imm8("128");
     CHECK(will_true2);
     const bool will_false1 = nask_utility::is_imm8("0x80ff");
     CHECK(!will_false1);
     const bool will_false2 = nask_utility::is_imm8("-500");
     CHECK(!will_false2);
     const bool will_false3 = nask_utility::is_imm8("258");
     CHECK(!will_false3);
}
