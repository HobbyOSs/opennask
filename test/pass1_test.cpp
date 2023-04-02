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
		MOV		AX,0
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		ES,AX

		MOV		SI,msg

putloop:
		MOV		AL,[SI]
		ADD		SI,1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e
		MOV		BX,15
		INT		0x10
		JMP		putloop

fin:
		HLT
		JMP		fin

msg:

)";

    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    auto pass1 = std::make_unique<Pass1Strategy>();
    pass1->Eval(pt.get());

    EXPECT_EQ(31824, pass1->sym_table["entry"]);
    EXPECT_EQ(31839, pass1->sym_table["putloop"]);
    EXPECT_EQ(31857, pass1->sym_table["fin"]);
    EXPECT_EQ(31860, pass1->sym_table["msg"]);
    EXPECT_EQ(31860, pass1->loc);
}

TEST_F(Pass1Suite, Harib00g) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; シンボルテーブルへのラベルの設定の結合テスト
CYLS	EQU		10
		ORG		0x7c00

		JMP	entry
		DB		0x90
		DB		"HARIBOTE"
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
		DB		"HARIBOTEOS "
		DB		"FAT12   "
		RESB	18

entry:
		MOV		AX,0
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0
		MOV		DH,0
		MOV		CL,2
readloop:
		MOV		SI,0
retry:
		MOV		AH,0x02
		MOV		AL,1
		MOV		BX,0
		MOV		DL,0x00
		INT		0x13
		JNC		next
		ADD		SI,1
		CMP		SI,5
		JAE		error
		MOV		AH,0x00
		MOV		DL,0x00
		INT		0x13
		JMP		retry
next:
		MOV		AX,ES
		ADD		AX,0x0020
		MOV		ES,AX
		ADD		CL,1
		CMP		CL,18
		JBE		readloop
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB		readloop
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop
		MOV		[0x0ff0],CH
		JMP		0xc200
error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e
		MOV		BX,15
		INT		0x10
		JMP		putloop
fin:
		HLT
		JMP		fin
msg:
		DB		0x0a, 0x0a
		DB		"load error"
		DB		0x0a
		DB		0
		RESB	0x7dfe-$
		DB		0x55, 0xaa
)";

    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    auto pass1 = std::make_unique<Pass1Strategy>();
    pass1->Eval(pt.get());

    EXPECT_EQ(0x7c00 + 80, pass1->sym_table["entry"]);
    EXPECT_EQ(0x7c00 + 101, pass1->sym_table["readloop"]);
    EXPECT_EQ(0x7c00 + 104, pass1->sym_table["retry"]);
    EXPECT_EQ(0x7c00 + 133, pass1->sym_table["next"]);
    EXPECT_EQ(0x7c00 + 175, pass1->sym_table["error"]);
    EXPECT_EQ(0x7c00 + 178, pass1->sym_table["putloop"]);
    EXPECT_EQ(0x7c00 + 196, pass1->sym_table["fin"]);
    EXPECT_EQ(0x7c00 + 199, pass1->sym_table["msg"]);
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
