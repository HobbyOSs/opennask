#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "nask_utility.hpp"
#include "string_util.hpp"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include <gtest/gtest.h>
#include "front_end.hh"
#include "diff.hh"

class InstSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    InstSuite() {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // 試験終了時に一回だけ実行
    ~InstSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

// テストデータクラス
struct StatementToMachineCodeParam {
    const OPENNASK_MODES _bit_mode;
    const std::string _statement;
    const std::vector<uint8_t> _expected;

    StatementToMachineCodeParam(
        const OPENNASK_MODES bit_mode,
        const std::string& statement,
        const std::vector<uint8_t> expected
    ): _bit_mode(bit_mode),
       _statement(statement),
       _expected(expected)
    {
    }
};

void PrintTo(const StatementToMachineCodeParam& param, ::std::ostream* os) {

    // テストケース名のspaceとカンマは「_」にしたい
    std::string case_name = param._statement;
    for (char& c : case_name) {
        if (c == ' ' || c == ',') {
            c = '_';
        }
        if (c == '[' || c == ']') {
            c = '/';
        }
    }
    *os << case_name;
}

class StatementToMachineCode : public testing::TestWithParam<StatementToMachineCodeParam> {

protected:
    // 試験開始時に一回だけ実行
    StatementToMachineCode() {
        if(!spdlog::get("opennask")) {
            auto logger = spdlog::stdout_color_st("opennask");
        }
    }

