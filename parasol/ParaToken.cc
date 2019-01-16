/* ParaToken.cc */
/* Created by Enomoto Sanshiro on 24 July 1999. */
/* Last updated by Enomoto Sanshiro on 26 June 1999. */


#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <string>
#include "ParaToken.hh"

using namespace std;


TParaToken::TParaToken(void)
{
    _Type = ttEmpty;
    _LineNumber = 0;
}

TParaToken::TParaToken(const string& TokenString, TTokenType Type, long LineNumber)
{
    _TokenString = TokenString;
    _Type = Type;
    _LineNumber = LineNumber;
}

TParaToken::TParaToken(const TParaToken& Token)
{
    _TokenString = Token._TokenString;
    _Type = Token._Type;
    _LineNumber = Token._LineNumber;
}

TParaToken::~TParaToken()
{
}

TParaToken& TParaToken::operator=(const TParaToken& Token)
{
    _TokenString = Token._TokenString;
    _Type = Token._Type;
    _LineNumber = Token._LineNumber;

    return *this;
}

bool TParaToken::IsKeyword(void) const
{
    return (_Type == TParaToken::ttKeyword);
}

bool TParaToken::IsIdentifier(void) const
{
    return (_Type == TParaToken::ttIdentifier);
}

bool TParaToken::IsInteger(void) const
{
    return (_Type == TParaToken::ttInteger);
}

bool TParaToken::IsFloating(void) const
{
    return (_Type == TParaToken::ttFloating);
}

bool TParaToken::IsSeparator(void) const
{
    return (_Type == TParaToken::ttSeparator);
}

bool TParaToken::IsOperator(void) const
{
    return (_Type == TParaToken::ttOperator);
}

bool TParaToken::IsQuote(void) const
{
    return (_Type == TParaToken::ttQuote);
}

bool TParaToken::IsEmpty(void) const
{
    return (_Type == TParaToken::ttEmpty);
}

bool TParaToken::IsComment(void) const
{
    return (_Type == TParaToken::ttComment);    
}

bool TParaToken::IsWhiteSpace(void) const
{
    return (_Type == TParaToken::ttWhiteSpace);
}

bool TParaToken::Is(const string& String) const
{
    return (_TokenString == String);
}

bool TParaToken::IsNot(const string& String) const
{
    return (_TokenString != String);
}

string TParaToken::AsString(void) const
{
    return _TokenString;
}

long TParaToken::AsLong(void) const noexcept(false)
{
    if (! IsInteger()) {
        throw TScriptException(Position() + "integer is expected.");
    }

    long LongValue;
    if ((_TokenString.size() > 2) && (tolower(_TokenString[1]) == 'x')) {
	// Hex Number
	istringstream ValueStream(_TokenString.substr(2, string::npos));
	if (! (ValueStream >> hex >> LongValue)) {
	    throw TScriptException(Position() + "integer is expected.");
	}
    }
    else {
	// Dec Number
	istringstream ValueStream(_TokenString);
	if (! (ValueStream >> LongValue)) {
	    throw TScriptException(Position() + "integer is expected.");
	}
    }
    
    return LongValue;
}

double TParaToken::AsDouble(void) const noexcept(false)
{
    if ((! IsInteger()) && (! IsFloating())) {
        throw TScriptException(
	    Position() + "floating number is expected."
	);
    }

    double DoubleValue;
    if (IsInteger()) {
	// the token might be a HEX number, so use AsLong() conversion
	DoubleValue = (double) AsLong();
    }
    else {
	istringstream ValueStream(_TokenString);
	if (! (ValueStream >> DoubleValue)) {
	     throw TScriptException(
		 Position() + "floating number is expected."
	     );
	}
    }    
    
    return DoubleValue;
}

TParaToken& TParaToken::RemoveQuotation(char Quoter)
{
    if (_TokenString.size() > 1) {
	string::iterator Head = _TokenString.begin();
	string::iterator Tail = _TokenString.end() - 1;
    
	if (Quoter == '\0') {
	    Quoter = *Head;
	}

	if ((*Head == Quoter) && (*Tail == Quoter)) {
	    _TokenString.erase(_TokenString.begin());
	    _TokenString.erase(_TokenString.end() - 1);
	}
    }
    
    return *this;
}

string TParaToken::Position(void) const
{
    if (_LineNumber == 0) {
	return string("");
    }

    ostringstream Stream;
    if (_LineNumber > 0) {
	Stream << "line " << _LineNumber << ": ";
    }

    return Stream.str();
}

TParaToken& TParaToken::MustBe(const string& ExpectedString) noexcept(false)
{
    if (_TokenString != ExpectedString) {
	string Message = Position();
	Message += "invalid token: \"" + AsString() + "\"";
	Message += " (\"" + ExpectedString + "\" is expected)";

	throw TScriptException(Message);
    }

    return *this;
}

TParaToken& TParaToken::MustBe(TTokenType ExpectedTokenType) noexcept(false)
{
    if (_Type != ExpectedTokenType) {
        string Expected = "\?\?\?";
        switch (ExpectedTokenType) {
          case ttKeyword:
            Expected = "keyword";
            break;
          case ttIdentifier:
            Expected = "identifier";
            break;
          case ttInteger: 
            Expected = "integer";
            break;
          case ttFloating: 
            Expected = "floating";
            break;
          case ttSeparator:
            Expected = "separator";
            break;
          case ttOperator:
            Expected = "operator";
            break;
          case ttQuote: 
            Expected = "quote";
            break;
          default:
            ;
        }
        
	string Message = Position();
	Message += "invalid token: \"" + AsString() + "\"";
	Message += " (\"" + Expected + "\" is expected)";

	throw TScriptException(Message);
    }

    return *this;
}

void TParaToken::ThrowUnexpected(const string& Expected) noexcept(false)
{
    string Message = Position();
    
    if (! IsEmpty()) {
	Message += "unexpected token: \"" + AsString() + "\"";
    }
    else {
	Message += "unexpected end-of-file";
    }

    if (! Expected.empty()) {
	Message += " (" + Expected + " is expected)";
    }

    throw TScriptException(Message);
}
