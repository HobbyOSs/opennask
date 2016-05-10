/* ParaToken.hh */
/* Created by Enomoto Sanshiro on 24 July 1999. */
/* Last updated by Enomoto Sanshiro on 26 July 1999. */


#ifndef __ParaToken_hh__
#define __ParaToken_hh__


#include <string>
#include "ParaDefs.hh"


class TParaToken {
  public:
    enum TTokenType {
        ttKeyword,
        ttIdentifier,
        ttInteger, 
        ttFloating, 
        ttSeparator,
        ttOperator,
        ttQuote, 
	ttComment,
	ttWhiteSpace,
        ttEmpty, 
        ttUnknown
    };
  public:
    TParaToken(void);
    TParaToken(const std::string& TokenString, TTokenType Type, long LineNumber);
    TParaToken(const TParaToken& Token);
    virtual ~TParaToken();
    virtual TParaToken& operator=(const TParaToken& Token);
    virtual bool IsKeyword(void) const;
    virtual bool IsIdentifier(void) const;
    virtual bool IsInteger(void) const;
    virtual bool IsFloating(void) const;
    virtual bool IsSeparator(void) const;
    virtual bool IsOperator(void) const;
    virtual bool IsQuote(void) const;
    virtual bool IsEmpty(void) const;
    virtual bool IsComment(void) const;
    virtual bool IsWhiteSpace(void) const;
    virtual bool Is(const std::string& String) const;
    virtual bool IsNot(const std::string& String) const;
    virtual std::string AsString(void) const;
    virtual long AsLong(void) const throw(TScriptException);
    virtual double AsDouble(void) const throw(TScriptException);
    virtual TParaToken& RemoveQuotation(char Quoter = '\0');
    virtual TParaToken& MustBe(const std::string& ExpectedString) throw(TScriptException);
    virtual TParaToken& MustBe(TTokenType ExpectedTokenType) throw(TScriptException);
    virtual void ThrowUnexpected(const std::string& Expected = "") throw(TScriptException);
    virtual std::string Position(void) const;
  protected:
    std::string _TokenString;
    TTokenType _Type;
    long _LineNumber;
};


#endif
