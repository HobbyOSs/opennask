#ifndef DRIVER_HH
#define DRIVER_HH
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "parser.hh"
#include "printer.hh"
#include "absyn.hh"
#include "parsererror.hh"


class Driver {

public:
    Driver(bool trace_scanning, bool trace_parsing);

    std::map<std::string, int> variables;

    // FILE* f をパースする, 成功時は0を返す
    int Parse(FILE *input);

    bool trace_parsing;
    bool trace_scanning;
};

#endif // ! DRIVER_HH
