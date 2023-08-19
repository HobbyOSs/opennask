#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "matchit.h"
#include "coffi/coffi.hpp"
#include "front_end.hh"
#include "demangle.hpp"
#include "object_file_writer.hh"

using namespace std::placeholders;
using namespace matchit;


void FrontEnd::visitConfigStmt(ConfigStmt *config_stmt) {

    // TODO: 必要な設定を行う, コンフィグ項目ごとにfactory的なコーディングをすればよさそう
    // [FORMAT "WCOFF"], [FILE "xxxx.c"], [INSTRSET "i386"], [BITS 32]
    if (config_stmt->configtype_) config_stmt->configtype_->accept(this);
    if (config_stmt->factor_) config_stmt->factor_->accept(this);

    const std::string config_type = type(*config_stmt->configtype_);
    TParaToken t = this->ctx.top();
    log()->debug("[pass2] visitConfigStmt: args = {}", t.to_string());
    log()->debug("[pass2] config_type = {}, str = {}", config_type, t.AsString());

    match(config_type)(
        pattern | "BitsConfig" | when (t.AsString() == "16") = [&] { this->bit_mode = ID_16BIT_MODE; },
        pattern | "BitsConfig" | when (t.AsString() == "32") = [&] { this->bit_mode = ID_32BIT_MODE; },
        pattern | "BitsConfig" = [&] {
            throw std::runtime_error("Invalid bit_mode: " + t.AsString());
        },
        pattern | "FormConfig" | when (t.AsString() == "WCOFF") = [&] {
            auto writer = std::make_unique<ObjectFileWriter>();
            writer->write_coff(*a_);
        },
        pattern | "InstConfig" = [&] {
            // NOP
        },
        pattern | _ = [&] {
            throw std::runtime_error("Invalid config type = " + config_type + ", str = " + t.AsString());
        }
    );

    this->ctx.pop(); // 読み取ったトークンは不要なので捨てる
}
