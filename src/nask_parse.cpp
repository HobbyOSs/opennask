#include <iostream>
#include <memory>
#include <getopt.h>
#include "spdlog/spdlog.h"
#include "front_end.hh"

void usage() {
    printf("Usage:  [--help | --parse | --scan] source \n");
}

int main (int argc, char *argv[]) {
    int res = 0;

    int opt, i, option_index;
    bool trace_parsing = false;
    bool trace_scanning = false;
    char *filename = NULL;

    struct option long_options[] = {
        { "version", no_argument, NULL, 'V' },
        { "parse",   no_argument, NULL, 'p' },
        { "scan",    no_argument, NULL, 's' },
        { "verbose", no_argument, NULL, 'v' },
        { "help",    no_argument, NULL, 'h' },
        { 0, 0, 0, 0 }
    };

    while ((opt = getopt_long(argc, argv, "mes:", long_options, &option_index)) != -1) {

        switch(opt){
        case 'V':
            printf("opennask %s \n", OPENNASK_VERSION);
            printf("Copyright (C) 2021 Hiroyuki Nagata.\n"
                   "ライセンス GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
                   "This is free software: you are free to change and redistribute it.\n"
                   "There is NO WARRANTY, to the extent permitted by law.\n"
                   "\n"
                   "Thank you osask project !\n");
            return 0;
        case 'v':
            spdlog::set_level(spdlog::level::debug);
            break;
        case 'p':
            trace_parsing = true;
            break;
        case 's':
            trace_scanning = true;
            break;
        case 'h':
            usage();
            return 0;
            // 解析できないオプションが見つかった場合は「?」を返す
            // オプション引数が不足している場合も「?」を返す
        case '?':
            printf("unknown or required argument option -%c\n", optopt);
            usage();
            return 1;
        }
    }

    // argv[optind] がオプションでない最初のコマンドラインパラメータ
    const char* assembly_src;
    const char* assembly_dst;
    std::unique_ptr<FrontEnd> d(new FrontEnd(trace_scanning, trace_parsing));

    if (argv[optind+1] != NULL) {
        // ファイルから読み込む場合
        assembly_src = argv[optind];
        assembly_dst = argv[optind + 1];

        std::ifstream input(assembly_src);
        if (! input.good()) {
            std::string out = assembly_src ? std::string(assembly_src) : std::string("");
            std::cerr << "NASK : can't read "
                      << out // srcがNULLの場合がある
                      << std::endl;
            return 17;
        }
        auto parse_tree = d->Parse<Program>(input);
        return d->Eval<Program>(parse_tree.get(), assembly_dst);
    }

    // 標準入力から読み込む場合
    assembly_dst = argv[optind];
    auto parse_tree = d->Parse<Program>(std::cin);
    return d->Eval<Program>(parse_tree.get(), assembly_dst);
}
