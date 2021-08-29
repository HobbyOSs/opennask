#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include<vector>
#include<string>
#include "absyn.hh"


Program* pProgram(FILE *inp);
Program* pProgram(const char *str);


#endif
