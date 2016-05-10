/* statementsample.cc */
/* Created by Enomoto Sanshiro on 30 August 1999. */
/* Last updated by Enomoto Sanshiro on 1 September 1999. */


#include <iostream>
#include <cstdlib>
#include "ParaDefs.hh"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaStatement.hh"
#include "ParaStandardLibrary.hh"
#include "ParaMathLibrary.hh"

using namespace std;


int main(int argc, char** argv)
{
    TParaCxxTokenTable TokenTable;
    TParaCxxOperatorTable OperatorTable;
    TParaCxxStatementTable StatementTable;
    
    TParaObjectPrototypeTable ObjectPrototypeTable;
    ObjectPrototypeTable.RegisterClass("Console", new TParaConsoleObject);

    TParaBuiltinFunctionTable BuiltinFunctionTable;
    BuiltinFunctionTable.RegisterAnonymousClass(new TParaMathObject);

    TParaSymbolTable SymbolTable(&ObjectPrototypeTable, &BuiltinFunctionTable);

    TParaExpressionParser ExpressionParser(&OperatorTable);
    TParaStatementParser StatementParser(&StatementTable, &ExpressionParser);

    TParaTokenizer Tokenizer(cin, &TokenTable);

    TParaStatement* Statement = 0;
    try {
        Statement = StatementParser.Parse(&Tokenizer, &SymbolTable);
    }
    catch (TScriptException &e) {
        cerr << "ERROR: " << e << endl;
	delete Statement;

	return EXIT_FAILURE;
    }

    cout << "---" << endl;
      
    try {
	TParaStatement::TExecResult Result = Statement->Execute(&SymbolTable);
        cout << Result.ReturnValue.AsString() << endl;
    }
    catch (TScriptException &e) {
        cerr << "ERROR: " << e << endl;
    }
    
    delete Statement;

    return 0;
}





