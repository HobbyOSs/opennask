#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include<vector>
#include<string>
#include "absyn.hh"


Program* pProgram(FILE *inp);
Program* psProgram(const char *str);
ListStatement* pListStatement(FILE *inp);
ListStatement* psListStatement(const char *str);
Statement* pStatement(FILE *inp);
Statement* psStatement(const char *str);
ListMnemonicArgs* pListMnemonicArgs(FILE *inp);
ListMnemonicArgs* psListMnemonicArgs(const char *str);
MnemonicArgs* pMnemonicArgs(FILE *inp);
MnemonicArgs* psMnemonicArgs(const char *str);
Exp* pExp(FILE *inp);
Exp* psExp(const char *str);
Factor* pFactor(FILE *inp);
Factor* psFactor(const char *str);
ConfigType* pConfigType(FILE *inp);
ConfigType* psConfigType(const char *str);
DataType* pDataType(FILE *inp);
DataType* psDataType(const char *str);
Opcode* pOpcode(FILE *inp);
Opcode* psOpcode(const char *str);


#endif
