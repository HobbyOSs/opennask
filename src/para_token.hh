/* para_token.hh */
/* Created by Enomoto Sanshiro on 24 July 1999. */
/* Last updated by Enomoto Sanshiro on 26 July 1999. */
/* Modified by Hiroyuki Nagata 2021. */

#ifndef __ParaToken_hh__
#define __ParaToken_hh__


#include <string>


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
    TParaToken(const std::string& token_string, TTokenType type);
    TParaToken(const TParaToken& token);
    virtual ~TParaToken();
    virtual TParaToken& operator=(const TParaToken& token);
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
    virtual bool Is(const std::string& string) const;
    virtual bool IsNot(const std::string& string) const;
    virtual std::string AsString(void) const;
    virtual long AsLong(void) const noexcept(false);
    virtual double AsDouble(void) const noexcept(false);
    virtual TParaToken& RemoveQuotation(char quoter = '\0');
    virtual TParaToken& MustBe(const std::string& expected_string) noexcept(false);
    virtual TParaToken& MustBe(TTokenType expected_token_type) noexcept(false);
    virtual void ThrowUnexpected(const std::string& expected = "") noexcept(false);
protected:
    std::string _token_string;
    TTokenType _type;
};


#endif
