/* psh.cc */
/* Created by Enomoto Sanshiro on 27 February 2002. */
/* Last updated by Enomoto Sanshiro on 27 February 2002. */


#include <iostream>
#include "ParaParser.hh"
#include "MacroDrawParser.hh"

using namespace std;


int main(void)
{
    TCanvas Canvas;
    TMacroDrawParser Parser(&Canvas);

    TParaTokenTable* TokenTable = Parser.GetTokenTable();
    TParaSymbolTable* SymbolTable = Parser.GetSymbolTable();
    TParaStatementParser* StatementParser = Parser.GetStatementParser();

    TParaTokenizer Tokenizer(cin, TokenTable);

    TParaValue Result;
    while (cout << "> " << flush, ! Tokenizer.LookAhead().IsEmpty()) {
	TParaStatement* Statement = 0;
	try {
	    Statement = StatementParser->Parse(&Tokenizer, SymbolTable);
	}
	catch (TScriptException &e) {
	    cerr << "ERROR: " << e << endl;
	    continue;
	}

	try {
	    Statement->Execute(SymbolTable);
	}
	catch (TScriptException &e) {
	    cerr << "ERROR: " << e << endl;
	}

	delete Statement;
    }

    cout << endl;

    return 0;
}
