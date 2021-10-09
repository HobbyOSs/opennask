#include "driver.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"
#include <list>
#include <memory>
#include <cmath>
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

// メモリリーク扱いされるのでテストケース側でloggerを作る
auto logger = spdlog::stdout_logger_mt("opennask", "console");

TEST_GROUP(exp_suite)
{

};

TEST(exp_suite, testToken)
{
    std::unique_ptr<Driver> d(new Driver(false, false));

    d->visitInteger(30);
    CHECK_EQUAL(30, d->ctx.top().AsInt());
    d->ctx.pop();

    d->visitChar('H');
    std::cout << d->ctx.top().AsString() << std::endl;
    CHECK_EQUAL("H", d->ctx.top().AsString());
    d->ctx.pop();

    d->visitDouble(3.14);
    CHECK_EQUAL(3.14, d->ctx.top().AsDouble());
    d->ctx.pop();

    d->visitString("hello1");
    CHECK_EQUAL("hello1", d->ctx.top().AsString());
    d->ctx.pop();

    d->visitIdent("hello2");
    CHECK_EQUAL("hello2", d->ctx.top().AsString());
    d->ctx.pop();

    d->visitHex("hello3");
    CHECK_EQUAL("hello3", d->ctx.top().AsString());
    d->ctx.pop();
}

TEST(exp_suite, testFactor)
{
    std::unique_ptr<Driver> d(new Driver(false, false));
    auto numberFactor = NumberFactor(30);
    d->visitNumberFactor(&numberFactor);
    CHECK_EQUAL(30, d->ctx.top().AsInt());
    d->ctx.pop();

    auto hexFactor = HexFactor("hello1");
    d->visitHexFactor(&hexFactor);
    CHECK_EQUAL("hello1", d->ctx.top().AsString());
    d->ctx.pop();

    auto identFactor = IdentFactor("hello2");
    d->visitIdentFactor(&identFactor);
    CHECK_EQUAL("hello2", d->ctx.top().AsString());
    d->ctx.pop();

    auto stringFactor = StringFactor("hello3");
    d->visitStringFactor(&stringFactor);
    CHECK_EQUAL("hello3", d->ctx.top().AsString());
    d->ctx.pop();
}

TEST(exp_suite, testImmExp)
{
    std::unique_ptr<Driver> d(new Driver(false, false));

    auto numberFactor = NumberFactor(30);
    auto hexFactor = HexFactor("hello1");
    auto identFactor = IdentFactor("hello2");
    auto stringFactor = StringFactor("hello3");

    {
        auto immExp = ImmExp(numberFactor.clone());
        d->visitImmExp(&immExp);
        CHECK_EQUAL(30, d->ctx.top().AsInt());
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(hexFactor.clone());
        d->visitImmExp(&immExp);
        CHECK_EQUAL("hello1", d->ctx.top().AsString());
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(identFactor.clone());
        d->visitImmExp(&immExp);
        CHECK_EQUAL("hello2", d->ctx.top().AsString());
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(stringFactor.clone());
        d->visitImmExp(&immExp);
        CHECK_EQUAL("hello3", d->ctx.top().AsString());
        d->ctx.pop();
    }
}

TEST(exp_suite, testArithmeticOperations)
{
    std::unique_ptr<Driver> d(new Driver(false, false));
    auto numberFactor1 = NumberFactor(7);
    auto numberFactor2 = NumberFactor(3);
    auto immExp1 = ImmExp(numberFactor1.clone());
    auto immExp2 = ImmExp(numberFactor2.clone());

    {
        auto plusExp = PlusExp(immExp1.clone(), immExp2.clone());
        d->visitPlusExp(&plusExp);
        CHECK_EQUAL(10, d->ctx.top().AsInt());
        d->ctx.pop();
    }
    {
        auto minusExp = MinusExp(immExp1.clone(), immExp2.clone());
        d->visitMinusExp(&minusExp);
        CHECK_EQUAL(4, d->ctx.top().AsInt());
        d->ctx.pop();
    }
    {
        auto mulExp = MulExp(immExp1.clone(), immExp2.clone());
        d->visitMulExp(&mulExp);
        CHECK_EQUAL(21, d->ctx.top().AsInt());
        d->ctx.pop();
    }
    {
        auto divExp = DivExp(immExp1.clone(), immExp2.clone());
        d->visitDivExp(&divExp);
        CHECK_EQUAL(2, d->ctx.top().AsInt());
        d->ctx.pop();
    }
    {
        auto modExp = ModExp(immExp1.clone(), immExp2.clone());
        d->visitModExp(&modExp);
        CHECK_EQUAL(1, d->ctx.top().AsInt());
        d->ctx.pop();
    }
}

TEST(exp_suite, testMnemoArgs)
{
    std::unique_ptr<Driver> d(new Driver(false, false));
    {
        auto numberFactor = NumberFactor(12);
        auto immExp = ImmExp(numberFactor.clone());
        auto mnemoArg = MnemoArg(immExp.clone());

        d->visitMnemoArg(&mnemoArg);
        CHECK_EQUAL(12, d->ctx.top().AsInt());
        d->ctx.pop();
    }
    {
        auto mnemoArg1 = MnemoArg(ImmExp(NumberFactor(12).clone()).clone());
        auto mnemoArg2 = MnemoArg(ImmExp(NumberFactor(13).clone()).clone());
        auto mnemoArgs = ListMnemonicArgs();

        mnemoArgs.push_back(mnemoArg1.clone());
        mnemoArgs.push_back(mnemoArg2.clone());

        d->visitListMnemonicArgs(&mnemoArgs);
        CHECK_EQUAL(2, d->ctx.size());

        CHECK_EQUAL(13, d->ctx.top().AsInt());
        d->ctx.pop();
        CHECK_EQUAL(12, d->ctx.top().AsInt());
        d->ctx.pop();
    }
}

TEST(exp_suite, testSimpleMnemonic)
{
    std::unique_ptr<Driver> d(new Driver(false, false));
    auto expected = std::vector<uint8_t>{10,20,30};
    d->Parse<Program>("DB 10,20,30", "test.img");
    CHECK_EQUAL(3, d->binout_container.size());
    CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}

TEST(exp_suite, testDeclareStmt)
{
    std::unique_ptr<Driver> d(new Driver(false, false));
    {
        auto numberFactor = NumberFactor(10);
        auto immExp = ImmExp(numberFactor.clone());
        auto declareStmt = DeclareStmt("CYLS", immExp.clone());

        d->visitDeclareStmt(&declareStmt);
        CHECK_EQUAL(10, d->equ_map["CYLS"].AsInt());

        d->visitIdent("CYLS");
        CHECK_EQUAL("10", d->ctx.top().AsString());
        CHECK_EQUAL(10, d->ctx.top().AsInt());
        d->ctx.pop();
    }
}

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)
    //args.push_back("TEST(exp_suite, testDeclareStmt)");

    // TODO: bnfc側のメモリリーク修正する https://github.com/HobbyOSs/opennask/issues/42
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
