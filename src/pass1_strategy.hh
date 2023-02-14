#ifndef PASS1_STRATEGY_HH
#define PASS1_STRATEGY_HH

#include <any>
#include "para_token.hh"
#include "parser.hh"
#include "printer.hh"
#include "absyn.hh"
#include "parsererror.hh"
#include "nask_defs.hpp"
#include "skeleton.hh"
#include "bin_util.hh"

class Pass1Strategy : public Skeleton, public BinUtil {

public:
    Pass1Strategy();
    virtual ~Pass1Strategy();

    // visitorのcontext情報
    std::stack<TParaToken> ctx;

    // LOC(location of counter)
    uint32_t loc = 0;
    // Pass1のシンボルテーブル, リテラルテーブル
    static std::map<std::string, uint32_t> sym_table;
    static std::map<std::string, uint32_t> lit_table;

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
    void visitConfigStmt(ConfigStmt *config_stmt) override;
    void visitDeclareStmt(DeclareStmt *declare_stmt) override;
    void visitMnemonicStmt(MnemonicStmt *mnemonic_stmt) override;
    void visitOpcodeStmt(OpcodeStmt *opcode_stmt) override;
    void visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) override;
    void visitMnemoArg(MnemoArg *mnemo_arg) override;

    // opcodeの読み取り(空の実装)
    void visitOpcodesADD(OpcodesADD *opcodes_add) override {};
    void visitOpcodesCALL(OpcodesCALL *opcodes_call) override {};
    void visitOpcodesCLI(OpcodesCLI *opcodes_cli) override {};
    void visitOpcodesCMP(OpcodesCMP *opcodes_cmp) override {};
    void visitOpcodesDB(OpcodesDB *opcodes_db) override {};
    void visitOpcodesDW(OpcodesDW *opcodes_dw) override {};
    void visitOpcodesDD(OpcodesDD *opcodes_dd) override {};
    void visitOpcodesHLT(OpcodesHLT *opcodes_hlt) override {};
    void visitOpcodesINT(OpcodesINT *opcodes_int) override {};
    void visitOpcodesJAE(OpcodesJAE *opcodes_jae) override {};
    void visitOpcodesJB(OpcodesJB *opcodes_jb) override {};
    void visitOpcodesJBE(OpcodesJBE *opcodes_jbe) override {};
    void visitOpcodesJC(OpcodesJC *opcodes_jc) override {};
    void visitOpcodesJE(OpcodesJE *opcodes_je) override {};
    void visitOpcodesJNC(OpcodesJNC *opcodes_jnc) override {};
    void visitOpcodesJMP(OpcodesJMP *opcodes_jmp) override {};
    void visitOpcodesLGDT(OpcodesLGDT *opcodes_lgdt) override {};
    void visitOpcodesMOV(OpcodesMOV *opcodes_mov) override {};
    void visitOpcodesNOP(OpcodesNOP *opcodes_nop) override {};
    void visitOpcodesORG(OpcodesORG *opcodes_org) override {};
    void visitOpcodesOUT(OpcodesOUT *opcodes_out) override {};
    void visitOpcodesRESB(OpcodesRESB *opcodes_resb) override {};

    // opcodeの処理
    void processADD(std::vector<TParaToken>& memonic_args);
    void processCALL(std::vector<TParaToken>& memonic_args);
    void processCLI();
    void processCMP(std::vector<TParaToken>& memonic_args);
    void processDB(std::vector<TParaToken>& memonic_args);
    void processDW(std::vector<TParaToken>& memonic_args);
    void processDD(std::vector<TParaToken>& memonic_args);
    void processHLT();
    void processINT(std::vector<TParaToken>& memonic_args);
    void processJAE(std::vector<TParaToken>& memonic_args);
    void processJB(std::vector<TParaToken>& memonic_args);
    void processJBE(std::vector<TParaToken>& memonic_args);
    void processJC(std::vector<TParaToken>& memonic_args);
    void processJE(std::vector<TParaToken>& memonic_args);
    void processJNC(std::vector<TParaToken>& memonic_args);
    void processJMP(std::vector<TParaToken>& memonic_args);
    void processLGDT(std::vector<TParaToken>& memonic_args);
    void processMOV(std::vector<TParaToken>& memonic_args);
    void processNOP();
    void processORG(std::vector<TParaToken>& memonic_args);
    void processOUT(std::vector<TParaToken>& memonic_args);
    void processRESB(std::vector<TParaToken>& memonic_args);

    // expression
    void visitImmExp(ImmExp *p) override;
    void visitIndirectAddrExp(IndirectAddrExp *p) override;

    void visitDatatypeExp(DatatypeExp *p) override;
    void visitByteDataType(ByteDataType *p) override;
    void visitWordDataType(WordDataType *p) override;
    void visitDwordDataType(DwordDataType *p) override;
    template <class T>
    void visitDataTypes(T *t);

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

    // ASTを評価しPass1の結果をメンバーに保持する
    template <class T>
    int Eval(T* parse_tree);
};

template <typename T>
constexpr bool false_v = false;

#endif // ! PASS1_STRATEGY_HH