    // 試験終了時に一回だけ実行
    ~StatementToMachineCode() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
        //spdlog::set_level(spdlog::level::debug);
        spdlog::set_level(spdlog::level::trace);
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_P(StatementToMachineCode, StatementToMachineCode) {
    const auto p = GetParam();

    std::stringstream ss;
    ss << p._statement;

    auto front = std::make_unique<FrontEnd>(true, true);
    front->bit_mode = p._bit_mode;
    auto pt = front->Parse<Program>(ss);
    front->Eval<Program>(pt.get(), "test.img");

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF, p._expected, front->binout_container);
}

INSTANTIATE_TEST_SUITE_P(InstSuite, StatementToMachineCode,
    testing::Values(
        // 引数なしオペコード
        StatementToMachineCodeParam(ID_16BIT_MODE, "AAA", std::vector<uint8_t>{0x37}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "AAS", std::vector<uint8_t>{0x3f}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CBW", std::vector<uint8_t>{0x98}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CDQ", std::vector<uint8_t>{0x99}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CLC", std::vector<uint8_t>{0xf8}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CLD", std::vector<uint8_t>{0xfc}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CLI", std::vector<uint8_t>{0xfa}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CLTS", std::vector<uint8_t>{0x0f, 0x06}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMC", std::vector<uint8_t>{0xf5}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CWDE", std::vector<uint8_t>{0x98}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DAA", std::vector<uint8_t>{0x27}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DAS", std::vector<uint8_t>{0x2f}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "F2XM1", std::vector<uint8_t>{0xd9, 0xf0}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FABS", std::vector<uint8_t>{0xd9, 0xe1}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FCHS", std::vector<uint8_t>{0xd9, 0xe0}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FCLEX", std::vector<uint8_t>{0x9b, 0xdb, 0xe2}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FCOMPP", std::vector<uint8_t>{0xde, 0xd9}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FCOS", std::vector<uint8_t>{0xd9, 0xff}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FDECSTP", std::vector<uint8_t>{0xd9, 0xf6}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FINCSTP", std::vector<uint8_t>{0xd9, 0xf7}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FINIT", std::vector<uint8_t>{0x9b, 0xdb, 0xe3}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FLD1", std::vector<uint8_t>{0xd9, 0xe8}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FLDL2E", std::vector<uint8_t>{0xd9, 0xea}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FLDL2T", std::vector<uint8_t>{0xd9, 0xe9}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FLDLG2", std::vector<uint8_t>{0xd9, 0xec}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FLDLN2", std::vector<uint8_t>{0xd9, 0xed}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FLDPI", std::vector<uint8_t>{0xd9, 0xeb}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FLDZ", std::vector<uint8_t>{0xd9, 0xee}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FNCLEX", std::vector<uint8_t>{0xdb, 0xe2}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FNINIT", std::vector<uint8_t>{0xdb, 0xe3}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FNOP", std::vector<uint8_t>{0xd9, 0xd0}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FPATAN", std::vector<uint8_t>{0xd9, 0xf3}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FPTAN", std::vector<uint8_t>{0xd9, 0xf2}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FPREM", std::vector<uint8_t>{0xd9, 0xf8}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FPREM1", std::vector<uint8_t>{0xd9, 0xf5}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FRNDINT", std::vector<uint8_t>{0xd9, 0xfc}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FSCALE", std::vector<uint8_t>{0xd9, 0xfd}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FSIN", std::vector<uint8_t>{0xd9, 0xfe}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FSINCOS", std::vector<uint8_t>{0xd9, 0xfb}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FSQRT", std::vector<uint8_t>{0xd9, 0xfa}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FTST", std::vector<uint8_t>{0xd9, 0xe4}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FUCOMPP", std::vector<uint8_t>{0xda, 0xe9}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FXAM", std::vector<uint8_t>{0xd9, 0xe5}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FXTRACT", std::vector<uint8_t>{0xd9, 0xf4}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FYL2X", std::vector<uint8_t>{0xd9, 0xf1}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "FYL2XP1", std::vector<uint8_t>{0xd9, 0xf9}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "HLT", std::vector<uint8_t>{0xf4}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "INT3", std::vector<uint8_t>{0xcc}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "INTO", std::vector<uint8_t>{0xce}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "INVD", std::vector<uint8_t>{0x0f, 0x08}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "IRET", std::vector<uint8_t>{0xcf}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "IRETD", std::vector<uint8_t>{0xcf}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "LAHF", std::vector<uint8_t>{0x9f}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "LEAVE", std::vector<uint8_t>{0xc9}),
        //StatementToMachineCodeParam(ID_16BIT_MODE, "MOVSD", std::vector<uint8_t>{0xa5}), TODO: エラー発生する
        StatementToMachineCodeParam(ID_16BIT_MODE, "NOP", std::vector<uint8_t>{0x90}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "POPA", std::vector<uint8_t>{0x61}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "POPAD", std::vector<uint8_t>{0x61}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "POPF", std::vector<uint8_t>{0x9d}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "POPFD", std::vector<uint8_t>{0x9d}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "PUSHA", std::vector<uint8_t>{0x60}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "PUSHAD", std::vector<uint8_t>{0x60}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "PUSHF", std::vector<uint8_t>{0x9c}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "PUSHFD", std::vector<uint8_t>{0x9c}),
        //StatementToMachineCodeParam(ID_16BIT_MODE, "SHAF", std::vector<uint8_t>{0x9c}), TODO:
        StatementToMachineCodeParam(ID_16BIT_MODE, "STC", std::vector<uint8_t>{0xf9}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "STD", std::vector<uint8_t>{0xfd}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "STI", std::vector<uint8_t>{0xfb}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "WBINVD", std::vector<uint8_t>{0x0f, 0x09}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "XLATB", std::vector<uint8_t>{0xd7}),
        // DB,DW,DD---
        StatementToMachineCodeParam(ID_16BIT_MODE, "DB 1,2,3", std::vector<uint8_t>{0x01, 0x02, 0x03}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DB \"Hello\"", std::vector<uint8_t>{0x48,0x65,0x6c,0x6c,0x6f}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DB 'hello'", std::vector<uint8_t>{0x68,0x65,0x6c,0x6c,0x6f}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DW 512", std::vector<uint8_t>{0x00, 0x02}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DD 0xffffffff", std::vector<uint8_t>{0xff, 0xff, 0xff, 0xff}),
        // CLI,HLT,NOP
        StatementToMachineCodeParam(ID_16BIT_MODE, "CLI", std::vector<uint8_t>{0xfa}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "NOP", std::vector<uint8_t>{0x90}),
        // ADD---
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD AL,1", std::vector<uint8_t>{0x04, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD AX,0x0020", std::vector<uint8_t>{0x05, 0x20, 0x00}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD EAX,3", std::vector<uint8_t>{0x66, 0x83, 0xc0, 0x03}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD BL,1", std::vector<uint8_t>{0x80, 0xc3, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD BH,1", std::vector<uint8_t>{0x80, 0xc7, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD SI,300", std::vector<uint8_t>{0x81, 0xc6, 0x2c, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD SI,1", std::vector<uint8_t>{0x83, 0xc6, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD EBX,1", std::vector<uint8_t>{0x66, 0x83, 0xc3, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "ADD EBX,300", std::vector<uint8_t>{0x66, 0x81, 0xc3, 0x2c, 0x01, 0x00, 0x00}),
        // ADD---
        StatementToMachineCodeParam(ID_16BIT_MODE, "AND EAX,0x7fffffff", std::vector<uint8_t>{0x66, 0x25, 0xff, 0xff, 0xff, 0x7f}),
        // CMP---
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP AL,0x01", std::vector<uint8_t>{0x3c, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP AX,0x2000", std::vector<uint8_t>{0x3d, 0x00, 0x20 }),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP EAX,0x3000", std::vector<uint8_t>{0x66, 0x3d, 0x00, 0x30, 0x00, 0x00}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP BL,1", std::vector<uint8_t>{0x80, 0xfb, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP BH,1", std::vector<uint8_t>{0x80, 0xff, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP SI,300", std::vector<uint8_t>{0x81, 0xfe, 0x2c, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP SI,1", std::vector<uint8_t>{0x83, 0xfe, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP EBX,1", std::vector<uint8_t>{0x66, 0x83, 0xfb, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "CMP EBX,300", std::vector<uint8_t>{0x66, 0x81, 0xfb, 0x2c, 0x01, 0x00, 0x00}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "CMP EBX,1", std::vector<uint8_t>{0x83, 0xfb, 0x01}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "CMP EBX,300", std::vector<uint8_t>{0x81, 0xfb, 0x2c, 0x01, 0x00, 0x00}),
        // OUT---
        StatementToMachineCodeParam(ID_16BIT_MODE, "OUT 0x01,AL", std::vector<uint8_t>{0xe6, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "OUT 0x01,AX", std::vector<uint8_t>{0xe7, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "OUT 0x01,EAX", std::vector<uint8_t>{0x66, 0xe7, 0x01}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "OUT 0x01,EAX", std::vector<uint8_t>{0xe7, 0x01}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "OUT DX,AL", std::vector<uint8_t>{0xee}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "OUT DX,AX", std::vector<uint8_t>{0xef}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "OUT DX,EAX", std::vector<uint8_t>{0x66, 0xef}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "OUT DX,EAX", std::vector<uint8_t>{0xef}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "OUT DX,AL", std::vector<uint8_t>{0xee}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "OUT DX,AX", std::vector<uint8_t>{0x66, 0xef}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "OUT DX,EAX", std::vector<uint8_t>{0xef}),
        // IN---
        StatementToMachineCodeParam(ID_32BIT_MODE, "IN AL,DX", std::vector<uint8_t>{0xec}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "IN AX,DX", std::vector<uint8_t>{0x66, 0xed}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "IN EAX,DX", std::vector<uint8_t>{0xed}),
        // MOV---
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AL,0x0e", std::vector<uint8_t>{0xb0, 0x0e}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AH,0x0e", std::vector<uint8_t>{0xb4, 0x0e}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AL,BL", std::vector<uint8_t>{0x88, 0xd8}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AL,BH", std::vector<uint8_t>{0x88, 0xf8}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AH,BL", std::vector<uint8_t>{0x88, 0xdc}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AH,BH", std::vector<uint8_t>{0x88, 0xfc}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AL,[SI]", std::vector<uint8_t>{0x8a, 0x04}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AH,[SI]", std::vector<uint8_t>{0x8a, 0x24}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV BX,15", std::vector<uint8_t>{0xbb, 0x0f, 0x00}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV AX,BX", std::vector<uint8_t>{0x89, 0xD8}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV ESI,0x7c00", std::vector<uint8_t>{0x66, 0xbe, 0x00, 0x7c, 0x00, 0x00}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "MOV ESI,0x7c00", std::vector<uint8_t>{0xbe, 0x00, 0x7c, 0x00, 0x00}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV SP,0x7c00", std::vector<uint8_t>{0xbc, 0x00, 0x7c}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV BX,SP", std::vector<uint8_t>{0x89, 0xe3}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV SP,BX", std::vector<uint8_t>{0x89, 0xdc}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "JMP 0xc200", std::vector<uint8_t>{0xe9, 0x01, 0xc2}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV [0x0ff0],CH", std::vector<uint8_t>{0x88, 0x2e, 0xf0, 0x0f}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "MOV AX,SS", std::vector<uint8_t>{0x66, 0x8c, 0xd0}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "MOV DS,AX", std::vector<uint8_t>{0x8e, 0xd8}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "MOV EDX,[ESP+4]", std::vector<uint8_t>{0x8b, 0x54, 0x24, 0x04}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "MOV AX,[ESP+4]", std::vector<uint8_t>{0x66, 0x8b, 0x44, 0x24, 0x04}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "MOV [ESP+6],AX", std::vector<uint8_t>{0x66, 0x89, 0x44, 0x24, 0x06}),
        StatementToMachineCodeParam(ID_32BIT_MODE, "XOR DWORD [EBX],0xffffffff", std::vector<uint8_t>{0x83, 0x33, 0xFF})
        //StatementToMachineCodeParam(ID_16BIT_MODE, "CALL 3", std::vector<uint8_t>{0xe8, 0x00, 0x03})
    )
);
