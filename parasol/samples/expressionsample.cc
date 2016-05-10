/* expressionsample.cc */
/* Created by Enomoto Sanshiro on 25 August 1999. */
/* Last updated by Enomoto Sanshiro on 25 August 1999. */


#include <iostream>
#include <sstream>
#include <cstdlib>
#include "ParaDefs.hh"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaMathLibrary.hh"
#include "ParaExpression.hh"

using namespace std;


int main(int argc, char** argv)
{
    TParaCxxTokenTable TokenTable;
    TParaCxxOperatorTable OperatorTable;
    TParaExpressionParser ExpressionParser(&OperatorTable);

    TParaObjectPrototypeTable ObjectPrototypeTable;
    TParaBuiltinFunctionTable BuiltinFunctionTable;
    BuiltinFunctionTable.RegisterAnonymousClass(new TParaMathObject);
    TParaSymbolTable SymbolTable(&ObjectPrototypeTable, &BuiltinFunctionTable);

    SymbolTable.RegisterVariable("pi", TParaValue(3.141592));
    SymbolTable.RegisterVariable("e", TParaValue(2.718281828));
    
    string Line;
    while (cout << "expression> ", getline(cin, Line, '\n')) {
        istringstream ExpressionStream(Line);        
        TParaTokenizer Tokenizer(ExpressionStream, &TokenTable);

        TParaExpression* Expression = 0;
        try {
            Expression = ExpressionParser.Parse(&Tokenizer, &SymbolTable);
            Expression->Dump(cerr);
            cerr << "---" << endl;

            TParaValue Value = Expression->Evaluate(&SymbolTable);
            cout << Value.AsString() << endl;
        }
        catch (TScriptException &e) {
            cerr << "ERROR: " << e << endl;
        }
        
        delete Expression;
    }

    cout << endl;
    
    return 0;
}
