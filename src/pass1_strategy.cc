#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "pass1_strategy.hh"
#include "matchit.h"
#include "demangle.hpp"
#include "x86.hh"

//using namespace std::placeholders;
using namespace matchit;

std::map<std::string, uint32_t> Pass1Strategy::sym_table = std::map<std::string, uint32_t>{};
std::map<std::string, uint32_t> Pass1Strategy::lit_table = std::map<std::string, uint32_t>{};

Pass1Strategy::Pass1Strategy() {
    // spdlog
    if(!spdlog::get("opennask")) {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    this->loc = 0;
}

Pass1Strategy::~Pass1Strategy() {
    sym_table.clear();
    lit_table.clear();
}

// 以下、抽象クラスの実装(内部で動的に分岐)
void Pass1Strategy::visitProgram(Program *t) {

    if (dynamic_cast<Prog*>(t) != nullptr) {
        this->visitProg(dynamic_cast<Prog*>(t));
    }
}

void Pass1Strategy::visitStatement(Statement *t) {

    if (dynamic_cast<LabelStmt*>(t) != nullptr) {
        this->visitLabelStmt(dynamic_cast<LabelStmt*>(t));
    } else if (dynamic_cast<DeclareStmt*>(t) != nullptr) {
        this->visitDeclareStmt(dynamic_cast<DeclareStmt*>(t));
    } else if (dynamic_cast<ConfigStmt*>(t) != nullptr) {
        this->visitConfigStmt(dynamic_cast<ConfigStmt*>(t));
    } else if (dynamic_cast<MnemonicStmt*>(t) != nullptr) {
        this->visitMnemonicStmt(dynamic_cast<MnemonicStmt*>(t));
    } else if (dynamic_cast<OpcodeStmt*>(t) != nullptr) {
        this->visitOpcodeStmt(dynamic_cast<OpcodeStmt*>(t));
    }
}

void Pass1Strategy::visitMnemonicArgs(MnemonicArgs *t) {

    if (dynamic_cast<MnemoArg*>(t) != nullptr) {
        this->visitMnemoArg(dynamic_cast<MnemoArg*>(t));
    }
}

void Pass1Strategy::visitExp(Exp *t) {

    if (dynamic_cast<PlusExp*>(t) != nullptr) {
        this->visitPlusExp(dynamic_cast<PlusExp*>(t));
    } else if (dynamic_cast<MinusExp*>(t) != nullptr) {
        this->visitMinusExp(dynamic_cast<MinusExp*>(t));
    } else if (dynamic_cast<MulExp*>(t) != nullptr) {
        this->visitMulExp(dynamic_cast<MulExp*>(t));
    } else if (dynamic_cast<DivExp*>(t) != nullptr) {
        this->visitDivExp(dynamic_cast<DivExp*>(t));
    } else if (dynamic_cast<ModExp*>(t) != nullptr) {
        this->visitModExp(dynamic_cast<ModExp*>(t));
    } else if (dynamic_cast<IndirectAddrExp*>(t) != nullptr) {
        this->visitIndirectAddrExp(dynamic_cast<IndirectAddrExp*>(t));
    } else if (dynamic_cast<DatatypeExp*>(t) != nullptr) {
        this->visitDatatypeExp(dynamic_cast<DatatypeExp*>(t));
    } else if (dynamic_cast<RangeExp*>(t) != nullptr) {
        this->visitRangeExp(dynamic_cast<RangeExp*>(t));
    } else if (dynamic_cast<ImmExp*>(t) != nullptr) {
        this->visitImmExp(dynamic_cast<ImmExp*>(t));
    }
}

void Pass1Strategy::visitFactor(Factor *t) {

    if (dynamic_cast<NumberFactor*>(t) != nullptr) {
        this->visitNumberFactor(dynamic_cast<NumberFactor*>(t));
    } else if (dynamic_cast<HexFactor*>(t) != nullptr) {
        this->visitHexFactor(dynamic_cast<HexFactor*>(t));
    } else if (dynamic_cast<IdentFactor*>(t) != nullptr) {
        this->visitIdentFactor(dynamic_cast<IdentFactor*>(t));
    } else if (dynamic_cast<StringFactor*>(t) != nullptr) {
        this->visitStringFactor(dynamic_cast<StringFactor*>(t));
    }
}

void Pass1Strategy::visitConfigType(ConfigType *t) {
    // NOP
}

void Pass1Strategy::visitOpcode(Opcode *t) {
    // NOP
}

void Pass1Strategy::visitDataType(DataType *t) {
    // NOP
}

void Pass1Strategy::visitProg(Prog *prog) {

    if (prog->liststatement_) {
        prog->liststatement_->accept(this);
    }
}

void Pass1Strategy::visitConfigStmt(ConfigStmt *config_stmt) {

    // TODO: 必要な設定を行う
    // [FORMAT "WCOFF"], [FILE "xxxx.c"], [INSTRSET "i386"], [BITS 32]
    if (config_stmt->configtype_) config_stmt->configtype_->accept(this);
    visitString(config_stmt->string_);

    TParaToken t = this->ctx.top();
    this->ctx.pop();
    log()->debug("visitConfigStmt: args = {}", t.to_string());
}

void Pass1Strategy::visitLabelStmt(LabelStmt *label_stmt) {
    visitLabel(label_stmt->label_);

    TParaToken t = this->ctx.top();
    this->ctx.pop();
    log()->debug("visitLabelStmt: args = {}", t.to_string());
}


void Pass1Strategy::visitDeclareStmt(DeclareStmt *declare_stmt) {

    visitIdent(declare_stmt->ident_);
    TParaToken key = this->ctx.top();
    this->ctx.pop();

    if (declare_stmt->exp_) {
        declare_stmt->exp_->accept(this);
    }
    TParaToken value = this->ctx.top();
    this->ctx.pop();

    log()->debug("declare {} = {}", key.AsString(), value.AsString());
    //equ_map[key.AsString()] = value;
}

void Pass1Strategy::visitMnemonicStmt(MnemonicStmt *mnemonic_stmt){

    if (mnemonic_stmt->opcode_) {
        mnemonic_stmt->opcode_->accept(this);
    }
    if (mnemonic_stmt->listmnemonicargs_) {
        mnemonic_stmt->listmnemonicargs_->accept(this);
    }

    std::vector<TParaToken> mnemonic_args;
    size_t size = this->ctx.size();

    for (int i = 0; i < size; i++ ) {
        // stackなので上から順に取得している
        TParaToken t = this->ctx.top();
        mnemonic_args.insert(mnemonic_args.begin(), t);
        this->ctx.pop();
    }

    //std::string debug_str = this->join(mnemonic_args->to_string(), ",");
    std::vector<std::string> debug_args;
    std::transform(mnemonic_args.begin(), mnemonic_args.end(),
                   std::back_inserter(debug_args),
                   [](TParaToken x) { return "{ " + x.to_string() + " }"; });

    log()->debug("mnemonic_args=[{}]", this->join(debug_args, ","));

    // TODO: それぞれのオペコードの場合のPass1の動作を実装する
    const std::string opcode = type(*mnemonic_stmt->opcode_);
    // funcs_type::iterator it = funcs.find(opcode);

    //if (it != funcs.end()) {
    // it->second(mnemonic_args);
    //} else {
    // throw std::runtime_error(opcode + " is not implemented!!!");
    //}
}

void Pass1Strategy::visitOpcodeStmt(OpcodeStmt *opcode_stmt) {
    if (opcode_stmt->opcode_) {
        opcode_stmt->opcode_->accept(this);
    }

    const std::string opcode = type(*opcode_stmt->opcode_);

    // TODO: それぞれのオペコードの場合のPass1の動作を実装する
    //funcs_type::iterator it = funcs.find(opcode);
    //if (it != funcs.end()) {
    //    it->second();
    //} else {
    //    throw std::runtime_error(opcode + " is not implemented!!!");
    //}
}

void Pass1Strategy::processDB(std::vector<TParaToken>& mnemonic_args) {
    // TODO: Pass1の実装
    // TODO: LCを正しいサイズに調整
    // TODO: L := データフィールドのサイズ
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
    for (const auto& e : mnemonic_args) {
        log()->debug("{}", e.to_string());

        if (e.IsInteger() || e.IsHex()) {
            // this->binout_container.push_back(e.AsInt());
        } else if (e.IsIdentifier()) {
            std::string s = e.AsString();
            // std::copy(s.begin(), s.end(), std::back_inserter(binout_container));
        }
    }
}

void Pass1Strategy::processADD(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
    return;
}

void Pass1Strategy::processCLI() {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processCALL(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processCMP(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processDW(std::vector<TParaToken>& mnemonic_args) {
    // TODO: Pass1の実装
    // TODO: LCを正しいサイズに調整
    // TODO: L := データフィールドのサイズ
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
    for (const auto& e : mnemonic_args) {
        log()->debug("{}", e.to_string());

        if (e.IsInteger() || e.IsHex()) {
            uint16_t word = e.AsInt();
            std::vector<uint8_t> bytes = {
                static_cast<uint8_t>( (word >> 8) & 0xff ),
                static_cast<uint8_t>( word & 0xff ),
            };
            // リトルエンディアンなので逆順コピー
            // std::reverse_copy(bytes.begin(), bytes.end(), std::back_inserter(binout_container));

        } else if (e.IsIdentifier()) {
            throw std::runtime_error("not implemented");
        }
    }
}

void Pass1Strategy::processDD(std::vector<TParaToken>& mnemonic_args) {
    // TODO: Pass1の実装
    // TODO: LCを正しいサイズに調整
    // TODO: L := データフィールドのサイズ
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
    for (const auto& e : mnemonic_args) {
        log()->debug("{}", e.to_string());

        if (e.IsInteger() || e.IsHex()) {
            uint32_t dword = e.AsLong();
            std::vector<uint8_t> bytes = {
                static_cast<uint8_t>( (dword >> 24) & 0xff ),
                static_cast<uint8_t>( (dword >> 16) & 0xff ),
                static_cast<uint8_t>( (dword >> 8)  & 0xff ),
                static_cast<uint8_t>( dword & 0xff ),
            };
            // リトルエンディアンなので逆順コピー
            // std::reverse_copy(bytes.begin(), bytes.end(), std::back_inserter(binout_container));

        } else if (e.IsIdentifier()) {
            throw std::runtime_error("not implemented");
        }
    }
}

void Pass1Strategy::processRESB(std::vector<TParaToken>& mnemonic_args) {
    // TODO: Pass1の実装
    // TODO: LCを正しいサイズに調整
    // TODO: L := データフィールドのサイズ
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
    auto arg = mnemonic_args[0];
    const std::string suffix = "-$";

    if (auto range = arg.AsString(); range.find(suffix) != std::string::npos) {
        log()->debug("type: {}, value: {}", type(arg), arg.to_string());
        auto resb_size = range.substr(0, range.length() - suffix.length());
        auto resb_token = TParaToken(resb_size, TParaToken::ttHex);

        // TODO: ここの実装どうするか
        // std::vector<uint8_t> resb(resb_token.AsLong() - dollar_position - binout_container.size(), 0);
        //log()->debug("padding upto: {}(={}), current: {}",
        //             resb_token.AsString(), resb_token.AsLong(), binout_container.size());
        // binout_container.insert(binout_container.end(), std::begin(resb), std::end(resb));
        return;
    }

    arg.MustBe(TParaToken::ttInteger);
    log()->debug("type: {}, value: {}", type(arg), arg.AsLong());

    std::vector<uint8_t> resb(arg.AsLong(), 0);
    // binout_container.insert(binout_container.end(), std::begin(resb), std::end(resb));
}

void Pass1Strategy::processHLT() {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processINT(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processJAE(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processJB(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processJBE(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processJC(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processJE(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processJMP(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processJNC(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processLGDT(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processMOV(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processNOP() {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

void Pass1Strategy::processORG(std::vector<TParaToken>& mnemonic_args) {
    // TODO: LCを変更
    auto arg = mnemonic_args[0];
    arg.MustBe(TParaToken::ttHex);
    log()->debug("type: {}, value: {}", type(arg), arg.AsLong());
    // dollar_position = arg.AsLong();
}

void Pass1Strategy::processOUT(std::vector<TParaToken>& mnemonic_args) {
    // TODO: L := 機械語のサイズ
    // TODO: リテラルテーブルを処理
    // TODO: ラベルが存在する場合, シンボルテーブルのラベルのレコードに現在のLCを設定
    // TODO: LC := LC + L
}

//
// Visit Opcode系の処理
//
void Pass1Strategy::visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) {

    for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i) {
        (*i)->accept(this);
    }
}

void Pass1Strategy::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
}

//
// expressionの処理
//
void Pass1Strategy::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitIndirectAddrExp(IndirectAddrExp *indirect_addr_exp) {
    if (indirect_addr_exp->exp_) {
        indirect_addr_exp->exp_->accept(this);
    }
    // [SI] のような間接アドレス表現を読み取る
    TParaToken t = this->ctx.top();
    t.SetAttribute(TParaToken::ttMem);
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitDatatypeExp(DatatypeExp *datatype_exp) {

    // DataType "[" Exp "]" ; という間接アドレス表現を読み取る
    // left "[" right "]" ; と変数をおいて、属性をTParaTokenに設定する
    if (datatype_exp->datatype_) {
        datatype_exp->datatype_->accept(this);
    }
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttKeyword);
    this->ctx.pop();

    if (datatype_exp->exp_) {
        datatype_exp->exp_->accept(this);
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

template void Pass1Strategy::visitDataTypes<ByteDataType>(ByteDataType *p);
template void Pass1Strategy::visitDataTypes<WordDataType>(WordDataType *p);
template void Pass1Strategy::visitDataTypes<DwordDataType>(DwordDataType *p);

template <class T>
void Pass1Strategy::visitDataTypes(T *t) {

    std::string literal;
    if constexpr (std::is_same_v<T, ByteDataType>) {
        literal = "BYTE";
    } else if constexpr (std::is_same_v<T, WordDataType>) {
        literal = "WORD";
    } else if constexpr (std::is_same_v<T, DwordDataType>) {
        literal = "DWORD";
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    log()->debug("datatype {}", literal);
    this->ctx.push(TParaToken(literal, TParaToken::ttKeyword));
}

void Pass1Strategy::visitByteDataType(ByteDataType *p) {
    visitDataTypes(p);
}
void Pass1Strategy::visitWordDataType(WordDataType *p) {
    visitDataTypes(p);
}
void Pass1Strategy::visitDwordDataType(DwordDataType *p) {
    visitDataTypes(p);
}

void Pass1Strategy::visitPlusExp(PlusExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitMinusExp(MinusExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitMulExp(MulExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitDivExp(DivExp *p) {
    visitArithmeticOperations(p);
}
void Pass1Strategy::visitModExp(ModExp *p) {
    visitArithmeticOperations(p);
}

template void Pass1Strategy::visitArithmeticOperations<PlusExp>(PlusExp *p);
template void Pass1Strategy::visitArithmeticOperations<MinusExp>(MinusExp *p);
template void Pass1Strategy::visitArithmeticOperations<MulExp>(MulExp *p);
template void Pass1Strategy::visitArithmeticOperations<DivExp>(DivExp *p);
template void Pass1Strategy::visitArithmeticOperations<ModExp>(ModExp *p);


template <class T>
void Pass1Strategy::visitArithmeticOperations(T *exp) {

    if (exp->exp_1) {
        exp->exp_1->accept(this);
    }
    TParaToken left = this->ctx.top();
    left.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    if (exp->exp_2) {
        exp->exp_2->accept(this);
    }
    TParaToken right = this->ctx.top();
    right.MustBe(TParaToken::ttInteger);
    this->ctx.pop();

    long ans = 0;
    if constexpr (std::is_same_v<T, PlusExp>) {
        ans = left.AsLong() + right.AsLong();
    } else if constexpr (std::is_same_v<T, MinusExp>) {
        ans = left.AsLong() - right.AsLong();
    } else if constexpr (std::is_same_v<T, MulExp>) {
        ans = left.AsLong() * right.AsLong();
    } else if constexpr (std::is_same_v<T, DivExp>) {
        ans = left.AsLong() / right.AsLong();
    } else if constexpr (std::is_same_v<T, ModExp>) {
        ans = left.AsLong() % right.AsLong();
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    TParaToken t = TParaToken(std::to_string(ans), TParaToken::ttInteger);
    this->ctx.push(t);
}

//
// factorの処理
//
void Pass1Strategy::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttInteger);
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttHex);
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->ident_);
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void Pass1Strategy::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

//
// tokenの処理
//
void Pass1Strategy::visitInteger(Integer x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttInteger);
    this->ctx.push(t);
}

void Pass1Strategy::visitChar(Char x) {
    std::string str{x};
    TParaToken t = TParaToken(str, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Pass1Strategy::visitDouble(Double x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttFloating);
    this->ctx.push(t);
}

void Pass1Strategy::visitString(String x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Pass1Strategy::visitIdent(Ident x) {
    // TODO: ラベルの変数定義を先に作っておく必要がありそう
    // if (equ_map.count(x) > 0) {
    //     // 変数定義があれば展開する
    //     log()->debug("EQU {} = {}", x, equ_map[x].AsString());
    //     TParaToken t = TParaToken(equ_map[x].AsString(), equ_map[x].AsType());
    //     this->ctx.push(t);
    //     return;
    // }
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Pass1Strategy::visitHex(Hex x) {
    TParaToken t = TParaToken(x, TParaToken::ttHex);
    this->ctx.push(t);
}

void Pass1Strategy::visitLabel(Label x) {

    std::string label = x.substr(0, x.find(":", 0));
    // LabelJmp::store_label_dst(label, label_dst_list, binout_container);
    // LabelJmp::update_label_dst_offset(label, label_src_list, dollar_position, binout_container);

    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

template <class T>
int Pass1Strategy::Eval(T *parse_tree) {

    // Eval開始
    if constexpr (std::is_same_v<T, Program>) {
        this->visitProgram(parse_tree);
    } else if constexpr (std::is_same_v<T, ListStatement>) {
        this->visitListStatement(parse_tree);
    } else if constexpr (std::is_same_v<T, Statement>) {
        this->visitStatement(parse_tree);
    } else if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
        this->visitListMnemonicArgs(parse_tree);
    } else if constexpr (std::is_same_v<T, MnemonicArgs>) {
        this->visitMnemonicArgs(parse_tree);
    } else if constexpr (std::is_same_v<T, Exp>) {
        this->visitExp(parse_tree);
    } else if constexpr (std::is_same_v<T, Factor>) {
        this->visitFactor(parse_tree);
    } else if constexpr (std::is_same_v<T, ConfigType>) {
        this->visitConfigType(parse_tree);
    } else if constexpr (std::is_same_v<T, DataType>) {
        this->visitDataType(parse_tree);
    } else if constexpr (std::is_same_v<T, Opcode>) {
        this->visitOpcode(parse_tree);
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    return 0;
}

template int Pass1Strategy::Eval<Program>(Program* pt);
template int Pass1Strategy::Eval<ListStatement>(ListStatement* pt);
template int Pass1Strategy::Eval<Statement>(Statement* pt);
template int Pass1Strategy::Eval<ListMnemonicArgs>(ListMnemonicArgs* pt);
template int Pass1Strategy::Eval<MnemonicArgs>(MnemonicArgs* pt);
template int Pass1Strategy::Eval<Exp>(Exp* pt);
template int Pass1Strategy::Eval<Factor>(Factor* pt);
template int Pass1Strategy::Eval<ConfigType>(ConfigType* pt);
template int Pass1Strategy::Eval<DataType>(DataType* pt);
template int Pass1Strategy::Eval<Opcode>(Opcode* pt);