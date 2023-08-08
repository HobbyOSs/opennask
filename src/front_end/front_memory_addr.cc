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

void FrontEnd::visitBasedOrIndexed(BasedOrIndexed *p) {

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
