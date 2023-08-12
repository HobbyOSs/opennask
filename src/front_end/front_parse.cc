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


FrontEnd::FrontEnd(bool trace_scanning, bool trace_parsing) {

    // spdlog
    if(!spdlog::get("opennask")) {
        auto logger = spdlog::stdout_color_st("opennask");
    }

    // lexer, parser
    this->trace_scanning = trace_scanning;
    this->trace_parsing = trace_parsing;

    // nask
    dollar_position = 0;
    equ_map = std::map<std::string, TParaToken>{};

    //constexpr auto max_size = 65536;
    //binout_container.reserve(max_size);
    //std::memset(binout_container.data(), 0x00, max_size);

    using namespace asmjit;
    Environment env;
    env.setArch(Arch::kX86);
    code_.init(env);
    a_ = std::make_unique<x86::Assembler>(&code_);
}

FrontEnd::~FrontEnd() {
    // メモリの開放
    equ_map.clear();
};

// 以下、抽象クラスの実装(内部で動的に分岐)
void FrontEnd::visitProgram(Program *t) {

    if (dynamic_cast<Prog*>(t) != nullptr) {
        this->visitProg(dynamic_cast<Prog*>(t));
    }
}

void FrontEnd::visitStatement(Statement *t) {

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

void FrontEnd::visitMnemonicArgs(MnemonicArgs *t) {

    if (dynamic_cast<MnemoArg*>(t) != nullptr) {
        this->visitMnemoArg(dynamic_cast<MnemoArg*>(t));
    }
}

void FrontEnd::visitExp(Exp *t) {

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
    } else if (dynamic_cast<DatatypeExp*>(t) != nullptr) {
        this->visitDatatypeExp(dynamic_cast<DatatypeExp*>(t));
    } else if (dynamic_cast<RangeExp*>(t) != nullptr) {
        this->visitRangeExp(dynamic_cast<RangeExp*>(t));
    } else if (dynamic_cast<ImmExp*>(t) != nullptr) {
        this->visitImmExp(dynamic_cast<ImmExp*>(t));
    } else if (dynamic_cast<MemoryAddrExp*>(t) != nullptr) {
        this->visitMemoryAddrExp(dynamic_cast<MemoryAddrExp*>(t));
    }
}

void FrontEnd::visitMemoryAddrExp(MemoryAddrExp *t) {

    if (t->memoryaddr_) t->memoryaddr_->accept(this);
}

void FrontEnd::visitIndexExp(IndexExp *t) {

    if (dynamic_cast<IndexScaleExp*>(t) != nullptr) {
        this->visitIndexScaleExp(dynamic_cast<IndexScaleExp*>(t));
    }
}

