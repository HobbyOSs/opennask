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

    if (direct->factor_) direct->factor_->accept(this);
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

    visitIdent(based_or_indexed->ident_);
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();

    visitInteger(based_or_indexed->integer_);
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

    // DataType "[" Exp "]" ; という間接アドレス表現を読み取る
    // left "[" right "]" ; と変数をおいて、属性をTParaTokenに設定する
    if (datatype_exp->datatype_) {
        datatype_exp->datatype_->accept(this);
    }
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttKeyword);
    this->ctx.pop();

    if (datatype_exp->memoryaddr_) {
        datatype_exp->memoryaddr_->accept(this);
    }
    TParaToken right = this->ctx.top();
    this->ctx.pop();

    match(left.AsString())(
        pattern | "BYTE" = [&]{ right.SetAttribute(TParaToken::ttMem8); },
        pattern | "WORD" = [&]{ right.SetAttribute(TParaToken::ttMem16); },
        pattern | "DWORD" = [&]{ right.SetAttribute(TParaToken::ttMem32); },
        pattern | _ = [&] {
            throw std::runtime_error("datatype, Not implemented or not matched!!!");
        }
    );

    this->ctx.push(right);
}
