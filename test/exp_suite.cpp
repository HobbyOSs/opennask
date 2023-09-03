#include "front_end.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <optional>
#include <list>
#include <memory>
#include <cmath>
#include <gtest/gtest.h>
#include "diff.hh"

class ExpSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    ExpSuite() {
        spdlog::set_level(spdlog::level::trace);
    }

    // 試験終了時に一回だけ実行
    ~ExpSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(ExpSuite, ParaToken)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));

    d->visitInteger(30);
    EXPECT_EQ(30, d->ctx.top().AsUInt32());
    d->ctx.pop();

    d->visitChar('H');
    std::cout << d->ctx.top().AsString() << std::endl;
    EXPECT_EQ("H", d->ctx.top().AsString());
    d->ctx.pop();

    d->visitDouble(3.14);
    EXPECT_EQ(3.14, d->ctx.top().AsDouble());
    d->ctx.pop();

    d->visitString("hello1");
    EXPECT_EQ("hello1", d->ctx.top().AsString());
    d->ctx.pop();

    d->visitIdent("hello2");
    EXPECT_EQ("hello2", d->ctx.top().AsString());
    d->ctx.pop();

    d->visitHex("hello3");
    EXPECT_EQ("hello3", d->ctx.top().AsString());
    d->ctx.pop();
}

TEST_F(ExpSuite, Factor)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));
    auto numberFactor = NumberFactor(30);
    d->visitNumberFactor(&numberFactor);
    EXPECT_EQ(30, d->ctx.top().AsUInt32());
    d->ctx.pop();

    auto hexFactor = HexFactor("hello1");
    d->visitHexFactor(&hexFactor);
    EXPECT_EQ("hello1", d->ctx.top().AsString());
    d->ctx.pop();

    auto identFactor = IdentFactor("hello2");
    d->visitIdentFactor(&identFactor);
    EXPECT_EQ("hello2", d->ctx.top().AsString());
    d->ctx.pop();

    auto stringFactor = StringFactor("hello3");
    d->visitStringFactor(&stringFactor);
    EXPECT_EQ("hello3", d->ctx.top().AsString());
    d->ctx.pop();
}

TEST_F(ExpSuite, ImmExp)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));

    auto numberFactor = NumberFactor(30);
    auto hexFactor = HexFactor("hello1");
    auto identFactor = IdentFactor("hello2");
    auto stringFactor = StringFactor("hello3");

    {
        auto immExp = ImmExp(numberFactor.clone());
        d->visitImmExp(&immExp);
        EXPECT_EQ(30, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(hexFactor.clone());
        d->visitImmExp(&immExp);
        EXPECT_EQ("hello1", d->ctx.top().AsString());
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(identFactor.clone());
        d->visitImmExp(&immExp);
        EXPECT_EQ("hello2", d->ctx.top().AsString());
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(stringFactor.clone());
        d->visitImmExp(&immExp);
        EXPECT_EQ("hello3", d->ctx.top().AsString());
        d->ctx.pop();
    }
}

// テストデータクラス
struct MemoryAddrExpParam {
    const std::string _token;
    const std::optional<asmjit::x86::Reg> _base_reg;
    const std::optional<int32_t> _offset;

    MemoryAddrExpParam(
        const std::string& token,
        const std::optional<asmjit::x86::Reg> base_reg,
        const std::optional<int32_t> _offset
    ):
        _token(token), _base_reg(base_reg), _offset(_offset) {}
};

void PrintTo(const MemoryAddrExpParam& param, ::std::ostream* os) {
    *os << param._token
        << ";has_base:"
        << (param._base_reg.has_value() ? "true" : "false")
        << ";offset:"
        << (param._offset.has_value() ? std::to_string(param._offset.value()) : "n/a");
}

class MemoryAddrExpTest : public testing::TestWithParam<MemoryAddrExpParam> {};

TEST_P(MemoryAddrExpTest, MemoryAddrExpTest) {
    const auto p = GetParam();

    auto front = std::make_unique<FrontEnd>(false, false);

    std::stringstream ss;
    ss << "DB "; // dummyでopcodeとしてDBを入れておく, bison, flexでpartialに字句解析/構文解析のテストを行うのが厳しいため
    ss << p._token;

    auto pt = front->Parse<Exp>(ss);
    front->visitExp(pt.get()); // [AL] のようなメモリーアドレス表現はExpからパースされる

    TParaToken token = front->ctx.top();
    auto mem = token.AsMem();

    if (p._base_reg.has_value()) {
        EXPECT_TRUE(mem.hasBase());
        EXPECT_TRUE(mem.baseReg().isSame(p._base_reg.value()));
    } else {
        EXPECT_FALSE(mem.hasBase());
    }

    if (p._offset.has_value()) {
        EXPECT_TRUE(mem.hasOffset());
        EXPECT_EQ(p._offset.value(), mem.offset());
    } else {
        EXPECT_FALSE(mem.hasOffset());
    }
}

