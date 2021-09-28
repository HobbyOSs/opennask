#ifndef DRIVER_HH
#define DRIVER_HH

#include <string>
#include <memory>
#include <iostream>
#include <any>
#include "spdlog/spdlog.h"
#include "para_token.hh"
#include "parser.hh"
#include "printer.hh"
#include "absyn.hh"
#include "parsererror.hh"
#include "nask_defs.hpp"
#include "skeleton.hh"


struct LabelCalc {
    std::string label;   // ex) entry
    size_t src_index;
    size_t dst_index;

    size_t get_offset() {
        return dst_index - src_index;
    };
};

class Driver : public Skeleton {

private:
    // 字句解析, 構文解析のフラグ
    bool trace_parsing;
    bool trace_scanning;
    // ラベルによるオフセットの計算をする
    std::stack<LabelCalc> label_calc_stack;
    // EQUで設定された変数情報
    static std::map<std::string, std::string> equ_map;
    // $ の位置
    uint32_t dollar_position = 0;

public:
    // visitorのcontext情報
    std::stack<TParaToken> ctx;
    // 出力するバイナリ情報
    std::vector<uint8_t> binout_container;

    Driver(bool trace_scanning, bool trace_parsing);

    // FILE* f / const char* in をパースする, 成功時は0を返す
    template <class T, class IN>
    int Parse(IN input, const char* assembly_dst);

    // ASTを評価し結果をファイルに書き込む
    template <class T>
    int Eval(T* parse_tree, const char* assembly_dst);

    template <class T>
    void Delete(T* parse_tree);

    // 以下、抽象クラスの実装(内部で動的に分岐)
    void visitProgram(Program *t) override;
    void visitStatement(Statement *t) override;
    void visitMnemonicArgs(MnemonicArgs *t) override;
    void visitExp(Exp *t) override;
    void visitFactor(Factor *t) override;
    void visitConfigType(ConfigType *t) override;
    void visitDataType(DataType *t) override;
    void visitOpcode(Opcode *t) override;

    // 以下、Parse/Evalのための実装
    void visitProg(Prog *prog) override;
    void visitLabelStmt(LabelStmt *label_stmt) override;
    void visitDeclareStmt(DeclareStmt *declare_stmt) override;
    void visitMnemonicStmt(MnemonicStmt *mnemonic_stmt) override;
    void visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) override;
    void visitMnemoArg(MnemoArg *mnemo_arg) override;

    // opcodeの読み取り
    void visitOpcodesADD(OpcodesADD *opcodes_add) override;
    void visitOpcodesDB(OpcodesDB *opcodes_db) override;
    void visitOpcodesDW(OpcodesDW *opcodes_dw) override;
    void visitOpcodesDD(OpcodesDD *opcodes_dd) override;
    void visitOpcodesJMP(OpcodesJMP *opcodes_jmp) override;
    void visitOpcodesMOV(OpcodesMOV *opcodes_mov) override;
    void visitOpcodesORG(OpcodesORG *opcodes_org) override;
    void visitOpcodesRESB(OpcodesRESB *opcodes_resb) override;

    // opcodeの処理
    void processADD(std::vector<TParaToken>& memonic_args);
    void processDB(std::vector<TParaToken>& memonic_args);
    void processDW(std::vector<TParaToken>& memonic_args);
    void processDD(std::vector<TParaToken>& memonic_args);
    void processJMP(std::vector<TParaToken>& memonic_args);
    void processMOV(std::vector<TParaToken>& memonic_args);
    void processORG(std::vector<TParaToken>& memonic_args);
    void processRESB(std::vector<TParaToken>& memonic_args);

    // expression
    void visitImmExp(ImmExp *p) override;
    void visitIndirectAddrExp(IndirectAddrExp *p) override;
    void visitPlusExp(PlusExp *p) override;
    void visitMinusExp(MinusExp *p) override;
    void visitMulExp(MulExp *p) override;
    void visitDivExp(DivExp *p) override;
    void visitModExp(ModExp *p) override;
    template <class T>
    void visitArithmeticOperations(T *t);

    // factor
    void visitNumberFactor(NumberFactor *p) override;
    void visitHexFactor(HexFactor *p) override;
    void visitIdentFactor(IdentFactor *p) override;
    void visitStringFactor(StringFactor *p) override;

    // token
    void visitInteger(Integer x) override;
    void visitChar(Char x) override;
    void visitDouble(Double x) override;
    void visitString(String x) override;
    void visitIdent(Ident x) override;
    void visitHex(Hex x) override;
    void visitLabel(Label x) override;

    // 以下、整理後にstring_utilに移動
    const std::string join(std::vector<TParaToken>& array, const std::string& sep = "");

};

template <typename T>
constexpr bool false_v = false;

#endif // ! DRIVER_HH
