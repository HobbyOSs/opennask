#include "front_end.hh"
#include "pass1_strategy.hh"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <gtest/gtest.h>

class Pass1Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Pass1Suite() {
        spdlog::set_level(spdlog::level::trace);
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


TEST_F(Pass1Suite, Harib00i) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; シンボルテーブルへのラベルの設定の結合テスト
; 各種命令の機械語のサイズ取得のe2eテストも兼ねる
BOTPAK	EQU		0x00280000
DSKCAC	EQU		0x00100000
DSKCAC0	EQU		0x00008000
CYLS	EQU		0x0ff0
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2
SCRNX	EQU		0x0ff4
SCRNY	EQU		0x0ff6
VRAM	EQU		0x0ff8

		ORG		0xc200

		MOV		AL,0x13
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000
		MOV		AH,0x02
		INT		0x16
		MOV		[LEDS],AL
		MOV		AL,0xff
		OUT		0x21,AL
		NOP
		OUT		0xa1,AL
		CLI
		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf
		OUT		0x60,AL
		CALL	waitkbdout

[INSTRSET "i486p"]
		LGDT	[GDTR0]
		MOV		EAX,CR0
		AND		EAX,0x7fffffff
		OR		EAX,0x00000001
		MOV		CR0,EAX
		JMP		pipelineflush
pipelineflush:
		MOV		AX,1*8
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX
		MOV		ESI,bootpack
		MOV		EDI,BOTPAK
		MOV		ECX,512*1024/4
		CALL	memcpy
		MOV		ESI,0x7c00
		MOV		EDI,DSKCAC
		MOV		ECX,512/4
		CALL	memcpy
		MOV		ESI,DSKCAC0+512
		MOV		EDI,DSKCAC+512
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4
		SUB		ECX,512/4
		CALL	memcpy
		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3
		SHR		ECX,2
		JZ		skip
		MOV		ESI,[EBX+20]
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]
		JMP		DWORD 2*8:0x0000001b
waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout
		RET
memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy
		RET
		ALIGNB	16
GDT0:
		RESB	8
		DW		0xffff,0x0000,0x9200,0x00cf
		DW		0xffff,0x0000,0x9a28,0x0047
		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0
		ALIGNB	16
bootpack:
)";

    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    auto pass1 = std::make_unique<Pass1Strategy>();
    pass1->Eval(pt.get());

    // 0xc200=49664
    EXPECT_EQ(0xc200 + 87,  pass1->sym_table["pipelineflush"]);
    EXPECT_EQ(0xc200 + 218, pass1->sym_table["skip"]);
    GTEST_SKIP(); // TODO: まだ機能しない
    EXPECT_EQ(0xc200 + 231, pass1->sym_table["waitkbdout"]);
    EXPECT_EQ(0xc200 + 238, pass1->sym_table["memcpy"]);
    EXPECT_EQ(0xc200 + 272, pass1->sym_table["GDT0"]);
    EXPECT_EQ(0xc200 + 298, pass1->sym_table["GDTR0"]);
    EXPECT_EQ(0xc200 + 304, pass1->sym_table["bootpack"]);
}
