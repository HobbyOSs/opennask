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


void FrontEnd::visitDirect(Direct *direct) {

    if (direct->exp_) direct->exp_->accept(this);
    using namespace asmjit;

    TParaToken t = this->ctx.top();
    log()->debug("[pass2] visitDirect [{}]", t.AsString());

    match(t)(
        pattern | _ | when(t.IsAsmJitGpbLo()) = [&] {
            t.SetAttribute(TParaToken::ttMem8);
            t.SetMem(x86::ptr(t.AsAsmJitGpbLo()));
        },
        pattern | _ | when(t.IsAsmJitGpbHi()) = [&] {
            t.SetAttribute(TParaToken::ttMem8);
            t.SetMem(x86::ptr(t.AsAsmJitGpbHi()));
        },
        pattern | _ | when(t.IsAsmJitGpw()) = [&] {
            t.SetAttribute(TParaToken::ttMem16);
            t.SetMem(x86::ptr(t.AsAsmJitGpw()));
        },
        pattern | _ | when(t.IsAsmJitSReg()) = [&] {
            t.SetAttribute(TParaToken::ttMem16);
            //t.SetMem(x86::ptr(t.AsAsmJitSReg())); TODO: コンパイルできない
        },
        pattern | _ | when(t.IsAsmJitGpd()) = [&] {
            t.SetAttribute(TParaToken::ttMem32);
            t.SetMem(x86::ptr(t.AsAsmJitGpd()));
        },
        pattern | _ | when(t.IsImmediate() && t.GetImmSize() == 1) = [&] {
            auto mem = x86::Mem();
            mem.setOffset(t.AsInt32());
            t.SetAttribute(TParaToken::ttMem8);
            t.SetMem(mem);
        },
        pattern | _ | when(t.IsImmediate() && t.GetImmSize() == 2) = [&] {
            auto mem = x86::Mem();
            mem.setOffset(t.AsInt32());
            t.SetAttribute(TParaToken::ttMem16);
            t.SetMem(mem);
        },
        pattern | _ | when(t.IsImmediate() && t.GetImmSize() == 4) = [&] {
            auto mem = x86::Mem();
            mem.setOffset(t.AsInt32());
            t.SetAttribute(TParaToken::ttMem32);
            t.SetMem(mem);
        }
    );

    this->ctx.pop();
    this->ctx.push(t);
};

void FrontEnd::visitBasedOrIndexed(BasedOrIndexed *based_or_indexed) {

    visitIdent(based_or_indexed->id_);
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();

    if (based_or_indexed->exp_) based_or_indexed->exp_->accept(this);
    TParaToken right = this->ctx.top();
    right.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    using namespace asmjit;

    log()->debug("[pass2] visitBasedOrIndexed [{} + {}]",
                 left.AsString(), right.AsString());

    match(left)(
        pattern | _ | when(left.IsAsmJitGpbLo()) = [&] {
            left.SetAttribute(TParaToken::ttMem8);
            left.SetMem(x86::ptr(left.AsAsmJitGpbLo(), right.AsInt32()));
        },
        pattern | _ | when(left.IsAsmJitGpbHi()) = [&] {
            left.SetAttribute(TParaToken::ttMem8);
            left.SetMem(x86::ptr(left.AsAsmJitGpbHi(), right.AsInt32()));
        },
        pattern | _ | when(left.IsAsmJitGpw()) = [&] {
            left.SetAttribute(TParaToken::ttMem16);
            left.SetMem(x86::ptr(left.AsAsmJitGpw(), right.AsInt32()));
        },
        pattern | _ | when(left.IsAsmJitSReg()) = [&] {
            left.SetAttribute(TParaToken::ttMem16);
            //left.SetMem(x86::ptr(left.AsAsmJitSReg(), right.AsInt32())); TODO: コンパイルできない
        },
        pattern | _ | when(left.IsAsmJitGpd()) = [&] {
            left.SetAttribute(TParaToken::ttMem32);
            left.SetMem(x86::ptr(left.AsAsmJitGpd(), right.AsInt32()));
        }
    );

    this->ctx.push(left);
};

void FrontEnd::visitIndexed(Indexed *p) {};
void FrontEnd::visitBasedIndexed(BasedIndexed *p) {};
void FrontEnd::visitBasedIndexedDisp(BasedIndexedDisp *p) {};
void FrontEnd::visitBasedIndexedDispScale(BasedIndexedDispScale *p) {};
void FrontEnd::visitIndexScaleExp(IndexScaleExp *p) {};

void FrontEnd::visitDatatypeExp(DatatypeExp *datatype_exp) {
    if (datatype_exp->datatype_) datatype_exp->datatype_->accept(this);
    if (datatype_exp->memoryaddr_) datatype_exp->memoryaddr_->accept(this);

    // DataType "[" Exp "]" ; というメモリーアドレス表現を読み取る
    // メモリーアドレス表現に従ってasmjit::Memに {byte,word,dword}_ptr を設定しないといけない
    TParaToken memory_addr = this->ctx.top();
    this->ctx.pop();
    TParaToken data_type = this->ctx.top();
    this->ctx.pop();

    using namespace matchit;

    match(data_type.AsString())(
        pattern | "BYTE"  = [&]{
            memory_addr.AsMem().setSize(1);
            memory_addr.SetAttribute(TParaToken::ttMem8);
        },
        pattern | "WORD"  = [&]{
            memory_addr.AsMem().setSize(2);
            memory_addr.SetAttribute(TParaToken::ttMem16);
        },
        pattern | "DWORD" = [&]{
            memory_addr.AsMem().setSize(4);
            memory_addr.SetAttribute(TParaToken::ttMem32);
        },
        pattern | _ = [&] {
            throw std::runtime_error("[pass2] data type is invalid");
        }
    );
    this->ctx.push(memory_addr);
}
