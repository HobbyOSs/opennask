#include <iostream>
#include <memory>
#include <getopt.h>
#include "spdlog/spdlog.h"
#include "driver.hh"

void usage() {
    printf("Usage:  [--help | --parse | --scan] source \n");
}

int main (int argc, char *argv[]) {
    int res = 0;

    int opt, i, option_index;
    bool trace_parsing = false;
    bool trace_scanning = false;
    FILE *input;
    char *filename = NULL;

    struct option long_options[] = {
        { "parse",   no_argument, NULL, 'p' },
        { "scan",    no_argument, NULL, 's' },
        { "verbose", no_argument, NULL, 'v' },
        { "help",    no_argument, NULL, 'h' },
        { 0, 0, 0, 0 }
    };

    while ((opt = getopt_long(argc, argv, "mes:", long_options, &option_index)) != -1) {

        switch(opt){
        case 'V':
            printf("naskparse \n");
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

    filename = argv[optind];
    if (filename) {
        input = fopen(filename, "r");
        if (!input) {
            usage();
            return 1;
        }
    } else input = stdin;

    std::unique_ptr<Driver> d(new Driver(trace_scanning, trace_parsing));
    return d->Parse(input);
}
