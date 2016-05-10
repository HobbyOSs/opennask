/* tokenizersample.cc */
/* Created by Enomoto Sanshiro on 25 June 1999. */
/* Last updated by Enomoto Sanshiro on 7 July 1999. */


#include <iostream>
#include <cstdlib>
#include "ParaDefs.hh"
#include "ParaTokenizer.hh"

using namespace std;


int main(int argc, char** argv)
{
    try {
        TParaCxxTokenTable TokenTable;
        TParaTokenizer Tokenizer(cin, &TokenTable);

	//Tokenizer.SetCommentSkipping(false);
	//Tokenizer.SetWhiteSpaceSkipping(false);
	//Tokenizer.SetEscapeSequenceProcessing(false);
        
        TParaToken Token;
        while (! (Token = Tokenizer.Next()).IsEmpty()) {
            if (Token.IsKeyword()) {
                cout << "k: ";
            }
            else if (Token.IsIdentifier()) {
                cout << "i: ";
            }
            else if (Token.IsInteger()) {
                cout << "z: ";
            }
            else if (Token.IsFloating()) {
                cout << "r: ";
            }
            else if (Token.IsSeparator()) {
                cout << "s: ";
            }
            else if (Token.IsOperator()) {
                cout << "o: ";
            }
            else if (Token.IsQuote()) {
                cout << "q: ";
            }
            else if (Token.IsComment()) {
                cout << "c: ";
            }
            else if (Token.IsWhiteSpace()) {
                cout << "w: ";
            }
            else {
                cout << "\?: ";
            }
            
            cout << '[' << Token.AsString() << ']' << endl;
        }
    }
    catch (TScriptException &e) {
        cerr << "ERROR: " << e << endl;
    }
    
    return 0;
}
