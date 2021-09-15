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

    //d->visitChar('H');
    //std::cout << d->ctx.top().AsString() << std::endl;
    //CHECK_EQUAL('H', (char) d->ctx.top().AsString().at(0));
    //d->ctx.pop();

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
    {
        auto numberFactor1 = NumberFactor(7);
        auto numberFactor2 = NumberFactor(3);
        auto immExp1 = ImmExp(numberFactor1.clone());
        auto immExp2 = ImmExp(numberFactor2.clone());
        auto plusExp = PlusExp(immExp1.clone(), immExp2.clone());

        d->visitPlusExp(&plusExp);
        CHECK_EQUAL(10, d->ctx.top().AsInt());
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
        //auto actual = d->ctx.top();

        //CHECK_EQUAL(2, d->ctx.size());
        //CHECK_EQUAL(12, std::any_cast<int>(actual[0]));
        //CHECK_EQUAL(13, std::any_cast<int>(actual[1]));
        //d->ctx.pop();
    }
}

/**

TEST(exp_suite, testSimpleMnemonic)
{

    std::unique_ptr<Driver> d(new Driver(false, false));
    d->Parse<Program>("DB 10, 20, 30", "test.img");

}

TEST(exp_suite, testInt)
{
    int error;

    const double a = te_interp("(5+5)", 0);
    CHECK_EQUAL(10, a);

    const double b = te_interp("(5+5)", &error);
    CHECK_EQUAL(10, b);
    CHECK_EQUAL(0, error);

    const double c = te_interp("(5+5", &error);
    CHECK(std::isnan(c));
    CHECK_EQUAL(4, error);
}
*/

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)
    //args.push_back("TEST(exp_suite, testToken)");

    // TODO: bnfc側のメモリリーク修正する https://github.com/HobbyOSs/opennask/issues/42
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
