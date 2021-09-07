#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include<vector>
#include<string>
#include "absyn.hh"


Program* pProgram(FILE *inp);
Program* pProgram(const char *str);
ListStatement* pListStatement(FILE *inp);
ListStatement* pListStatement(const char *str);
Statement* pStatement(FILE *inp);
Statement* pStatement(const char *str);
ListMnemonicArgs* pListMnemonicArgs(FILE *inp);
ListMnemonicArgs* pListMnemonicArgs(const char *str);
MnemonicArgs* pMnemonicArgs(FILE *inp);
MnemonicArgs* pMnemonicArgs(const char *str);
Exp* pExp(FILE *inp);
Exp* pExp(const char *str);
Factor* pFactor(FILE *inp);
Factor* pFactor(const char *str);
ConfigType* pConfigType(FILE *inp);
ConfigType* pConfigType(const char *str);
DataType* pDataType(FILE *inp);
DataType* pDataType(const char *str);
Opcode* pOpcode(FILE *inp);
Opcode* pOpcode(const char *str);


#endif
