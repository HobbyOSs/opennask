#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "matchit.h"
#include "coffi/coffi.hpp"
#include "front_end.hh"
#include "demangle.hpp"
#include "object_file_writer.hh"

using namespace std::placeholders;
using namespace matchit;

void FrontEnd::visitExportSymStmt(ExportSymStmt *export_sym_stmt) {

    if (export_sym_stmt->listfactor_) export_sym_stmt->listfactor_->accept(this);

    std::vector<TParaToken> symbols;
    size_t size = this->ctx.size();

    for (int i = 0; i < size; i++ ) {
        // stackなので上から順に取得している
        TParaToken t = this->ctx.top();
        symbols.insert(symbols.begin(), t);
        this->ctx.pop();
    }

    for (auto sym : symbols) {
        o_writer_->add_global_symbol(sym.AsString());
        log()->debug("[pass2] symbol {}", sym.AsString());
    }
}

void FrontEnd::visitExternSymStmt(ExternSymStmt *extern_sym_stmt) {

    if (extern_sym_stmt->listfactor_) extern_sym_stmt->listfactor_->accept(this);

    std::vector<TParaToken> symbols;
    size_t size = this->ctx.size();

    for (int i = 0; i < size; i++ ) {
        // stackなので上から順に取得している
        TParaToken t = this->ctx.top();
        symbols.insert(symbols.begin(), t);
        this->ctx.pop();
    }

    for (auto sym : symbols) {
        o_writer_->add_extern_symbol(sym.AsString());
        log()->debug("[pass2] symbol {}", sym.AsString());
    }
}

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
            throw std::runtime_error("[pass2] Invalid bit_mode: " + t.AsString());
        },
        pattern | "FormConfig" | when (t.AsString() == "WCOFF") = [&] {
            o_writer_ = std::make_unique<ObjectFileWriter>(); // TODO: ELFも出したい場合ObjectFileWriterのIFを作って内部で分岐
        },
        pattern | "FileConfig" = [&] {
            auto file_name = t.AsString();
            o_writer_->set_file_name(file_name);
        },
        pattern | "SectConfig" = [&] {
            // TODO: [SECTION .text] 以外の処理にも対応する(?) しかしその場合BNFの構文自体変えたほうが良さそうである
        },
        pattern | "InstConfig" = [&] {
            // NOP
        },
        pattern | _ = [&] {
            throw std::runtime_error("[pass2] Invalid config type = " + config_type + ", str = " + t.AsString());
        }
    );

    this->ctx.pop(); // 読み取ったトークンは不要なので捨てる
}
