#include <fstream>
#include <typeinfo>
#include <type_traits>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "matchit.h"
#include "front_end.hh"
#include "driver.hh"
#include "parser.hh"
#include "demangle.hpp"
#include "mod_rm.hpp"


using namespace std::placeholders;
using namespace matchit;


void FrontEnd::processDB(std::vector<TParaToken>& mnemonic_args) {

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        for (const auto& e : mnemonic_args) {
            log()->debug("[pass2] {}", e.to_string());

            if (e.IsInteger() || e.IsHex()) {
                a.db(e.AsInt32());
            } else if (e.IsIdentifier()) {
                const std::string s = e.AsString();
                std::for_each(s.begin(),
                              s.end(),
                              [&](char c) { a.db(c); });
            }
        }
    });
}

void FrontEnd::processDW(std::vector<TParaToken>& mnemonic_args) {
    // uint16_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        for (const auto& e : mnemonic_args) {
            log()->debug("[pass2] {}", e.to_string());

            if (e.IsInteger() || e.IsHex()) {
                a.dw(e.AsInt32());
            } else if (e.IsIdentifier()) {
                throw std::runtime_error("not implemented");
            }
        }
    });
}

void FrontEnd::processDD(std::vector<TParaToken>& mnemonic_args) {
    // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        for (const auto& e : mnemonic_args) {
            log()->debug("[pass2] {}", e.to_string());

            if (e.IsInteger() || e.IsHex()) {
                a.dd(e.AsInt32());
            } else if (e.IsIdentifier()) {
                throw std::runtime_error("not implemented");
            }
        }
    });
}

void FrontEnd::processRESB(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    const std::string suffix = "-$";

    with_asmjit([&](asmjit::x86::Assembler& a, PrefixInfo& _) {
        using namespace asmjit;

        if (auto range = arg.AsString(); range.find(suffix) != std::string::npos) {
            log()->debug("[pass2] type: {}, value: {}", type(arg), arg.to_string());
            auto resb_size = range.substr(0, range.length() - suffix.length());
            auto resb_token = TParaToken(resb_size, TParaToken::ttHex);

            CodeBuffer& buf = code_.textSection()->buffer();
            log()->debug("[pass2] padding upto: {}(={}), current: {}",
                         resb_token.AsString(), resb_token.AsInt32(), buf.size());
            a.db(0x00, resb_token.AsInt32() - dollar_position - buf.size());
            return;
        }

        arg.MustBe(TParaToken::ttInteger);
        log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsInt32());
        a.db(0x00, arg.AsInt32());
    });
}

void FrontEnd::processORG(std::vector<TParaToken>& mnemonic_args) {

    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttHex);
    log()->debug("[pass2] type: {}, value: {}", type(arg), arg.AsUInt32());
    dollar_position = arg.AsUInt32();
    code_.relocateToBase(dollar_position);
}
