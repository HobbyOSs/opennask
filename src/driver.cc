#include <fstream>
#include <typeinfo>
#include "driver.hh"
#include "parser.hh"
#include "demangle.hpp"

using namespace std::placeholders;

Driver::Driver(bool trace_scanning, bool trace_parsing) {

    // spdlog
    auto logger = spdlog::stdout_logger_mt("opennask", "console");

    this->trace_scanning = trace_scanning;
    this->trace_parsing = trace_parsing;
    this->dollar_position = 0;
}

int Driver::Parse(FILE *input, const char* assembly_dst) {

    /* The default entry point is used. For other options see Parser.H */
    Program *parse_tree = NULL;

    try {
        parse_tree = pProgram(input);
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

        this->Eval(parse_tree, assembly_dst);

        delete(parse_tree);
        return 0;
    }
    return 1;
}

int Driver::Eval(Program *parse_tree, const char* assembly_dst) {

    std::ofstream binout(assembly_dst, std::ios::trunc | std::ios::binary);
    if ( binout.bad() || binout.fail() ) {
        std::cerr << "NASK : can't open " << assembly_dst << std::endl;
        return 17;
    }

    // Eval開始(FIXME: なぜダウンキャストするんだ…?)
    Prog *prog = dynamic_cast<Prog*>(parse_tree);
    this->visitProg(prog);

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

void Driver::visitLabel(Label x) {
    // label: (label_dstと呼ぶ)
    // 1) label_dstの位置を記録する → label_dst_stack
    // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了

    std::string label_dst = x.substr(0, x.find(":", 0));
    log()->debug("coming another label: {} bin[{}]",
                 label_dst, std::to_string(this->binout_container.size()));

    //inst.store_label_dst(label_dst, binout_container);
    //inst.update_label_dst_offset(label_dst, binout_container);
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

void Driver::visitImmExp(ImmExp *imm_exp) {
    if (imm_exp->factor_) {
        imm_exp->factor_->accept(this);
    }
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
