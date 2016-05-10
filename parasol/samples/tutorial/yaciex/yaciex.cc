/* yaciex.cc */
/* Created by Enomoto Sanshiro on 30 August 1999. */
/* Last updated by Enomoto Sanshiro on 24 August 2000. */


#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ParaParser.hh"
#include "BitSet.hh"
#include "BitFunction.hh"
#include "BitOperators.hh"
#include "RepeatUntilStatement.hh"

using namespace std;


class TYaciExParser: public TParaStandardParser {
  public:
    TYaciExParser(int argc, char** argv);
    virtual ~TYaciExParser();
  protected:
    virtual TParaTokenTable* CreateTokenTable(void);
    virtual TParaObjectPrototypeTable* CreateObjectPrototypeTable(void);
    virtual TParaBuiltinFunctionTable* CreateBuiltinFunctionTable(void);
    virtual TParaOperatorTable* CreateOperatorTable(void);
    virtual TParaStatementTable* CreateStatementTable(void);
};



TYaciExParser::TYaciExParser(int argc, char** argv)
: TParaStandardParser(argc, argv)
{
}

TYaciExParser::~TYaciExParser()
{
}

TParaTokenTable* TYaciExParser::CreateTokenTable(void)
{
    TParaTokenTable* TokenTable;
    TokenTable = TParaStandardParser::CreateTokenTable();

    TokenTable->AddOperator("#");
    TokenTable->AddOperator("..");

    TokenTable->AddKeyword("repeat");
    TokenTable->AddKeyword("until");

    return TokenTable;
}

TParaObjectPrototypeTable* TYaciExParser::CreateObjectPrototypeTable(void)
{
    TParaObjectPrototypeTable* ObjectPrototypeTable;
    ObjectPrototypeTable = TParaStandardParser::CreateObjectPrototypeTable();

    ObjectPrototypeTable->RegisterClass("BitSet", new TBitSetObject());

    return ObjectPrototypeTable;
}

TParaBuiltinFunctionTable* TYaciExParser::CreateBuiltinFunctionTable(void)
{
    TParaBuiltinFunctionTable* BuiltinFunctionTable;
    BuiltinFunctionTable = TParaStandardParser::CreateBuiltinFunctionTable();

    BuiltinFunctionTable->RegisterAnonymousClass(new TBitFunction());

    return BuiltinFunctionTable;
}

TParaOperatorTable* TYaciExParser::CreateOperatorTable(void)
{
    TParaOperatorTable* OperatorTable;
    OperatorTable = TParaStandardParser::CreateOperatorTable();

    OperatorTable->AddPrepositionalOperator(new TOperatorBitGenerate());
    OperatorTable->AddOperator(
	new TOperatorBitFill(), TParaOperatorPriority("&", -1)
    );
    
    return OperatorTable;
}

TParaStatementTable* TYaciExParser::CreateStatementTable(void)
{
    TParaStatementTable* StatementTable;
    StatementTable = TParaStandardParser::CreateStatementTable();

    StatementTable->AddStatement(new TRepeatUntilStatement());

    return StatementTable;
}



int main(int argc, char** argv)
{
    if (argc < 2) {
	cerr << "Usage: " << argv[0] << " SourceFileName" << endl;
	return 0;
    }
    ifstream SourceFile(argv[1]);
    if (! SourceFile) {
	cerr << "ERROR: unable to open " << argv[1] << endl;
	return 0;
    }

    TYaciExParser Parser(argc, argv);

    try {
        Parser.Parse(SourceFile);
    }
    catch (TScriptException &e) {
	cerr << "ERROR: " << e << endl;
	return EXIT_FAILURE;
    }

    TParaValue Result;
    try {
	if (Parser.HasEntryOf("main")) {
	    Result = Parser.Execute("main");
	}
	else {
	    Result = Parser.Execute();
	}
	//cout << Result.AsString() << endl;
    }
    catch (TScriptException &e) {
        cerr << "ERROR: " << e << endl;
    }
    
    return Result.AsLong();
}

