/* ParaTokenizer.hh */
/* Created by Enomoto Sanshiro on 24 June 1999. */
/* Last updated by Enomoto Sanshiro on 24 August 2000. */


#ifndef __ParaTokenizer_hh__
#define __ParaTokenizer_hh__


#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "ParaDefs.hh"
#include "ParaToken.hh"
#include "ParaTokenTable.hh"


class TParaInputBuffer {
  public:
    explicit TParaInputBuffer(std::istream& InputStream);
    virtual ~TParaInputBuffer();
    virtual bool GetNext(char& Character);
    virtual void Unget(char Character);
    virtual long LineNumber(void);
    virtual void SetChildInput(std::istream& InputStream);
    virtual void SetChildInputBuffer(TParaInputBuffer* InputBuffer);
    virtual bool AbortChildInput(void);
  protected:
    std::istream& _InputStream;
    std::stack<char, std::vector<char> > _UngetStack;
    long _LineNumber;
    TParaInputBuffer* _ChildInputBuffer;
};

    
class TParaTokenizer {
  public:
    TParaTokenizer(std::istream& SourceStream, const TParaTokenTable* TokenTable);
    TParaTokenizer(TParaInputBuffer* InputBuffer, const TParaTokenTable* TokenTable);
    virtual ~TParaTokenizer();
    virtual TParaToken Next(void);
    virtual void Unget(TParaToken &Token);
    virtual TParaToken LookAhead(int n = 1);
    virtual TParaToken GetLine(char Terminator = '\n');
    virtual bool GetChar(char& Character);
    virtual void UngetChar(char Character);
    virtual TParaTokenizer& SkipWhiteSpace(void);
    virtual TParaInputBuffer* InputBuffer(void);
    virtual long LineNumber(void) const;
    virtual void SetTokenTable(const TParaTokenTable* TokenTable);
    virtual void SetCommentSkipping(bool IsEnabled);
    virtual void SetWhiteSpaceSkipping(bool IsEnabled);
    virtual void SetEscapeSequenceProcessing(bool IsEnabled);
  protected:
    virtual TParaToken ParseNext(void);
    virtual bool ConvertEscape(char& Character) const;
    virtual void ParseIdentifier(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseNumber(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseHexNumber(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseFloating(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseFloatingExponent(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseOperator(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseQuote(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseComment(std::string& TokenString, TParaToken::TTokenType& TokenType);
    virtual void ParseWhiteSpace(std::string& TokenString, TParaToken::TTokenType& TokenType);
  protected:
    TParaInputBuffer *_InputBuffer, *_MyInputBuffer;
    std::stack<TParaToken, std::vector<TParaToken> > _UngetStack;
    const TParaTokenTable* _TokenTable;
    bool _IsCommentSkippingEnabled;
    bool _IsWhiteSpaceSkippingEnabled;
    bool _IsEscapeSequenceProcessingEnabled;
};


#endif
