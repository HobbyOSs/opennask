#include "front_end.hh"
#include "pass1_strategy.hh"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <gtest/gtest.h>

class Pass1Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Pass1Suite() {
        //spdlog::set_level(spdlog::level::debug);
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

TEST_F(Pass1Suite, Helloos3) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; シンボルテーブルへのラベルの設定の結合テスト
		ORG		0x7c00

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"
		DW		512
		DB		1
		DW		1
		DB		2
		DW		224
		DW		2880
		DB		0xf0
		DW		9
		DW		18
		DW		2
		DD		0
		DD		2880
		DB		0,0,0x29
		DD		0xffffffff
		DB		"HELLO-OS   "
		DB		"FAT12   "
		RESB	18

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		;MOV		SS,AX
		;MOV		SP,0x7c00
		;MOV		DS,AX
		;MOV		ES,AX

		;MOV		SI,msg

;putloop:
		;MOV		AL,[SI]
)";

    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    auto pass1 = std::make_unique<Pass1Strategy>();
    pass1->Eval(pt.get());

    EXPECT_EQ(31824, pass1->sym_table["entry"]);
    //EXPECT_EQ(31839, pass1->sym_table["putloop"]);
    EXPECT_EQ(31827, pass1->loc);
}

TEST_F(Pass1Suite, SymTable) {

    GTEST_SKIP(); // TODO: まだ機能しない

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

    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    auto pass1 = std::make_unique<Pass1Strategy>();
    pass1->Eval(pt.get());

    EXPECT_EQ(1, pass1->sym_table.count("LOOP"));
    EXPECT_EQ(259, pass1->sym_table.at("LOOP"));
}
