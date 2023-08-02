#include "front_end.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <list>
#include <memory>
#include <cmath>
#include <gtest/gtest.h>
#include "diff.hh"

class ExpSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    ExpSuite() {
        spdlog::set_level(spdlog::level::debug);
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
    const std::string token;
    const TParaToken::TIdentiferAttribute expected;

    MemoryAddrExpParam(
        const std::string& token,
        const TParaToken::TIdentiferAttribute expected
    ):
        token(token), expected(expected) {}
};

void PrintTo(const MemoryAddrExpParam& param, ::std::ostream* os) {
    *os << param.token
        << " = "
        << TParaToken::TIAttributeNames[param.expected];
}

class MemoryAddrExpTest : public testing::TestWithParam<MemoryAddrExpParam> {};

TEST_P(MemoryAddrExpTest, MemoryAddrExpTest) {
    const auto p = GetParam();

    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));

    std::stringstream ss;
    ss << p.token;

    auto front = std::make_unique<FrontEnd>(true, true);
    //front->bit_mode = p._bit_mode;
    auto pt = front->Parse<MemoryAddr>(ss);

    //d->visitMemoryAddrExp(&indirectAddrExp);
    //EXPECT_EQ(p.token, d->ctx.top().AsString());
    //EXPECT_EQ(p.expected, d->ctx.top().AsAttr());
    //d->ctx.pop();
}

INSTANTIATE_TEST_SUITE_P(ExpSuite, MemoryAddrExpTest,
    testing::Values(
        MemoryAddrExpParam("[AL]", TParaToken::ttMem8),
        MemoryAddrExpParam("[AX]", TParaToken::ttMem16),
        MemoryAddrExpParam("[EAX]", TParaToken::ttMem32),
        MemoryAddrExpParam("[RAX]", TParaToken::ttMem64)
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

TEST_F(ExpSuite, MovWithBracket)
{
    GTEST_SKIP(); // TODO: まだ機能しない

    std::unique_ptr<FrontEnd> d(new FrontEnd(false, false));
    auto expected = std::vector<uint8_t>{0x8b, 0x4c, 0x24, 0x04, 0x8a, 0x44, 0x24, 0x08};
    std::stringstream ss;
    ss << u8R"##(
# [BITS 32]
# [INSTRSET "i486p"]

MOV ECX,[ESP+4]
MOV AL,[ESP+8]
)##";

    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");
    EXPECT_EQ(8, d->binout_container.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
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
    *os << param._statement;
}

class StatementsToMachineCode : public testing::TestWithParam<StatementsToMachineCodeParam> {};

TEST_P(StatementsToMachineCode, StatementsToMachineCode) {
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


INSTANTIATE_TEST_SUITE_P(ExpSuite, StatementsToMachineCode,
    testing::Values(
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     "DB 0x01\nDB 0x02\nDB 0x03",
                                     std::vector<uint8_t>{0x01, 0x02, 0x03}),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     "RESB 18",
                                     std::vector<uint8_t>(0x00, 18)),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     "RESB 0x12-$",
                                     std::vector<uint8_t>(0x00, 18)),
        StatementsToMachineCodeParam(ID_16BIT_MODE,
                                     "MOV AX,0\nMOV SS,AX",
                                     std::vector<uint8_t>{0xb8, 0x00, 0x00, 0x8e, 0xd0})

    ));
