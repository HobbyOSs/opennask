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

        this->Delete<T>(parse_tree);

        return 0;
    }
    return 1;
}


template <class T>
void Driver::Delete(T *pt) {

    if (auto prog = dynamic_cast<Prog*>(pt); prog != nullptr) {

        log()->debug("success cast {}", type(prog));
        for (auto stmt : *prog->liststatement_) {
            log()->debug("iterate under prog {}", type(stmt));
            this->Delete<Statement>(stmt);
        }
        delete(prog->liststatement_); //deleteできない
        delete(prog);

    } else if (auto stmt = dynamic_cast<Statement*>(pt); stmt != nullptr) {

        log()->debug("success cast {}", type(stmt));

        if (auto t = dynamic_cast<LabelStmt*>(stmt); t != nullptr) {
            // NOP
        } else if (auto t = dynamic_cast<DeclareStmt*>(stmt); t != nullptr) {
            // NOP
        } else if (auto t = dynamic_cast<ConfigStmt*>(stmt); t != nullptr) {
            // NOP
        } else if (auto t = dynamic_cast<MnemonicStmt*>(stmt); t != nullptr) {
            log()->debug("success cast {}", type(t));

            delete(t->opcode_);
            log()->debug("delete {}", type(t->opcode_));
            for (auto arg : *t->listmnemonicargs_) {
                log()->debug("iterate under mnemonic_stmt {}", type(arg));
                this->Delete<MnemonicArgs>(arg);
                delete(arg);
            }
            log()->debug("delete {}", type(t->listmnemonicargs_));
            delete(t->listmnemonicargs_);
            log()->debug("delete {}", type(t));
            delete(t);

        } else if (auto t = dynamic_cast<OpcodeStmt*>(stmt); t != nullptr) {
            log()->debug("success cast {}", type(t));
            delete(t->opcode_);
            log()->debug("delete {}", type(t->opcode_));
        }

    } else if (auto args = dynamic_cast<MnemonicArgs*>(pt); args != nullptr) {

        if (auto arg = dynamic_cast<MnemoArg*>(args); arg != nullptr) {
            log()->debug("success cast {}", type(arg));

            this->Delete<Exp>(arg->exp_);

            log()->debug("delete {}", type(arg->exp_));
            delete(arg->exp_);
        }
    } else if (auto exp = dynamic_cast<Exp*>(pt); exp != nullptr) {

        log()->debug("success cast {}", type(exp));

        if (auto plus_exp = dynamic_cast<PlusExp*>(exp); plus_exp != nullptr) {
        } else if (dynamic_cast<MinusExp*>(exp) != nullptr) {
        } else if (dynamic_cast<MulExp*>(exp) != nullptr) {
        } else if (dynamic_cast<DivExp*>(exp) != nullptr) {
        } else if (dynamic_cast<ModExp*>(exp) != nullptr) {
        } else if (dynamic_cast<IndirectAddrExp*>(exp) != nullptr) {
        } else if (dynamic_cast<DatatypeExp*>(exp) != nullptr) {
        } else if (dynamic_cast<RangeExp*>(exp) != nullptr) {
        } else if (dynamic_cast<LabelExp*>(exp) != nullptr) {
        } else if (auto imm_exp = dynamic_cast<ImmExp*>(exp); imm_exp != nullptr) {
            log()->debug("success cast {}", type(imm_exp));

            this->Delete<Factor>(imm_exp->factor_);

            log()->debug("delete {}", type(imm_exp->factor_));
            delete(imm_exp->factor_);
        }
    } else if (auto factor = dynamic_cast<Factor*>(pt); factor != nullptr) {

        log()->debug("success cast {}", type(factor));

        if (auto f = dynamic_cast<NumberFactor*>(factor); f != nullptr) {
            log()->debug("success cast {}", type(f));
        } else if (auto f = dynamic_cast<HexFactor*>(factor); f != nullptr) {
            log()->debug("success cast {}", type(f));
        } else if (auto f = dynamic_cast<IdentFactor*>(factor); f != nullptr) {
            log()->debug("success cast {}", type(f));

        } else if (auto f = dynamic_cast<StringFactor*>(factor); f != nullptr) {
            log()->debug("success cast {}", type(f));
        }
    }

    return;
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

    if (mnemonic_stmt->opcode_) {
        mnemonic_stmt->opcode_->accept(this);
    }
    if (mnemonic_stmt->listmnemonicargs_) {
        mnemonic_stmt->listmnemonicargs_->accept(this);
    }

    std::vector<TParaToken> mnemonic_args;
    size_t size = this->ctx.size();
    mnemonic_args.reserve(size);

    for (int i = 0; i < size; i++ ) {
        TParaToken t = this->ctx.top();
        mnemonic_args.push_back(t);
        this->ctx.pop();
    }

    std::reverse(mnemonic_args.begin(), mnemonic_args.end());
    std::string debug_str = this->join(mnemonic_args, ",");
    log()->debug("visitMnemonicStmt: result {}", debug_str);

    typedef std::function<void(std::vector<TParaToken>&)> nim_callback;
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
}

void Driver::processDB(std::vector<TParaToken>& memonic_args) {
    for (const auto& e : memonic_args) {
        if (e.IsInteger() || e.IsHex()) {
            log()->info("type: {}, value: {}", type(e), e.AsInt());
            this->binout_container.push_back(e.AsInt());
        }
    }
}

void Driver::processORG(std::vector<TParaToken>& memonic_args) {
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

    for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i) {
        (*i)->accept(this);
    }
}

void Driver::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

//
// expressionの処理
//
void Driver::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    TParaToken t = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitPlusExp(PlusExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitMinusExp(MinusExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitMulExp(MulExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitDivExp(DivExp *p) {
    visitArithmeticOperations(p);
}
void Driver::visitModExp(ModExp *p) {
    visitArithmeticOperations(p);
}

template void Driver::visitArithmeticOperations<PlusExp>(PlusExp *p);
template void Driver::visitArithmeticOperations<MinusExp>(MinusExp *p);
template void Driver::visitArithmeticOperations<MulExp>(MulExp *p);
template void Driver::visitArithmeticOperations<DivExp>(DivExp *p);
template void Driver::visitArithmeticOperations<ModExp>(ModExp *p);


template <class T>
void Driver::visitArithmeticOperations(T *exp) {

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
void Driver::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttInteger);
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttHex);
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->ident_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

void Driver::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    TParaToken t = this->ctx.top();
    t.MustBe(TParaToken::ttIdentifier);
    this->ctx.pop();
    this->ctx.push(t);
}

//
// tokenの処理
//
void Driver::visitInteger(Integer x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttInteger);
    this->ctx.push(t);
}

void Driver::visitChar(Char x) {
    std::string str{x};
    TParaToken t = TParaToken(str, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Driver::visitDouble(Double x) {
    TParaToken t = TParaToken(std::to_string(x), TParaToken::ttFloating);
    this->ctx.push(t);
}

void Driver::visitString(String x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Driver::visitIdent(Ident x) {
    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

void Driver::visitHex(Hex x) {
    TParaToken t = TParaToken(x, TParaToken::ttHex);
    this->ctx.push(t);
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

    TParaToken t = TParaToken(x, TParaToken::ttIdentifier);
    this->ctx.push(t);
}

const std::string Driver::join(std::vector<TParaToken>& array, const std::string& sep) {
    std::stringstream ss;
    ss << std::hex
       << std::setw(2)
       << std::setfill('0');

    for(size_t i = 0; i < array.size(); ++i) {
        if(i != 0) {
            ss << sep;
        }
        ss << array[i].AsInt();
    }
    return ss.str();
}
