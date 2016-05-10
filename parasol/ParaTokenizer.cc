/* ParaTokenizer.cc */
/* Created by Enomoto Sanshiro on 24 June 1999. */
/* Last updated by Enomoto Sanshiro on 11 March 2002. */


#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include "ParaToken.hh"
#include "ParaTokenTable.hh"
#include "ParaTokenizer.hh"

using namespace std;


TParaInputBuffer::TParaInputBuffer(istream& InputStream)
: _InputStream(InputStream)
{
    _InputStream.unsetf(ios::skipws);
    _LineNumber = 1;

    _ChildInputBuffer = 0;
}

TParaInputBuffer::~TParaInputBuffer()
{
}

bool TParaInputBuffer::GetNext(char& Character)
{
    if (! _UngetStack.empty()) {
	Character = _UngetStack.top();
	_UngetStack.pop();
	return true;
    }

    if (_ChildInputBuffer) {
	if (_ChildInputBuffer->GetNext(Character)) {
	    return true;
	}
	else {
	    delete _ChildInputBuffer;
	    _ChildInputBuffer = 0;
	}
    }

    if (! (_InputStream >> Character)) {
	Character = 0;
	return false;
    }

    if (Character == '\n') {
	_LineNumber++;
    }

    return true;
}

void TParaInputBuffer::Unget(char Character)
{
    if (_ChildInputBuffer) {
	_ChildInputBuffer->Unget(Character);
    }
    else {
	_UngetStack.push(Character);
    }
}

long TParaInputBuffer::LineNumber(void) 
{
    if (_ChildInputBuffer) {
	return _ChildInputBuffer->LineNumber();
    }
    else {
	return _LineNumber;
    }
}

void TParaInputBuffer::SetChildInput(istream& InputStream)
{
    this->SetChildInputBuffer(new TParaInputBuffer(InputStream));
}

void TParaInputBuffer::SetChildInputBuffer(TParaInputBuffer* InputBuffer)
{
    if (_ChildInputBuffer) {
	_ChildInputBuffer->SetChildInputBuffer(InputBuffer);
    }
    else {
	_ChildInputBuffer = InputBuffer;
    }
}

bool TParaInputBuffer::AbortChildInput(void)
{
    if (_ChildInputBuffer == 0) {
	return false;
    }

    if (! _ChildInputBuffer->AbortChildInput()) {
	delete _ChildInputBuffer;
	_ChildInputBuffer = 0;
    }

    return true;
}



TParaTokenizer::TParaTokenizer(istream& InputStream, const TParaTokenTable* TokenTable)
{
    _MyInputBuffer = new TParaInputBuffer(InputStream);
    _InputBuffer = _MyInputBuffer;
    _TokenTable = TokenTable;

    _IsCommentSkippingEnabled = true;
    _IsWhiteSpaceSkippingEnabled = true;
    _IsEscapeSequenceProcessingEnabled = true;
}

TParaTokenizer::TParaTokenizer(TParaInputBuffer* InputBuffer, const TParaTokenTable* TokenTable)
{
    _MyInputBuffer = 0;
    _InputBuffer = InputBuffer;
    _TokenTable = TokenTable;

    _IsCommentSkippingEnabled = true;
    _IsWhiteSpaceSkippingEnabled = true;
    _IsEscapeSequenceProcessingEnabled = true;
}

TParaTokenizer::~TParaTokenizer()
{
    delete _MyInputBuffer;
}

void TParaTokenizer::SetTokenTable(const TParaTokenTable* TokenTable)
{
    _TokenTable = TokenTable;
}

void TParaTokenizer::SetCommentSkipping(bool IsEnabled)
{
    _IsCommentSkippingEnabled = IsEnabled;
}

void TParaTokenizer::SetWhiteSpaceSkipping(bool IsEnabled)
{
    _IsWhiteSpaceSkippingEnabled = IsEnabled;
}

void TParaTokenizer::SetEscapeSequenceProcessing(bool IsEnabled)
{
    _IsEscapeSequenceProcessingEnabled = IsEnabled;
}

TParaToken TParaTokenizer::Next(void)
{
    TParaToken Token;

    if (_UngetStack.empty()) {
	Token = ParseNext();
    }
    else {
	Token = _UngetStack.top();
	_UngetStack.pop();
    }

    if (_IsWhiteSpaceSkippingEnabled && Token.IsWhiteSpace()) {
        return Next();
    }
    if (_IsCommentSkippingEnabled && Token.IsComment()) {
        return Next();
    }

    return Token;
}

TParaToken TParaTokenizer::GetLine(char Terminator)
{
    string TokenString;
    TParaToken::TTokenType TokenType = TParaToken::ttUnknown;

    char Character;
    while ((_InputBuffer->GetNext(Character)) && (Character != Terminator)) {
	TokenString += Character;
    }

    return TParaToken(TokenString, TokenType, LineNumber());
}

bool TParaTokenizer::GetChar(char& Character)
{
    return _InputBuffer->GetNext(Character);
}

