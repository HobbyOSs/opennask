#ifndef DRIVER_HH
#define DRIVER_HH
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "spdlog/spdlog.h"
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
    // 出力するバイナリ情報
    std::vector<uint8_t> binout_container;
    static LABEL_DST_STACK label_dst_stack;
    static LABEL_SRC_STACK label_src_stack;
    // EQUで設定された変数情報
    static std::map<std::string, std::string> equ_map;

    // $ の位置
    uint32_t dollar_position = 0;

public:
    Driver(bool trace_scanning, bool trace_parsing);



    // FILE* f をパースする, 成功時は0を返す
    int Parse(FILE *input, const char* assembly_dst);
    // ASTを評価し結果をファイルに書き込む
    int Eval(Program *parse_tree, const char* assembly_dst);

    bool trace_parsing;
    bool trace_scanning;

    // 以下、Parse/Evalのための関数
    void visitProg(Prog *prog) override;
    void visitLabelStmt(LabelStmt *label_stmt) override;
    void visitLabel(Label x) override;
    void visitDeclareStmt(DeclareStmt *declare_stmt) override;
    void visitMnemonicStmt(MnemonicStmt *mnemonic_stmt) override;
    void visitMnemoArg(MnemoArg *mnemo_arg) override;
    void visitImmExp(ImmExp *p) override;

    // opcodeの読み取り
    void visitOpcodesORG(OpcodesORG *opcodes_org) override;

    // opcodeの処理
    void processORG(ListMnemonicArgs* list_mnemonic_args);
};

#endif // ! DRIVER_HH