void FrontEnd::visitFactor(Factor *t) {

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

void FrontEnd::visitConfigType(ConfigType *t) {
    // NOP
}

void FrontEnd::visitOpcode(Opcode *t) {
    // NOP
}

void FrontEnd::visitDataType(DataType *t) {
    // NOP
}

void FrontEnd::visitProg(Prog *prog) {

    if (prog->liststatement_) {
        prog->liststatement_->accept(this);
    }
}

void FrontEnd::visitLabelStmt(LabelStmt *label_stmt) {
    visitLabel(label_stmt->label_);

    TParaToken t = this->ctx.top();
    this->ctx.pop();
    log()->debug("[pass2] visitLabelStmt: args = {}", t.to_string());
}

void FrontEnd::visitDeclareStmt(DeclareStmt *declare_stmt) {

    visitIdent(declare_stmt->ident_);
    TParaToken key = this->ctx.top();
    this->ctx.pop();

    if (declare_stmt->exp_) {
        declare_stmt->exp_->accept(this);
    }
    TParaToken value = this->ctx.top();
    this->ctx.pop();

    log()->debug("[pass2] declare {} = {}", key.AsString(), value.AsString());
    equ_map[key.AsString()] = value;
}

void FrontEnd::visitMnemonicStmt(MnemonicStmt *mnemonic_stmt){

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

    log()->debug("[pass2] mnemonic_args=[{}]", this->join(debug_args, ","));

    typedef std::function<void(std::vector<TParaToken>&)> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesADD", std::bind(&FrontEnd::processADD, this, _1)),
        std::make_pair("OpcodesAND", std::bind(&FrontEnd::processAND, this, _1)),
        std::make_pair("OpcodesCALL", std::bind(&FrontEnd::processCALL, this, _1)),
        std::make_pair("OpcodesCMP", std::bind(&FrontEnd::processCMP, this, _1)),
        std::make_pair("OpcodesDB", std::bind(&FrontEnd::processDB, this, _1)),
        std::make_pair("OpcodesDW", std::bind(&FrontEnd::processDW, this, _1)),
        std::make_pair("OpcodesDD", std::bind(&FrontEnd::processDD, this, _1)),
        std::make_pair("OpcodesINT", std::bind(&FrontEnd::processINT, this, _1)),
        std::make_pair("OpcodesJAE", std::bind(&FrontEnd::processJAE, this, _1)),
        std::make_pair("OpcodesJB", std::bind(&FrontEnd::processJB, this, _1)),
        std::make_pair("OpcodesJBE", std::bind(&FrontEnd::processJBE, this, _1)),
        std::make_pair("OpcodesJC", std::bind(&FrontEnd::processJC, this, _1)),
        std::make_pair("OpcodesJE", std::bind(&FrontEnd::processJE, this, _1)),
        std::make_pair("OpcodesJMP", std::bind(&FrontEnd::processJMP, this, _1)),
        std::make_pair("OpcodesJNC", std::bind(&FrontEnd::processJNC, this, _1)),
        std::make_pair("OpcodesLGDT", std::bind(&FrontEnd::processLGDT, this, _1)),
        std::make_pair("OpcodesMOV", std::bind(&FrontEnd::processMOV, this, _1)),
        std::make_pair("OpcodesOR", std::bind(&FrontEnd::processOR, this, _1)),
        std::make_pair("OpcodesORG", std::bind(&FrontEnd::processORG, this, _1)),
        std::make_pair("OpcodesOUT", std::bind(&FrontEnd::processOUT, this, _1)),
        std::make_pair("OpcodesRESB", std::bind(&FrontEnd::processRESB, this, _1)),
    };

    const std::string opcode = type(*mnemonic_stmt->opcode_);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second(mnemonic_args);
    } else {
        throw std::runtime_error(opcode + " is not implemented!!!");
    }
}

void FrontEnd::visitOpcodeStmt(OpcodeStmt *opcode_stmt) {
    if (opcode_stmt->opcode_) {
        opcode_stmt->opcode_->accept(this);
    }

    typedef std::function<void()> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesCLI", std::bind(&FrontEnd::processCLI, this)),
        std::make_pair("OpcodesHLT", std::bind(&FrontEnd::processHLT, this)),
        std::make_pair("OpcodesNOP", std::bind(&FrontEnd::processNOP, this)),
    };

    const std::string opcode = type(*opcode_stmt->opcode_);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second();
    } else {
        throw std::runtime_error(opcode + " is not implemented!!!");
    }
}


//
// Visit Opcode系の処理
//
void FrontEnd::visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) {

    for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i) {
        (*i)->accept(this);
    }
}

void FrontEnd::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
}

//
// expressionの処理
//
void FrontEnd::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

template void FrontEnd::visitDataTypes<ByteDataType>(ByteDataType *p);
template void FrontEnd::visitDataTypes<WordDataType>(WordDataType *p);
template void FrontEnd::visitDataTypes<DwordDataType>(DwordDataType *p);

template <class T>
void FrontEnd::visitDataTypes(T *t) {

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

    log()->debug("[pass2] datatype {}", literal);
    this->ctx.push(TParaToken(literal, TParaToken::ttKeyword));
}

void FrontEnd::visitByteDataType(ByteDataType *p) {
    visitDataTypes(p);
}
void FrontEnd::visitWordDataType(WordDataType *p) {
    visitDataTypes(p);
}
void FrontEnd::visitDwordDataType(DwordDataType *p) {
    visitDataTypes(p);
}