// テスト項目
// メモリーアドレス表現                   : "[BL]"
// 格納されるべきasmjit側のベースレジスタ : asmjit::x86::al
// 格納されるべきasmjit側のオフセット     : 2
INSTANTIATE_TEST_SUITE_P(ExpSuite, MemoryAddrExpTest,
    testing::Values(
        MemoryAddrExpParam("[AL]"         , asmjit::x86::al , std::nullopt),
        MemoryAddrExpParam("[BL]"         , asmjit::x86::bl , std::nullopt),
        MemoryAddrExpParam("[AX]"         , asmjit::x86::ax , std::nullopt),
        MemoryAddrExpParam("[BX]"         , asmjit::x86::bx , std::nullopt),
        MemoryAddrExpParam("[SI]"         , asmjit::x86::si , std::nullopt),
        MemoryAddrExpParam("[EAX]"        , asmjit::x86::eax, std::nullopt),
        MemoryAddrExpParam("[EBX]"        , asmjit::x86::ebx, std::nullopt),
        MemoryAddrExpParam("[0x0ff0]"     , std::nullopt,     0x0ff0),
        MemoryAddrExpParam("WORD [0x0ff0]", std::nullopt,     0x0ff0),
        MemoryAddrExpParam("[BP + 1]"     , asmjit::x86::bp , 1),
        MemoryAddrExpParam("[BX + 2]"     , asmjit::x86::bx , 2),
        MemoryAddrExpParam("[SI + 3]"     , asmjit::x86::si , 3),
        MemoryAddrExpParam("[DI + 4]"     , asmjit::x86::di , 4),
        MemoryAddrExpParam("[EBX + 16]"   , asmjit::x86::ebx, 16),
        MemoryAddrExpParam("[ESP + 20]"   , asmjit::x86::esp, 20),
        MemoryAddrExpParam("[ESP+12+4]"   , asmjit::x86::esp, 16)
    )
);


