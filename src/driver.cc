#include <fstream>
#include <typeinfo>
#include <type_traits>
#include "driver.hh"
#include "parser.hh"
#include "demangle.hpp"

using namespace std::placeholders;

Driver::Driver(bool trace_scanning, bool trace_parsing) {

    // spdlog
    if(!spdlog::get("opennask")) {
        auto logger = spdlog::stdout_logger_mt("opennask", "console");
    }

    // lexer, parser
    this->trace_scanning = trace_scanning;
    this->trace_parsing = trace_parsing;

    // nask
    this->dollar_position = 0;
}

// 以下、抽象クラスの実装(内部で動的に分岐)
void Driver::visitProgram(Program *t) {

    // std::visitを使いたいが、難しいのでdynamic_castで
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    if (dynamic_cast<Prog*>(t) != nullptr) {
        this->visitProg(dynamic_cast<Prog*>(t));
    }
}

void Driver::visitStatement(Statement *t) {

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

void Driver::visitMnemonicArgs(MnemonicArgs *t) {

    if (dynamic_cast<MnemoArg*>(t) != nullptr) {
        this->visitMnemoArg(dynamic_cast<MnemoArg*>(t));
    }
}

void Driver::visitExp(Exp *t) {

    if (dynamic_cast<EqExp*>(t) != nullptr) {
        this->visitEqExp(dynamic_cast<EqExp*>(t));
    } else if (dynamic_cast<NeqExp*>(t) != nullptr) {
        this->visitNeqExp(dynamic_cast<NeqExp*>(t));
    } else if (dynamic_cast<LtExp*>(t) != nullptr) {
        this->visitLtExp(dynamic_cast<LtExp*>(t));
    } else if (dynamic_cast<GtExp*>(t) != nullptr) {
        this->visitGtExp(dynamic_cast<GtExp*>(t));
    } else if (dynamic_cast<LteExp*>(t) != nullptr) {
        this->visitLteExp(dynamic_cast<LteExp*>(t));
    } else if (dynamic_cast<GteExp*>(t) != nullptr) {
        this->visitGteExp(dynamic_cast<GteExp*>(t));
    } else if (dynamic_cast<PlusExp*>(t) != nullptr) {
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
    } else if (dynamic_cast<LabelExp*>(t) != nullptr) {
        this->visitLabelExp(dynamic_cast<LabelExp*>(t));
    } else if (dynamic_cast<ImmExp*>(t) != nullptr) {
        this->visitImmExp(dynamic_cast<ImmExp*>(t));
    }
}

void Driver::visitFactor(Factor *t) {

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

void Driver::visitConfigType(ConfigType *t) {
}

void Driver::visitDataType(DataType *t) {
}

void Driver::visitOpcode(Opcode *t) {
}

// テストのため各種の型をエントリーポイントとしてパースできるようにしている
template int Driver::Parse<Program>(FILE* in, char const* dst);
template int Driver::Parse<Program>(const char* in, char const* dst);
template int Driver::Parse<ListStatement>(FILE* in, char const* dst);
template int Driver::Parse<ListStatement>(const char* in, char const* dst);
template int Driver::Parse<Statement>(FILE* in, char const* dst);
template int Driver::Parse<Statement>(const char* in, char const* dst);
template int Driver::Parse<ListMnemonicArgs>(FILE* in, char const* dst);
template int Driver::Parse<ListMnemonicArgs>(const char* in, char const* dst);
template int Driver::Parse<MnemonicArgs>(FILE* in, char const* dst);
template int Driver::Parse<MnemonicArgs>(const char* in, char const* dst);
template int Driver::Parse<Exp>(FILE* in, char const* dst);
template int Driver::Parse<Exp>(const char* in, char const* dst);
template int Driver::Parse<Factor>(FILE* in, char const* dst);
template int Driver::Parse<Factor>(const char* in, char const* dst);
template int Driver::Parse<ConfigType>(FILE* in, char const* dst);
template int Driver::Parse<ConfigType>(const char* in, char const* dst);
template int Driver::Parse<DataType>(FILE* in, char const* dst);
template int Driver::Parse<DataType>(const char* in, char const* dst);
template int Driver::Parse<Opcode>(FILE* in, char const* dst);
template int Driver::Parse<Opcode>(const char* in, char const* dst);


template <class T, class IN>
int Driver::Parse(IN input, const char* assembly_dst) {

    T* parse_tree = NULL;

    try {
        if constexpr (std::is_same_v<T, Program>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pProgram(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psProgram(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, ListStatement>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pListStatement(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psListStatement(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, Statement>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pStatement(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psStatement(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pListMnemonicArgs(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psListMnemonicArgs(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, MnemonicArgs>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pMnemonicArgs(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psMnemonicArgs(input);
            }
        }
        else if constexpr (std::is_same_v<T, Exp>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pExp(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psExp(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, Factor>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pFactor(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psFactor(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, ConfigType>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pConfigType(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psConfigType(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, DataType>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pDataType(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psDataType(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
        }
        else if constexpr (std::is_same_v<T, Opcode>) {
            if constexpr (std::is_same_v<IN, FILE*>) {
                parse_tree = pOpcode(input);
            } else if constexpr (std::is_same_v<IN, const char*>) {
                parse_tree = psOpcode(input);
            } else {
                static_assert(false_v<IN>, "Bad IN!!!! Failed to dedution!!!");
            }
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
            std::unique_ptr<ShowAbsyn> s(new ShowAbsyn());
            printf("%s\n\n", s->show(parse_tree));
        }
        if (trace_parsing) {
            printf("[Linearized Tree]\n");
            std::unique_ptr<PrintAbsyn> p(new PrintAbsyn());
            printf("%s\n\n", p->print(parse_tree));
        }

        this->Eval<T>(parse_tree, assembly_dst);
        log()->debug("parse_tree is: {}", type(parse_tree));

        delete(parse_tree);
        return 0;
    }
    return 1;
}


template <class T>
int Driver::Eval(T *parse_tree, const char* assembly_dst) {

    std::ofstream binout(assembly_dst, std::ios::trunc | std::ios::binary);
    if ( binout.bad() || binout.fail() ) {
        std::cerr << "NASK : can't open " << assembly_dst << std::endl;
        return 17;
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

    // output binary
    binout.write(
        reinterpret_cast<char*>(binout_container.data()), binout_container.size()
    );
    binout.close();

    return 0;
}

void Driver::visitProg(Prog *prog) {

    if (prog->liststatement_) {
        prog->liststatement_->accept(this);
    }
    std::cerr << "visitProg end" << std::endl;
}

void Driver::visitLabelStmt(LabelStmt *label_stmt) {
    visitLabel(label_stmt->label_);
}


void Driver::visitDeclareStmt(DeclareStmt *declare_stmt) {

    log()->debug("visitDeclareStmt start");
    visitIdent(declare_stmt->ident_);
    if (declare_stmt->exp_) {
        declare_stmt->exp_->accept(this);
    }
    log()->debug("visitDeclareStmt end");
}

void Driver::visitMnemonicStmt(MnemonicStmt *mnemonic_stmt){

    log()->debug("visitMnemonicStmt start");
    if (mnemonic_stmt->opcode_) {
        mnemonic_stmt->opcode_->accept(this);
    }
    if (mnemonic_stmt->listmnemonicargs_) {
        mnemonic_stmt->listmnemonicargs_->accept(this);
    }

    std::any c = this->ctx.top();
    std::vector<std::any> mnemonic_args = std::any_cast<std::vector<std::any>>(c);
    this->ctx.pop();

    typedef std::function<void(std::vector<std::any>&)> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesDB", std::bind(&Driver::processDB, this, _1)),
        std::make_pair("OpcodesORG", std::bind(&Driver::processORG, this, _1)),
    };

    const std::string opcode = type(*mnemonic_stmt->opcode_);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second(mnemonic_args);
    } else {
        std::cout << "not implemented..." << std::endl;
    }
    log()->debug("visitMnemonicStmt end");
}

void Driver::processDB(std::vector<std::any>& memonic_args) {
    for (const auto& e : memonic_args) {
        std::cout << "processDB: args " << type(e) << " -> " << std::endl;
    }
}

void Driver::processORG(std::vector<std::any>& memonic_args) {
    for (const auto& e : memonic_args) {
        std::cout << type(e) << std::endl;
    }
}

//
// Visit Opcode系の処理
//
void Driver::visitOpcodesDB(OpcodesDB *opcodes_db) {
    // NOP
}

void Driver::visitOpcodesORG(OpcodesORG *opcodes_org) {
    // NOP
}


void Driver::visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args) {

    std::vector<std::any> cs;
    for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i) {
        (*i)->accept(this);

        std::any c = this->ctx.top();
        this->ctx.pop();
        cs.push_back(c);
    }
    this->ctx.push(cs);
}

void Driver::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
    std::any c = this->ctx.top();
    this->ctx.pop();

    if (c.type() == typeid(int)) {
        this->ctx.push(std::any_cast<int>(c));
    } else if (c.type() == typeid(std::string)) {
        this->ctx.push(std::any_cast<std::string>(c));
    } else {
        // error!
    }
}

//
// expressionの処理
//
void Driver::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    std::any c = this->ctx.top();

    this->ctx.pop();
    if (c.type() == typeid(int)) {
        this->ctx.push(std::any_cast<int>(c));
    } else if (c.type() == typeid(std::string)) {
        this->ctx.push(std::any_cast<std::string>(c));
    } else {
        // error!
    }
}

void Driver::visitPlusExp(PlusExp *plus_exp) {
    if (plus_exp->exp_1) {
        plus_exp->exp_1->accept(this);
    }

    std::any left = this->ctx.top();
    this->ctx.pop();

    if (plus_exp->exp_2) {
        plus_exp->exp_2->accept(this);
    }

    std::any right = this->ctx.top();
    this->ctx.pop();

    if (left.type() == typeid(int) && right.type() == typeid(int)) {
        this->ctx.push(std::any_cast<int>(left) + std::any_cast<int>(right));
    } else {
        // error!
    }
}

//
// factorの処理
//
void Driver::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    std::any c = this->ctx.top();
    this->ctx.pop();
    if (c.type() == typeid(int)) {
        this->ctx.push(std::any_cast<int>(c));
    } else {
        // error!
    }
}

void Driver::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    std::any c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

void Driver::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->ident_);
    std::any c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

void Driver::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    std::any c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

//
// tokenの処理
//
void Driver::visitInteger(Integer x) {
    std::any c = x;
    this->ctx.push(c);
}

void Driver::visitChar(Char x) {
    std::any c = x;
    this->ctx.push(c);
}

void Driver::visitDouble(Double x) {
    std::any c = x;
    this->ctx.push(c);
}

void Driver::visitString(String x) {
    std::any c = x;
    this->ctx.push(c);
}

void Driver::visitIdent(Ident x) {
    std::any c = x;
    this->ctx.push(c);
}

void Driver::visitHex(Hex x) {
    std::any c = x;
    this->ctx.push(c);
}

void Driver::visitLabel(Label x) {
    // label: (label_dstと呼ぶ)
    // 1) label_dstの位置を記録する → label_dst_stack
    // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了

    std::string label_dst = x.substr(0, x.find(":", 0));
    log()->debug("coming another label: {} bin[{}]",
                 label_dst, std::to_string(this->binout_container.size()));

    //inst.store_label_dst(label_dst, binout_container);
    //inst.update_label_dst_offset(label_dst, binout_container);

    std::any c = x;
    this->ctx.push(c);
}
