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
    *os << param._statement;
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
        // DB,DW,DD---
        StatementToMachineCodeParam(ID_16BIT_MODE, "DB 1,2,3", std::vector<uint8_t>{0x01, 0x02, 0x03}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DB \"Hello\"", std::vector<uint8_t>{0x48,0x65,0x6c,0x6c,0x6f}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DW 512", std::vector<uint8_t>{0x00, 0x02}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "DD 0xffffffff", std::vector<uint8_t>{0xff, 0xff, 0xff, 0xff}),
        // CLI,HLT,NOP
        StatementToMachineCodeParam(ID_16BIT_MODE, "CLI", std::vector<uint8_t>{0xfa}),
        StatementToMachineCodeParam(ID_16BIT_MODE, "HLT", std::vector<uint8_t>{0xf4}),
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
        StatementToMachineCodeParam(ID_16BIT_MODE, "MOV [0x0ff0],CH", std::vector<uint8_t>{0x88, 0x2e, 0xf0, 0x0f})
        //StatementToMachineCodeParam(ID_16BIT_MODE, "CALL 3", std::vector<uint8_t>{0xe8, 0x00, 0x03})
    )
);