TEST_F(ExpSuite, ArithmeticOperations)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));
    auto numberFactor1 = NumberFactor(7);
    auto numberFactor2 = NumberFactor(3);
    auto immExp1 = ImmExp(numberFactor1.clone());
    auto immExp2 = ImmExp(numberFactor2.clone());

    {
        auto plusExp = PlusExp(immExp1.clone(), immExp2.clone());
        d->visitPlusExp(&plusExp);
        EXPECT_EQ(10, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
    {
        auto minusExp = MinusExp(immExp1.clone(), immExp2.clone());
        d->visitMinusExp(&minusExp);
        EXPECT_EQ(4, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
    {
        auto mulExp = MulExp(immExp1.clone(), immExp2.clone());
        d->visitMulExp(&mulExp);
        EXPECT_EQ(21, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
    {
        auto divExp = DivExp(immExp1.clone(), immExp2.clone());
        d->visitDivExp(&divExp);
        EXPECT_EQ(2, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
    {
        auto modExp = ModExp(immExp1.clone(), immExp2.clone());
        d->visitModExp(&modExp);
        EXPECT_EQ(1, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
}

TEST_F(ExpSuite, MnemoArgs)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));
    {
        auto numberFactor = NumberFactor(12);
        auto immExp = ImmExp(numberFactor.clone());
        auto mnemoArg = MnemoArg(immExp.clone());

        d->visitMnemoArg(&mnemoArg);
        EXPECT_EQ(12, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
    {
        auto mnemoArg1 = MnemoArg(ImmExp(NumberFactor(12).clone()).clone());
        auto mnemoArg2 = MnemoArg(ImmExp(NumberFactor(13).clone()).clone());
        auto mnemoArgs = ListMnemonicArgs();

        mnemoArgs.cons(mnemoArg1.clone());
        mnemoArgs.cons(mnemoArg2.clone());

        d->visitListMnemonicArgs(&mnemoArgs);
        EXPECT_EQ(2, d->ctx.size());

        EXPECT_EQ(13, d->ctx.top().AsUInt32());
        d->ctx.pop();
        EXPECT_EQ(12, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
}

TEST_F(ExpSuite, SimpleMnemonic)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));
    auto expected = std::vector<uint8_t>{10,20,30};
    std::stringstream ss;
    ss << "DB 10,20,30";
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

    EXPECT_EQ(3, d->binout_container.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}

TEST_F(ExpSuite, MovMemoryAddressing)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));
    auto expected = std::vector<uint8_t>{0x8b, 0x4c, 0x24, 0x04, 0x8a, 0x44, 0x24, 0x08};
    std::stringstream ss;
    ss << u8R"##(
[BITS 32]
[INSTRSET "i486p"]

MOV ECX,[ESP+4]
MOV AL,[ESP+8]
)##";

    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");
    EXPECT_EQ(ID_32BIT_MODE, d->bit_mode);

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF, expected, d->binout_container);
}


TEST_F(ExpSuite, DeclareStmt)
{
    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));
    {
        auto numberFactor = NumberFactor(10);
        auto immExp = ImmExp(numberFactor.clone());
        auto declareStmt = DeclareStmt("CYLS", immExp.clone());

        d->visitDeclareStmt(&declareStmt);
        EXPECT_EQ(10, d->equ_map["CYLS"].AsUInt32());

        d->visitIdent("CYLS");
        EXPECT_EQ("10", d->ctx.top().AsString());
        EXPECT_EQ(10, d->ctx.top().AsUInt32());
        d->ctx.pop();
    }
}

// テストデータクラス
struct StatementsToMachineCodeParam {
    const OPENNASK_MODES _bit_mode;
    const std::string _statement;
    const std::vector<uint8_t> _expected;

    StatementsToMachineCodeParam(
        const OPENNASK_MODES bit_mode,
        const std::string& statement,
        const std::vector<uint8_t> expected
    ): _bit_mode(bit_mode),
       _statement(statement),
       _expected(expected)
    {
    }
};

void PrintTo(const StatementsToMachineCodeParam& param, ::std::ostream* os) {
    std::istringstream iss(param._statement);
    std::vector<std::string> lines(std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>());

    for (auto& line : lines) {
        // trim
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        // スペースとカンマは「_」に変換
        std::replace(line.begin(), line.end(), ' ', '_');
        std::replace(line.begin(), line.end(), ',', '_');

        // ブラケットは「/」に変換
        std::replace(line.begin(), line.end(), '[', '/');
        std::replace(line.begin(), line.end(), ']', '/');
    }

    std::ostringstream oss;
    std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(oss, "_"));

    *os << oss.str();
}

class StatementsToMachineCode : public testing::TestWithParam<StatementsToMachineCodeParam> {};

TEST_P(StatementsToMachineCode, StatementsToMachineCode) {
    const auto p = GetParam();

    std::stringstream ss;
    ss << p._statement;

    auto front = std::make_unique<FrontEnd>(false, false);
    front->bit_mode = p._bit_mode;
    auto pt = front->Parse<Program>(ss);
    front->Eval<Program>(pt.get(), "test.img");

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF, p._expected, front->binout_container);
}


INSTANTIATE_TEST_SUITE_P(ExpSuite, StatementsToMachineCode,
    testing::Values(
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     R"(DB 0x01
                                        DB 0x02
                                        DB 0x03)",
                                     std::vector<uint8_t>{0x01, 0x02, 0x03}),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     "RESB 18",
                                     std::vector<uint8_t>(18, 0x00)),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     "RESB 0x12-$",
                                     std::vector<uint8_t>(18, 0x00)),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     R"(MOV AX,0
                                        MOV SS,AX)",
                                     std::vector<uint8_t>{0xb8, 0x00, 0x00, 0x8e, 0xd0}),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     "JMP DWORD 2*8:0x0000001b",
                                     std::vector<uint8_t>{0x66, 0xea, 0x1b,0x00, 0x00, 0x00, 0x10, 0x00}),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     R"([INSTRSET "i486p"]
                                        MOV [EDI],EAX)",
                                     std::vector<uint8_t>{0x67, 0x66, 0x89, 0x07}),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     R"([INSTRSET "i486p"]
                                        MOV EAX,[ESI])",
                                     std::vector<uint8_t>{0x67, 0x66, 0x8b, 0x06}),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     R"([INSTRSET "i486p"]
                                        MOV ESI,[EBX+20]
                                        MOV EDI,[EBX+12])",
                                     std::vector<uint8_t>{0x67, 0x66, 0x8b, 0x73, 0x14, 0x67, 0x66, 0x8b, 0x7b, 0x0c})
    ));