void TParaTokenizer::UngetChar(char Character)
{
    _InputBuffer->Unget(Character);
}

TParaTokenizer& TParaTokenizer::SkipWhiteSpace(void)
{
    char Character;
    while ((_InputBuffer->GetNext(Character)) && isspace(Character)) {
	;
    }

    if (Character != 0) {
	_InputBuffer->Unget(Character);
    }

    return *this;
}

long TParaTokenizer::LineNumber(void) const
{
    return _InputBuffer->LineNumber();
}

TParaInputBuffer* TParaTokenizer::InputBuffer(void)
{
    return _InputBuffer;
}

void TParaTokenizer::Unget(TParaToken &Token)
{
    _UngetStack.push(Token);
}

TParaToken TParaTokenizer::LookAhead(int n)
{
    TParaToken Token;
    vector<TParaToken> PrecedingTokenList;
    
    for (int i = 0; i < n; i++) {
        Token = Next();
        PrecedingTokenList.push_back(Token);
    }
    for (int j = n - 1; j >= 0; j--) {
        Unget(PrecedingTokenList[j]);
    }

    return Token;
}

bool TParaTokenizer::ConvertEscape(char& Character) const
{
    if (! _IsEscapeSequenceProcessingEnabled) {
	return false;
    }

    switch (Character) {
      case 'a':
	Character = '\a'; break;
      case 'b':
        Character = '\b'; break;
      case 'f':
        Character = '\f'; break;
      case 'n':
        Character = '\n'; break;
      case 'r':
        Character = '\r'; break;
      case 't':
        Character = '\t'; break;
      case 'v':
        Character = '\v'; break;
      case '\\':
        break;
      case '\"':
        break;
      case '\'':
        break;
      case '\?':
        break;
      default:
	return false;
    }

    return true;
}

TParaToken TParaTokenizer::ParseNext(void)
{
    string TokenString;
    TParaToken::TTokenType TokenType = TParaToken::ttEmpty;

    char Character;
    if (_InputBuffer->GetNext(Character)) {
        TokenString += Character;
    }
    else {
        return TParaToken("", TParaToken::ttEmpty, LineNumber());
    }
    
    if (isspace(Character)) {
	ParseWhiteSpace(TokenString, TokenType);
    }
    else if (_TokenTable->IsAlphabet(Character)) {
        ParseIdentifier(TokenString, TokenType);
    }
    else if (isdigit(Character)) {
        ParseNumber(TokenString, TokenType);
    }
    else if ((Character == '\"') || (Character == '\'')) {
        ParseQuote(TokenString, TokenType);
    }
    else if (_TokenTable->IsOperator(TokenString)){
        ParseOperator(TokenString, TokenType);
    }
    else if (_TokenTable->IsSeparator(TokenString)){
        TokenType = TParaToken::ttSeparator;
    }
    else {
	TokenType = TParaToken::ttUnknown;
    }

    return TParaToken(TokenString, TokenType, LineNumber());
}

void TParaTokenizer::ParseIdentifier(string& TokenString, TParaToken::TTokenType& TokenType)
{
    char Character;
    while (_InputBuffer->GetNext(Character)) {
        if (_TokenTable->IsFollowerAlphabet(Character)) {
            TokenString += Character;
        }
        else {
            _InputBuffer->Unget(Character);
            break;
        }
    }

    if (_TokenTable->IsCommentLimiter(TokenString)) {
        ParseComment(TokenString, TokenType);
    }
    else if (_TokenTable->IsKeyword(TokenString)) {
        TokenType = TParaToken::ttKeyword;
    }
    else if (_TokenTable->IsOperator(TokenString)) {
        TokenType = TParaToken::ttOperator;
    }
    else {
        TokenType = TParaToken::ttIdentifier;
    }
}

void TParaTokenizer::ParseNumber(string& TokenString, TParaToken::TTokenType& TokenType)
{
    char Character;
    while (_InputBuffer->GetNext(Character)) {
        if (isdigit(Character)) {
            TokenString += Character;
            continue;
        }

        char PrecedingCharacter = 0;
        if (_InputBuffer->GetNext(PrecedingCharacter)) {
            _InputBuffer->Unget(PrecedingCharacter);
        }
        
        if (toupper(Character) == 'X') {
            if ((TokenString == "0") && isxdigit(PrecedingCharacter)) {
                TokenString += Character;
                ParseHexNumber(TokenString, TokenType);
                return;
            }
        }
        else if (Character == '.') {
            if (isdigit(PrecedingCharacter)) {
                TokenString += Character;
                ParseFloating(TokenString, TokenType);
                return;
            }
        }
        else if (toupper(Character) == 'E') {
	    unsigned OriginalLength = TokenString.size();

	    _InputBuffer->Unget(Character);
	    ParseFloating(TokenString, TokenType);

	    if (TokenString.size() > OriginalLength) {
		return;
	    }
	    else {
		break;
	    }
        }

        _InputBuffer->Unget(Character);
        break;
    }    

    TokenType = TParaToken::ttInteger;
}

