#include <fstream>
#include "driver.hh"
#include "parser.hh"


Driver::Driver(bool trace_scanning, bool trace_parsing) {

    // spdlog
    auto logger = spdlog::stdout_logger_mt("opennask", "console");

    this->trace_scanning = trace_scanning;
    this->trace_parsing = trace_parsing;
static std::map<std::string, std::string> equ_map;
    //variables["one"] = 1;
    //variables["two"] = 2;
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

    // Eval開始
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