void FrontEnd::visitPlusExp(PlusExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitMinusExp(MinusExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitMulExp(MulExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitDivExp(DivExp *p) {
    visitArithmeticOperations(p);
}
void FrontEnd::visitModExp(ModExp *p) {
    visitArithmeticOperations(p);
}

template void FrontEnd::visitArithmeticOperations<PlusExp>(PlusExp *p);
template void FrontEnd::visitArithmeticOperations<MinusExp>(MinusExp *p);
template void FrontEnd::visitArithmeticOperations<MulExp>(MulExp *p);
template void FrontEnd::visitArithmeticOperations<DivExp>(DivExp *p);
template void FrontEnd::visitArithmeticOperations<ModExp>(ModExp *p);


template <class T>
void FrontEnd::visitArithmeticOperations(T *exp) {

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
        ans = left.AsInt32() + right.AsInt32();
    } else if constexpr (std::is_same_v<T, MinusExp>) {
        ans = left.AsInt32() - right.AsInt32();
    } else if constexpr (std::is_same_v<T, MulExp>) {
        ans = left.AsInt32() * right.AsInt32();
    } else if constexpr (std::is_same_v<T, DivExp>) {
        ans = left.AsInt32() / right.AsInt32();
    } else if constexpr (std::is_same_v<T, ModExp>) {
        ans = left.AsInt32() % right.AsInt32();
    } else {
        static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
    }

    TParaToken t = TParaToken(std::to_string(ans), TParaToken::ttInteger);
    this->ctx.push(t);
}

//
// factorの処理
//
void FrontEnd::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttInteger);
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttHex);
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->ident_);
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void FrontEnd::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

//
// tokenの処理
//
void FrontEnd::visitInteger(Integer x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttInteger);
    this->ctx.push(t);
}

