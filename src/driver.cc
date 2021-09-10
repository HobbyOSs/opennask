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
int Driver::Parse(IN *input, const char* assembly_dst) {

    T* parse_tree = NULL;

    try {
        if constexpr (std::is_same_v<T, Program>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pProgram(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psProgram(input);
            }
        }
        if constexpr (std::is_same_v<T, ListStatement>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pListStatement(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psListStatement(input);
            }
        }
        if constexpr (std::is_same_v<T, Statement>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pStatement(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psStatement(input);
            }
        }
        if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pListMnemonicArgs(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psListMnemonicArgs(input);
            }
        }
        if constexpr (std::is_same_v<T, MnemonicArgs>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pMnemonicArgs(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psMnemonicArgs(input);
            }
        }
        if constexpr (std::is_same_v<T, Exp>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pExp(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psExp(input);
            }
        }
        if constexpr (std::is_same_v<T, Factor>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pFactor(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psFactor(input);
            }
        }
        if constexpr (std::is_same_v<T, ConfigType>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pConfigType(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psConfigType(input);
            }
        }
        if constexpr (std::is_same_v<T, DataType>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pDataType(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psDataType(input);
            }
        }
        if constexpr (std::is_same_v<T, Opcode>) {
            if constexpr (std::is_same_v<IN, FILE>) {
                parse_tree = pOpcode(input);
            } else if constexpr (std::is_same_v<IN, char>) {
                parse_tree = psOpcode(input);
            }
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
    }
    if constexpr (std::is_same_v<T, ListStatement>) {
        this->visitListStatement(parse_tree);
    }
    if constexpr (std::is_same_v<T, Statement>) {
        this->visitStatement(parse_tree);
    }
    if constexpr (std::is_same_v<T, ListMnemonicArgs>) {
        this->visitListMnemonicArgs(parse_tree);
    }
    if constexpr (std::is_same_v<T, MnemonicArgs>) {
        this->visitMnemonicArgs(parse_tree);
    }
    if constexpr (std::is_same_v<T, Exp>) {
        this->visitExp(parse_tree);
    }
    if constexpr (std::is_same_v<T, Factor>) {
        this->visitFactor(parse_tree);
    }
    if constexpr (std::is_same_v<T, ConfigType>) {
        this->visitConfigType(parse_tree);
    }
    if constexpr (std::is_same_v<T, DataType>) {
        this->visitDataType(parse_tree);
    }
    if constexpr (std::is_same_v<T, Opcode>) {
        this->visitOpcode(parse_tree);
    }

    // output binary
    binout.write(
        reinterpret_cast<char*>(binout_container.data()), binout_container.size()
    );
    binout.close();

    return 0;
}

void Driver::visitProg(Prog *prog) {

    log()->debug("visitProg start");
    if (prog->liststatement_) {
        prog->liststatement_->accept(this);
    }
    log()->debug("visitProg end");
}

void Driver::visitLabelStmt(LabelStmt *label_stmt) {
    log()->debug("visitLabelStmt start");
    visitLabel(label_stmt->label_);
    log()->debug("visitLabelStmt end");
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

    typedef std::function<void(ListMnemonicArgs*)> nim_callback;
    typedef std::map<std::string, nim_callback> funcs_type;

    funcs_type funcs {
        std::make_pair("OpcodesORG", std::bind(&Driver::processORG, this, _1)),
    };

    const std::string opcode = type(*mnemonic_stmt->opcode_);

    funcs_type::iterator it = funcs.find(opcode);
    if (it != funcs.end()) {
        it->second(mnemonic_stmt->listmnemonicargs_);
    } else {
        std::cout << "not implemented..." << std::endl;
    }
    log()->debug("visitMnemonicStmt end");
}

void Driver::visitMnemoArg(MnemoArg *mnemo_arg) {
    if (mnemo_arg->exp_) {
        mnemo_arg->exp_->accept(this);
    }
    std::cout << "visitMnemoArg" << mnemo_arg->exp_ << std::endl;
}


void Driver::processORG(ListMnemonicArgs* list_mnemonic_args) {
    std::cout << "hello" << std::endl;

    for (const auto& e : *list_mnemonic_args) {
        std::cout << type(e) << std::endl;
    }
}

//
// Visit Opcode系の処理
//
void Driver::visitOpcodesORG(OpcodesORG *opcodes_org) {
    // NOP
}

//
// expressionの処理
//
void Driver::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
    context c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

void Driver::visitPlusExp(PlusExp *plus_exp) {
    if (plus_exp->exp_1) {
        plus_exp->exp_1->accept(this);
    }

    context left = this->ctx.top();
    this->ctx.pop();

    if (plus_exp->exp_2) {
        plus_exp->exp_2->accept(this);
    }

    context right = this->ctx.top();
    this->ctx.pop();

    context ans = std::visit(
        overload
        {
            [](int arg1, int arg2) -> context { return arg1 + arg2; },
            [](double arg1, double arg2) -> context { return arg1 + arg2; },
            [](auto arg1, auto arg2) -> context{ return 0; },
        },
        left, right
    );

    this->ctx.push(ans);
}

//
// factorの処理
//
void Driver::visitNumberFactor(NumberFactor *number_factor) {
    visitInteger(number_factor->integer_);
    context c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

void Driver::visitHexFactor(HexFactor *hex_factor) {
    visitHex(hex_factor->hex_);
    context c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

void Driver::visitIdentFactor(IdentFactor *ident_factor) {
    visitIdent(ident_factor->ident_);
    context c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

void Driver::visitStringFactor(StringFactor *string_factor) {
    visitString(string_factor->string_);
    context c = this->ctx.top();
    this->ctx.pop();
    this->ctx.push(c);
}

//
// tokenの処理
//
void Driver::visitInteger(Integer x) {
    context c = x;
    this->ctx.push(c);
}

void Driver::visitChar(Char x) {
    context c = x;
    this->ctx.push(c);
}

void Driver::visitDouble(Double x) {
    context c = x;
    this->ctx.push(c);
}

void Driver::visitString(String x) {
    context c = x;
    this->ctx.push(c);
}

void Driver::visitIdent(Ident x) {
    context c = x;
    this->ctx.push(c);
}

void Driver::visitHex(Hex x) {
    context c = x;
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

    context c = x;
    this->ctx.push(c);
}


//struct NumberVisitor {
//    int operator()(int x) { return x; }
//    double operator()(double x) { return x; }
//};
