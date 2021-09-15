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


struct LABEL_DST_ELEMENT {
    std::string label; // ex) entry:
    long src_index;  // JMPのオペコードが始まる場所
    long dst_index;  // JMPの飛び先のラベルが始まる場所
    long rel_index;  // rel_offsetを格納する場所
    long rel_offset() {
        // offset = rel - dst
        spdlog::get("opennask")->info("dst_offs: {} - {} - 1", std::to_string(src_index), std::to_string(rel_index));
        return src_index - rel_index - 1;
    };
};

struct LABEL_SRC_ELEMENT {
    std::string label;   // ex) entry
    OPERAND_KINDS operand;
    bool abs = false;
    long src_index;  // JMPのオペコードが始まる場所
    long dst_index;  // JMPの飛び先のラベルが始まる場所
    long rel_index;  // rel_offsetを格納する場所
    size_t offset_size; // オフセットの格納サイズを指定する
    long rel_offset() {
        // offset = rel - dst
        spdlog::get("opennask")->info("src_offs: {} - {} - 1", std::to_string(dst_index), std::to_string(rel_index));
        return dst_index - rel_index - 1;
    };
};

// 処理の中でlabel情報の収集をする
typedef std::vector<LABEL_DST_ELEMENT> LABEL_DST_STACK;
typedef std::vector<LABEL_SRC_ELEMENT> LABEL_SRC_STACK;


class Driver : public Skeleton {

private:
    // 字句解析, 構文解析のフラグ
    bool trace_parsing;
    bool trace_scanning;

    // 出力するバイナリ情報
    std::vector<uint8_t> binout_container;
    static LABEL_DST_STACK label_dst_stack;
    static LABEL_SRC_STACK label_src_stack;
    // EQUで設定された変数情報
    static std::map<std::string, std::string> equ_map;
    // $ の位置
    uint32_t dollar_position = 0;

public:
    // visitorのcontext情報
    std::stack<TParaToken> ctx;

    Driver(bool trace_scanning, bool trace_parsing);

    // FILE* f / const char* in をパースする, 成功時は0を返す
    template <class T, class IN>
    int Parse(IN input, const char* assembly_dst);

    // ASTを評価し結果をファイルに書き込む
    template <class T>
    int Eval(T* parse_tree, const char* assembly_dst);

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
    void visitOpcodesORG(OpcodesORG *opcodes_org) override;
    void visitOpcodesDB(OpcodesDB *opcodes_db) override;

    // opcodeの処理
    void processDB(std::vector<TParaToken>& memonic_args);
    void processORG(std::vector<TParaToken>& memonic_args);

    // expression
    void visitImmExp(ImmExp *p) override;
    void visitPlusExp(PlusExp *p) override;

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

};

template <typename T>
constexpr bool false_v = false;

#endif // ! DRIVER_HH
