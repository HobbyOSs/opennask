#include "driver.hh"
#include "tinyexpr.h"
#include <memory>
#include <cmath>
#include <CppUTest/TestHarness.h>


TEST_GROUP(exp_suite)
{

};

TEST(exp_suite, testToken)
{
    std::unique_ptr<Driver> d(new Driver(false, false));

    d->visitInteger(30);
    CHECK_EQUAL(30, std::get<int>(d->ctx.top()));
    d->ctx.pop();

    d->visitChar('H');
    CHECK_EQUAL('H', std::get<char>(d->ctx.top()));
    d->ctx.pop();

    d->visitDouble(3.14);
    CHECK_EQUAL(3.14, std::get<double>(d->ctx.top()));
    d->ctx.pop();

    d->visitString("hello1");
    CHECK_EQUAL("hello1", std::get<std::string>(d->ctx.top()));
    d->ctx.pop();

    d->visitIdent("hello2");
    CHECK_EQUAL("hello2", std::get<std::string>(d->ctx.top()));
    d->ctx.pop();

    d->visitHex("hello3");
    CHECK_EQUAL("hello3", std::get<std::string>(d->ctx.top()));
    d->ctx.pop();
}

TEST(exp_suite, testFactor)
{
    std::unique_ptr<Driver> d(new Driver(false, false));

    auto numberFactor = NumberFactor(30);
    d->visitNumberFactor(&numberFactor);
    CHECK_EQUAL(30, std::get<int>(d->ctx.top()));
    d->ctx.pop();

    auto hexFactor = HexFactor("hello1");
    d->visitHexFactor(&hexFactor);
    CHECK_EQUAL("hello1", std::get<std::string>(d->ctx.top()));
    d->ctx.pop();

    auto identFactor = IdentFactor("hello2");
    d->visitIdentFactor(&identFactor);
    CHECK_EQUAL("hello2", std::get<std::string>(d->ctx.top()));
    d->ctx.pop();

    auto stringFactor = StringFactor("hello3");
    d->visitStringFactor(&stringFactor);
    CHECK_EQUAL("hello3", std::get<std::string>(d->ctx.top()));
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
        CHECK_EQUAL(30, std::get<int>(d->ctx.top()));
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(hexFactor.clone());
        d->visitImmExp(&immExp);
        CHECK_EQUAL("hello1", std::get<std::string>(d->ctx.top()));
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(identFactor.clone());
        d->visitImmExp(&immExp);
        CHECK_EQUAL("hello2", std::get<std::string>(d->ctx.top()));
        d->ctx.pop();
    }
    {
        auto immExp = ImmExp(stringFactor.clone());
        d->visitImmExp(&immExp);
        CHECK_EQUAL("hello3", std::get<std::string>(d->ctx.top()));
        d->ctx.pop();
    }
}

TEST(exp_suite, testPlusExp)
{
    std::unique_ptr<Driver> d(new Driver(false, false));

    auto numberFactor1 = NumberFactor(7);
    auto numberFactor2 = NumberFactor(5);

    {
        auto immExp1 = ImmExp(numberFactor1.clone());
        auto immExp2 = ImmExp(numberFactor2.clone());
        auto plusExp = PlusExp(immExp1.clone(), immExp2.clone());
        d->visitPlusExp(&plusExp);
        CHECK_EQUAL(12, std::get<int>(d->ctx.top()));
        d->ctx.pop();
    }
}

TEST(exp_suite, testCalc)
{
    std::string input = "";
    FILE *old_stdin = stdin;
    FILE* strm = fmemopen((void*) input.c_str(), input.size(), "r");

    stdin = strm;
    scanf("512+1024+4");
    stdin = old_stdin;
    fclose(strm);

    Exp *exp = pExp(stdin);


    // const double ecx1 = te_interp("512*1024/4", &error);
    // CHECK(!std::isnan(ecx1));
    // CHECK_EQUAL(131072, ecx1);
    //
    // const double ecx2 = te_interp("512/4", &error);
    // CHECK(!std::isnan(ecx2));
    // CHECK_EQUAL(128, ecx2);
    //
    // const double dw = te_interp("8*3-1", &error);
    // CHECK(!std::isnan(dw));
    // CHECK_EQUAL(23, dw);
}

/**

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