void TParaTokenizer::ParseHexNumber(string& TokenString, TParaToken::TTokenType& TokenType)
{
    char Character;
    while (_InputBuffer->GetNext(Character)) {
        if (isxdigit(Character)) {
            TokenString += Character;
        }
        else {
            _InputBuffer->Unget(Character);
            break;
        }
    }    

    TokenType = TParaToken::ttInteger;
}

void TParaTokenizer::ParseFloating(string& TokenString, TParaToken::TTokenType& TokenType)
{
    char Character;
    while (_InputBuffer->GetNext(Character)) {

        if (isdigit(Character)) {
            TokenString += Character;
	    continue;
        }
        else if (toupper(Character) != 'E') {
            _InputBuffer->Unget(Character);
	    break;
	}
        else {
	    char PrecedingCharacter = 0;
	    char PrecedingPrecedingCharacter = 0;
	    if (_InputBuffer->GetNext(PrecedingCharacter)) {
		if (_InputBuffer->GetNext(PrecedingPrecedingCharacter)) {
		    _InputBuffer->Unget(PrecedingPrecedingCharacter);
		}
		_InputBuffer->Unget(PrecedingCharacter);
	    }
        
            bool PrecedingIsSign = (
                (PrecedingCharacter == '-') || (PrecedingCharacter == '+')
            );
            
            if (
                isdigit(PrecedingCharacter) ||
                (PrecedingIsSign && isdigit(PrecedingPrecedingCharacter))
            ){
                TokenString += Character;
                ParseFloatingExponent(TokenString, TokenType);
                return;
            }

            _InputBuffer->Unget(Character);
            break;
        }
    }    

    TokenType = TParaToken::ttFloating;
}

void TParaTokenizer::ParseFloatingExponent(string& TokenString, TParaToken::TTokenType& TokenType)
{
    char Character;
    if (_InputBuffer->GetNext(Character)) {
        if ((Character == '+') || (Character == '-')) {
            TokenString += Character;
        }
        else {
            _InputBuffer->Unget(Character);
        }
    }
    
    while (_InputBuffer->GetNext(Character)) {
        if (isdigit(Character)) {
            TokenString += Character;
        }
        else {
            _InputBuffer->Unget(Character);
            break;
        }
    }    

    TokenType = TParaToken::ttFloating;
}

void TParaTokenizer::ParseOperator(string& TokenString, TParaToken::TTokenType& TokenType)
{
    string PrecedingString = TokenString;
    char Character;
    while (_InputBuffer->GetNext(Character)) {
        PrecedingString += Character;
        if (_TokenTable->IsCommentLimiter(PrecedingString)) {
            TokenString += Character;
            ParseComment(TokenString, TokenType);
            return;
        }
        
        if (_TokenTable->IsOperator(PrecedingString)) {
            TokenString += Character;
        }
        else {
            _InputBuffer->Unget(Character);
            break;
        }
    }

    TokenType = TParaToken::ttOperator;
}

void TParaTokenizer::ParseQuote(string& TokenString, TParaToken::TTokenType& TokenType)
{
    char Quoting = TokenString[0];
    char Character;
    while (_InputBuffer->GetNext(Character)) {

        if (Character == '\\') {
            if (_InputBuffer->GetNext(Character)) {
                if (! ConvertEscape(Character)) {
		    TokenString += '\\';
		}
                TokenString += Character;
            }
            else {
                TokenString += '\\';
            }
        }
        else {
            TokenString += Character;
            if (Character == Quoting) {
                break;
            }
        }
        
    }

    TokenType = TParaToken::ttQuote;
}

void TParaTokenizer::ParseComment(string& TokenString, TParaToken::TTokenType& TokenType)
{
    string Limiter = TokenString;
    string Delimiter = _TokenTable->CommentDelimiterFor(Limiter);

    unsigned DelimiterLength = Delimiter.size();
    unsigned DelimiterOffset = TokenString.size();
    unsigned MatchLength = 0;
    
    char Character;
    while (MatchLength < DelimiterLength) {
	if (DelimiterOffset + MatchLength < TokenString.size()) {
	    Character = TokenString[DelimiterOffset + MatchLength];
	}
	else {
	    if (! _InputBuffer->GetNext(Character)) {
		break;
	    }
	    TokenString += Character;
	}

	if (Character == Delimiter[MatchLength]) {
	    MatchLength++;
	}
	else {
	    DelimiterOffset++;
	    MatchLength = 0;
	}
    }

    TokenType = TParaToken::ttComment;
}

void TParaTokenizer::ParseWhiteSpace(std::string& TokenString, TParaToken::TTokenType& TokenType)
{
    char Character;
    while (_InputBuffer->GetNext(Character)) {
        if (isspace(Character)) {
	    TokenString += Character;
	}
	else {
            _InputBuffer->Unget(Character);
            break;
        }
    }

    TokenType = TParaToken::ttWhiteSpace;
}
