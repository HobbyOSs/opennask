#include "front_end.hh"
#include "pass1_strategy.hh"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <gtest/gtest.h>

class Pass1Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Pass1Suite() {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // 試験終了時に一回だけ実行
    ~Pass1Suite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(Pass1Suite, SymTable) {

    std::stringstream ss;
    const char nask_statements[] = R"(
		ORG		0x0100
		;LDA		#5

LOOP:
		;SUB		#1
		;COMP		#0
		;JGT		LOOP
		;RSUB		#1
)";

    //ss << nask_statements;
    //auto d = std::make_unique<FrontEnd>(true, true);
    //auto pt = d->Parse<Program>(ss);
    //auto pass1 = std::make_unique<Pass1Strategy>();
    //pass1->Eval(pt.get());

    //EXPECT_EQ(1, pass1->sym_table.count("LOOP"));
    //EXPECT_EQ(259, pass1->sym_table.at("LOOP"));
}