void FrontEnd::visitChar(Char x) {
    std::string str{x};
    TParaToken t = TParaToken(str, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitDouble(Double x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttFloating);
    this->ctx.push(t);
}

void FrontEnd::visitString(String x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitIdent(Ident x) {
    if (equ_map.count(x) > 0) {
        // 変数定義があれば展開する
        log()->debug("[pass2] EQU {} = {}", x, equ_map[x].AsString());
        TParaToken t = TParaToken(equ_map[x].AsString(), equ_map[x].AsType());
        this->ctx.push(t);
        return;
    }
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void FrontEnd::visitHex(Hex x) {
    TParaToken t = TParaToken(x, TParaToken::ttHex);
    this->ctx.push(t);
}

template <class T>
std::shared_ptr<T> FrontEnd::Parse(std::istream &input) {

    std::shared_ptr<T> parse_tree = nullptr;
    auto driver = std::make_unique<NaskDriver>();
    // TODO: 開発者向けのデバッグログオプションを作る
    //driver->trace_scanning = this->trace_scanning;
    //driver->trace_parsing = this->trace_parsing;

    try {
        if constexpr (std::is_same_v<T, Program>) {
            parse_tree = driver->pProgram(input);
        }
        else if constexpr (std::is_same_v<T, ListStatement>) {
            parse_tree = driver->pListStatement(input);
        }
        else if constexpr (std::is_same_v<T, Statement>) {
            parse_tree = driver->pStatement(input);
        }
        else if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
            parse_tree = driver->pListMnemonicArgs(input);
        }
        else if constexpr (std::is_same_v<T, MnemonicArgs>) {
            parse_tree = driver->pMnemonicArgs(input);
        }
        else if constexpr (std::is_same_v<T, Exp>) {
            parse_tree = driver->pExp(input);
        }
        else if constexpr (std::is_same_v<T, MemoryAddr>) {
            parse_tree = driver->pMemoryAddr(input);
        }
        else if constexpr (std::is_same_v<T, IndexExp>) {
            parse_tree = driver->pIndexExp(input);
        }
        else if constexpr (std::is_same_v<T, Factor>) {
            parse_tree = driver->pFactor(input);
        }
        else if constexpr (std::is_same_v<T, ConfigType>) {
            parse_tree = driver->pConfigType(input);
        }
        else if constexpr (std::is_same_v<T, DataType>) {
            parse_tree = driver->pDataType(input);
        }
        else if constexpr (std::is_same_v<T, Opcode>) {
            parse_tree = driver->pOpcode(input);
        }
        else {
            static_assert(false_v<T>, "Bad T!!!! Failed to dedution!!!");
        }

    } catch( parse_error &e) {
        std::cerr << "Parse error on line " << e.getLine() << "\n";
    }

    if (parse_tree) {
        printf("\nParse Successful!\n");
        if (trace_scanning) {
            printf("\n[Abstract Syntax]\n");
            auto s = std::make_unique<ShowAbsyn>(ShowAbsyn());
            printf("%s\n\n", s->show(parse_tree.get()));
        }
        if (trace_parsing) {
            printf("[Linearized Tree]\n");
            auto p = std::make_unique<PrintAbsyn>(PrintAbsyn());
            printf("%s\n\n", p->print(parse_tree.get()));
        }
    }
    return parse_tree;
}

template std::shared_ptr<Program> FrontEnd::Parse<Program>(std::istream &input);
template std::shared_ptr<ListStatement> FrontEnd::Parse<ListStatement>(std::istream &input);
template std::shared_ptr<Statement> FrontEnd::Parse<Statement>(std::istream &input);
template std::shared_ptr<ListMnemonicArgs> FrontEnd::Parse<ListMnemonicArgs>(std::istream &input);
template std::shared_ptr<MnemonicArgs> FrontEnd::Parse<MnemonicArgs>(std::istream &input);
template std::shared_ptr<Exp> FrontEnd::Parse<Exp>(std::istream &input);
template std::shared_ptr<MemoryAddr> FrontEnd::Parse<MemoryAddr>(std::istream &input);
template std::shared_ptr<IndexExp> FrontEnd::Parse<IndexExp>(std::istream &input);
template std::shared_ptr<Factor> FrontEnd::Parse<Factor>(std::istream &input);
template std::shared_ptr<ConfigType> FrontEnd::Parse<ConfigType>(std::istream &input);
template std::shared_ptr<DataType> FrontEnd::Parse<DataType>(std::istream &input);
template std::shared_ptr<Opcode> FrontEnd::Parse<Opcode>(std::istream &input);


template <class T>
int FrontEnd::Eval(T *parse_tree, const char* assembly_dst) {

    std::ofstream binout(assembly_dst, std::ios::trunc | std::ios::binary);
    if ( binout.bad() || binout.fail() ) {
        std::cerr << "NASK : can't open " << assembly_dst << std::endl;
        return 17;
    }

    // ここでシンボルテーブル等をpass1からgetする
    auto pass1 = std::make_unique<Pass1Strategy>();
    pass1->Eval(parse_tree);
    sym_table = std::move(pass1->sym_table);

    for (auto entry : sym_table) {
        log()->debug("[pass2] symbol_table [{}] = {}({:x})", entry.first, entry.second, entry.second);
    }

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
    } else if constexpr (std::is_same_v<T, MemoryAddr>) {
        this->visitMemoryAddr(parse_tree);
    } else if constexpr (std::is_same_v<T, IndexExp>) {
        this->visitIndexExp(parse_tree);
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

    using namespace asmjit;
    CodeBuffer& buf = code_.textSection()->buffer();
    // TODO: 互換性のため、しばらくこのようにしてbinout_containerを
    // 使えるようにしておく
    binout_container.assign(buf.data(), buf.data() + buf.size());
    binout.write(reinterpret_cast<char*>(binout_container.data()),
                 binout_container.size());
    //binout.write(reinterpret_cast<char*>(buf.data()), buf.size());
    binout.close();

    return 0;
}

template int FrontEnd::Eval<Program>(Program* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ListStatement>(ListStatement* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Statement>(Statement* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ListMnemonicArgs>(ListMnemonicArgs* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<MnemonicArgs>(MnemonicArgs* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Exp>(Exp* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<MemoryAddr>(MemoryAddr* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<IndexExp>(IndexExp* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Factor>(Factor* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<ConfigType>(ConfigType* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<DataType>(DataType* parse_tree, const char* assembly_dst);
template int FrontEnd::Eval<Opcode>(Opcode* parse_tree, const char* assembly_dst);
