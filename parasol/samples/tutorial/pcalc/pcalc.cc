/* pcalc.cc */
/* Created by Enomoto Sanshiro on 27 February 2002. */
/* Last updated by Enomoto Sanshiro on 27 February 2002. */


#include <iostream>
#include <sstream>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"

using namespace std;


int main(int argc, char** argv)
{
    TParaCxxTokenTable TokenTable;
    TParaCxxOperatorTable OperatorTable;
    TParaObjectPrototypeTable ObjectPrototypeTable;
    TParaBuiltinFunctionTable BuiltinFunctionTable;
    TParaSymbolTable SymbolTable(&ObjectPrototypeTable, &BuiltinFunctionTable);
    TParaExpressionParser ExpressionParser(&OperatorTable);

    BuiltinFunctionTable.RegisterAnonymousClass(new TParaMathObject);

    SymbolTable.RegisterVariable("pi", TParaValue(3.141592));
    SymbolTable.RegisterVariable("e", TParaValue(2.718281828));
    SymbolTable.RegisterVariable("x", TParaValue((double) 0));
    
    string Input;
    while (cout << "> ", getline(cin, Input, '\n')) {
        istringstream InputStream(Input);
        TParaTokenizer Tokenizer(InputStream, &TokenTable);

        TParaExpression* Expression = 0;
        try {
            Expression = ExpressionParser.Parse(&Tokenizer, &SymbolTable);

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
