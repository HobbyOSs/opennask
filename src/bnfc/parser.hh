/* File generated by the BNF Converter (bnfc 2.9.5). */

#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include <vector>
#include <string>
#include <cstdio>
#include "absyn.hh"
#include "bison.hh"

Program* pProgram(std::istream &stream);
ListStatement* pListStatement(std::istream &stream);
Statement* pStatement(std::istream &stream);
ListMnemonicArgs* pListMnemonicArgs(std::istream &stream);
MnemonicArgs* pMnemonicArgs(std::istream &stream);
Exp* pExp(std::istream &stream);
Factor* pFactor(std::istream &stream);
ConfigType* pConfigType(std::istream &stream);
DataType* pDataType(std::istream &stream);
Opcode* pOpcode(std::istream &stream);


#endif
