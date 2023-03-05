#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "x86.hh"
#include <gtest/gtest.h>

using namespace x86_64;
using namespace jsoncons;

class X86TableSuite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    X86TableSuite() {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // 試験終了時に一回だけ実行
    ~X86TableSuite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(X86TableSuite, CheckJsonSchema)
{

    using namespace x86_64;
    using namespace jsoncons;

    try {
        json j = json::parse(std::string(X86_64_JSON));
        const json& inst = j["instructions"];
        for (const auto &item : inst.object_range()) {
            // debug code
            // std::string opcode = item.key();
            // std::cout << pretty_print(item.value()) << std::endl;
            // break;
            Instruction inst = \
                jsoncons::decode_json<Instruction>(item.value().to_string());
        }

        InstructionSet iset =                                           \
            jsoncons::decode_json<InstructionSet>(std::string(X86_64_JSON));

        EXPECT_EQ("x86-64", iset.instruction_set());
        EXPECT_EQ(1215, iset.instructions().size());
        EXPECT_EQ(1, iset.instructions().count("ADC"));
        EXPECT_EQ("Add with Carry", iset.instructions().at("ADC").summary());

    } catch(const jsoncons::conv_error& e) {
        std::cout << "Caught conv_error with category "
                  << e.code().category().name()
                  << ", code " << e.code().value()
                  << " and message " << e.what()
                  << " and line " << e.line()
                  << std::endl;
    }
}

TEST_F(X86TableSuite, GetOutputSize)
{
    auto iset = decode_json<InstructionSet>(std::string(X86_64_JSON));

    EXPECT_EQ("x86-64", iset.instruction_set());
    EXPECT_EQ(1215, iset.instructions().size());
    EXPECT_EQ(1, iset.instructions().count("MOV"));
}
