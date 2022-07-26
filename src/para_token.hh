/* para_token.hh */
/* Created by Enomoto Sanshiro on 24 July 1999. */
/* Last updated by Enomoto Sanshiro on 26 July 1999. */
/* Modified by Hiroyuki Nagata 2021. */

#ifndef __ParaToken_hh__
#define __ParaToken_hh__


#include <string>
#include <regex>


class TParaToken {

public:
    enum TTokenType {
        ttKeyword,
        ttIdentifier,
        ttInteger,
        ttHex,
        ttFloating,
        ttSeparator,
        ttOperator,
        ttQuote,
        ttComment,
        ttWhiteSpace,
        ttEmpty,
        ttUnknown,
        ttSizeOfEnum,
    };

    static constexpr const char* TTokenNames[] = {
        "ttKeyword",
        "ttIdentifier",
        "ttInteger",
        "ttHex",
        "ttFloating",
        "ttSeparator",
        "ttOperator",
        "ttQuote",
        "ttComment",
        "ttWhiteSpace",
        "ttEmpty",
        "ttUnknown",
    };

    static_assert(sizeof(TParaToken::TTokenNames)/sizeof(char*) == TParaToken::ttSizeOfEnum, "sizes dont match");

    // オペランドの種類
    enum TIdentiferAttribute {
        ttReg8 = 0,
        ttReg16,
        ttReg32,
        ttReg64,
        ttSegReg,
        ttMem,
        ttAcc,
        ttImm,
        ttLabel,
        ttAttrUnknown,
    };

    static constexpr const char* TIAttributeNames[] = {
        "ttReg8",
        "ttReg16",
        "ttReg32",
        "ttReg64",
        "ttSegReg",
        "ttMem",
        "ttAcc",
        "ttImm",
        "ttLabel",
        "ttAttrUnknown",
    };

public:
    TParaToken(void);
    TParaToken(const std::string& token_string, TTokenType type);
    TParaToken(const TParaToken& token);
    ~TParaToken();
    void SetAttribute();
    void SetAttribute(TIdentiferAttribute attr);
    std::string to_string() const;
    TParaToken& operator=(const TParaToken& token);
    bool IsKeyword(void) const;
    bool IsIdentifier(void) const;
    bool IsImmediate(void) const;
    bool IsInteger(void) const;
    bool IsHex(void) const;
    bool IsFloating(void) const;
    bool IsSeparator(void) const;
    bool IsOperator(void) const;
    bool IsQuote(void) const;
    bool IsEmpty(void) const;
    bool IsComment(void) const;
    bool IsWhiteSpace(void) const;
    bool Is(const std::string& string) const;
    bool IsNot(const std::string& string) const;
    std::string AsString(void) const;
    int AsInt(void) const noexcept(false);
    long AsLong(void) const noexcept(false);
    double AsDouble(void) const noexcept(false);
    std::array<uint8_t, 1> AsUInt8t(void) const noexcept(false);
    std::array<uint8_t, 2> AsUInt16t(void) const noexcept(false);
    std::array<uint8_t, 4> AsUInt32t(void) const noexcept(false);
    size_t GetImmSize() const;
    TTokenType AsType() const;
    TIdentiferAttribute AsAttr() const;
    TParaToken& RemoveQuotation(char quoter = '\0');
    TParaToken& MustBe(const std::string& expected_string) noexcept(false);
    TParaToken& MustBe(TTokenType expected_token_type) noexcept(false);
    void ThrowUnexpected(const std::string& expected = "") const noexcept(false);
protected:
    std::string _token_string;
    TTokenType _type;
    TIdentiferAttribute _attr;
};


#endif
