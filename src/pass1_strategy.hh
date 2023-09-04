#ifndef PASS1_STRATEGY_HH
#define PASS1_STRATEGY_HH

#include <any>
#include "para_token.hh"
#include "parser.hh"
#include "printer.hh"
#include "absyn.hh"
#include "parsererror.hh"
#include "nask_parse_common.hh"
#include "skeleton.hh"
#include "bin_util.hh"
#include "x86.hh"

class Pass1Strategy : public Skeleton, public BinUtil {

private:
    bool has_extern_symbol(const std::string&);

public:
    Pass1Strategy();
    virtual ~Pass1Strategy();

    // visitorのcontext情報
    std::stack<TParaToken> ctx;

    // LOC(location of counter)
    uint32_t loc = 0;
    OPENNASK_MODES bit_mode = ID_16BIT_MODE;
    // Pass1のシンボルテーブル
    std::map<std::string, uint32_t> sym_table;
    std::vector<std::string> global_symbol_list;
    std::vector<std::string> extern_symbol_list;
    // EQUで設定されたマクロ情報
    std::map<std::string, TParaToken> equ_map;
    // x86命令セット
    std::unique_ptr<x86_64::InstructionSet> iset;
    // オペコード処理のコールバック
    FuncsType funcs;
    void define_funcs();

    // 以下、抽象クラスの実装(内部で動的に分岐)
    void visitProgram(Program *t) override;
    void visitStatement(Statement *t) override;
    void visitMnemonicArgs(MnemonicArgs *t) override;
    void visitExp(Exp *t) override;
    void visitMemoryAddrExp(MemoryAddrExp *t) override;
    void visitJmpMemoryAddrExp(JmpMemoryAddrExp *t) override;
    void visitIndexExp(IndexExp *t) override;
    void visitFactor(Factor *t) override;
    void visitJumpDir(JumpDir *t) override;
    void visitConfigType(ConfigType *t) override;
    void visitDataType(DataType *t) override;
    void visitOpcode(Opcode *t) override;

    // 以下、Parse/Evalのための実装
    void visitProg(Prog *prog) override;
    void visitLabelStmt(LabelStmt *label_stmt) override;
    void visitConfigStmt(ConfigStmt *config_stmt) override;
    void visitDeclareStmt(DeclareStmt *declare_stmt) override;
    void visitExportSymStmt(ExportSymStmt *export_sym_stmt) override;
    void visitExternSymStmt(ExternSymStmt *extern_sym_stmt) override;
    void visitMnemonicStmt(MnemonicStmt *mnemonic_stmt) override;
    void visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) override;
    void visitMnemoArg(MnemoArg *mnemo_arg) override;

    // opcodeの処理
    void processALIGNB(std::vector<TParaToken>& memonic_args);
    void processAND(std::vector<TParaToken>& memonic_args);
    void processCALL(std::vector<TParaToken>& memonic_args);
    void processCLI();
    void processCMP(std::vector<TParaToken>& memonic_args);
    void processDB(std::vector<TParaToken>& memonic_args);
    void processDW(std::vector<TParaToken>& memonic_args);
    void processDD(std::vector<TParaToken>& memonic_args);
    void processHLT();
    void processIMUL(std::vector<TParaToken>& memonic_args);
    void processIN(std::vector<TParaToken>& memonic_args);
    void processINT(std::vector<TParaToken>& memonic_args);
    void processJMP(std::vector<TParaToken>& memonic_args);
    void processLIDT(std::vector<TParaToken>& memonic_args);
    void processLGDT(std::vector<TParaToken>& memonic_args);
    void processMOV(std::vector<TParaToken>& memonic_args);
    void processNOP();
    void processOR(std::vector<TParaToken>& memonic_args);
    void processORG(std::vector<TParaToken>& memonic_args);
    void processOUT(std::vector<TParaToken>& memonic_args);
    void processPOP(std::vector<TParaToken>& memonic_args);
    void processPUSH(std::vector<TParaToken>& memonic_args);
    void processRET(std::vector<TParaToken>& memonic_args);
    void processRESB(std::vector<TParaToken>& memonic_args);
    void processSHR(std::vector<TParaToken>& memonic_args);
    void processSUB(std::vector<TParaToken>& memonic_args);

    // 引数なしオペコードはtemplate化
    template <asmjit::x86::Inst::Id id, int N>
    void processCalc();
    template <asmjit::x86::Inst::Id id>
    void processCalcJcc(std::vector<TParaToken>& memonic_args);
    template <asmjit::x86::Inst::Id id>
    void processCalcAdd(std::vector<TParaToken>& memonic_args);

    // expression
    void visitImmExp(ImmExp *p) override;
    void visitSegmentOffsetExp(SegmentOffsetExp *p) override;
    void visitDirect(Direct *direct) override;
    void visitBasedOrIndexed(BasedOrIndexed *p) override;
    void visitIndexed(Indexed *p) override;
    void visitBasedIndexed(BasedIndexed *p) override;
    void visitBasedIndexedDisp(BasedIndexedDisp *p) override;
    void visitBasedIndexedDispScale(BasedIndexedDispScale *p) override;
    void visitIndexScaleExp(IndexScaleExp *p) override;

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
    void visitCharFactor(CharFactor *p) override;

    // token
    void visitInteger(Integer x) override;
    void visitChar(Char x) override;
    void visitDouble(Double x) override;
    void visitString(String x) override;
    void visitIdent(Ident x) override;
    void visitNaskChar(NaskChar x) override;
    void visitHex(Hex x) override;
    void visitLabel(Label x) override;
    void visitId(Id x) override;

    // ASTを評価しPass1の結果をメンバーに保持する
    template <class T>
    int Eval(T* parse_tree);
};

template <typename T>
constexpr bool false_v = false;

#endif // ! PASS1_STRATEGY_HH
