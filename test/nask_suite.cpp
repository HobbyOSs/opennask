#include "nask_utility.hpp"
#include "string_util.hpp"
#include <gtest/gtest.h>

class NaskSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    NaskSuite() {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // 試験終了時に一回だけ実行
    ~NaskSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(NaskSuite, NaskUtilityContains)
{
     const std::string line = "		MOV		AX,1*8			;  読み書き可能セグメント32bit";
     const bool a = nask_utility::contains(line, "*");
     EXPECT_TRUE(a);
}

TEST_F(NaskSuite, NaskUtilityIsContainsMathOp)
{
     const std::string line1 = "		MOV		AX,1*8	;  読み書き可能セグメント32bit";
     const bool a = nask_utility::is_contains_math_op(line1);
     EXPECT_TRUE(a);

     const std::string line2 = "		MOV		ECX,512*1024/4";
     const bool b = nask_utility::is_contains_math_op(line2);
     EXPECT_TRUE(b);

     const std::string line3 = "		DW		83-1";
     const bool c = nask_utility::is_contains_math_op(line3);
     EXPECT_TRUE(c);

     const std::string line4 = "		DW		83+1";
     const bool d = nask_utility::is_contains_math_op(line4);
     EXPECT_TRUE(d);

     const std::string line5 = "		DW		83";
     const bool e = nask_utility::is_contains_math_op(line5);
     EXPECT_TRUE(!e);
}

TEST_F(NaskSuite, NaskUtilityExprMathOp)
{
     const std::string line1 = "		MOV		AX,1*8";
     const std::string a = nask_utility::expr_math_op(line1);
     EXPECT_EQ("		MOV		AX,8", a);

     const std::string line2 = "		MOV		ECX,512*1024/4";
     const std::string b = nask_utility::expr_math_op(line2);
     EXPECT_EQ("		MOV		ECX,131072", b);

}

TEST_F(NaskSuite, NaskUtilityReplaceHex2Dec)
{
     const std::string line1 = "		MOV		BX,0x105+0x4000";
     const std::string a = nask_utility::replace_hex2dec(line1);
     EXPECT_EQ("		MOV		BX,261+16384", a);
}

TEST_F(NaskSuite, NaskUtilityIsImm8)
{
     const bool will_true1 = nask_utility::is_imm8("0xff");
     EXPECT_TRUE(will_true1);
     const bool will_false0 = nask_utility::is_imm8("128");
     EXPECT_TRUE(!will_false0);
     const bool will_false1 = nask_utility::is_imm8("0x80ff");
     EXPECT_TRUE(!will_false1);
     const bool will_false2 = nask_utility::is_imm8("-500");
     EXPECT_TRUE(!will_false2);
     const bool will_false3 = nask_utility::is_imm8("258");
     EXPECT_TRUE(!will_false3);
}
