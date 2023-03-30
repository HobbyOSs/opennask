#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "nask_utility.hpp"
#include "mod_rm.hpp"
#include "string_util.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <gtest/gtest.h>

class ModRMSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    ModRMSuite() {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // 試験終了時に一回だけ実行
    ~ModRMSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(ModRMSuite, Check32bitRegBrackets)
{
     using namespace nask_utility;

     EXPECT_TRUE(ModRM::get_rm_from_reg("[EAX]") == "000");
     EXPECT_TRUE(ModRM::get_rm_from_reg("[EAX+]") == "000");
     EXPECT_TRUE(ModRM::get_rm_from_reg("[EAX+4]") == "000");
     EXPECT_TRUE(ModRM::get_rm_from_reg("[EAX+12]") == "000");

     EXPECT_TRUE(ModRM::get_rm_from_reg("[EXX-12]") != "000");
     EXPECT_TRUE(ModRM::get_rm_from_reg("[EAX-12]") != "000");
     EXPECT_TRUE(ModRM::get_rm_from_reg("[EAXx12]") != "000");
}
