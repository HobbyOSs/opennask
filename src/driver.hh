#ifndef DRIVER_HH
#define DRIVER_HH
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "spdlog/spdlog.h"
#include "parser.hh"
#include "printer.hh"
#include "absyn.hh"
#include "parsererror.hh"


class Driver {

private:
    // 出力するバイナリ情報
    std::vector<uint8_t> binout_container;

public:
    Driver(bool trace_scanning, bool trace_parsing);


    std::map<std::string, int> variables;

    // FILE* f をパースする, 成功時は0を返す
    int Parse(FILE *input);
    // ASTを評価し結果をファイルに書き込む
    int Eval(const char* assembly_dst);

    bool trace_parsing;
    bool trace_scanning;
};

#endif // ! DRIVER_HH
