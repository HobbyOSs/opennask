/* pm3.cc */
/* Created by Enomoto Sanshiro on 27 February 2002. */
/* Last updated by Enomoto Sanshiro on 27 February 2002. */


#include <iostream>
#include <cstdlib>
#include "ParaTokenizer.hh"

using namespace std;


bool ProcessMacro(TParaTokenizer& Tokenizer, ostream& os)
{
    static int PageCount = 1;

    if (
	Tokenizer.LookAhead(1).IsNot("%") || 
	Tokenizer.LookAhead(3).IsNot("%")
    ){
	return false;
    }

    Tokenizer.Next();
    TParaToken Token = Tokenizer.Next();
    Tokenizer.Next();

    if (Token.Is("%")) {
	cout << "%";
    }
    else if (Token.Is("next_page")) {
	cout << "\f";
	PageCount++;
    }
    else if (Token.Is("page")) {
	cout << PageCount;
    }
    else {
	throw TScriptException("unknown macro: " + Token.AsString());
    }

    return true;
}


int main(int argc, char** argv)
{
    try {
        TParaTokenTable TokenTable;
	TokenTable.AddOperator("%");
	TokenTable.AddAlphabet('_');
        TParaTokenizer Tokenizer(cin, &TokenTable);
        
	Tokenizer.SetCommentSkipping(false);
	Tokenizer.SetWhiteSpaceSkipping(false);
	Tokenizer.SetEscapeSequenceProcessing(false);
        
        TParaToken Token;        
        while (! (Token = Tokenizer.LookAhead()).IsEmpty()) {
	    if (! ProcessMacro(Tokenizer, cout)) {
		cout << Tokenizer.Next().AsString();
	    }
        }
    }
    catch (TScriptException &e) {
        cerr << "ERROR: " << e << endl;
    }
    
    return 0;
}
