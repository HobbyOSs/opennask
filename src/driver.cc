#include "driver.hh"
#include "parser.hh"
#include <fstream>

Driver::Driver(bool trace_scanning, bool trace_parsing) {

    // spdlog
    auto logger = spdlog::basic_logger_mt("opennask", "debug.log");

    this->trace_scanning = trace_scanning;
    this->trace_parsing = trace_parsing;

    //variables["one"] = 1;
    //variables["two"] = 2;
}

int Driver::Parse(FILE *input) {

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
        delete(parse_tree);
        return 0;
    }
    return 1;
}

int Driver::Eval(const char* assembly_dst) {

    std::ofstream binout(assembly_dst, std::ios::trunc | std::ios::binary);
    if ( binout.bad() || binout.fail() ) {
        std::cerr << "NASK : can't open " << assembly_dst << std::endl;
        return 17;
    }

    // output binary
    binout.write(reinterpret_cast<char*>(binout_container.data()),
                 binout_container.size());
    binout.close();

    return 0;
}
