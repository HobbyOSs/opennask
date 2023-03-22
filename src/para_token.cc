/* para_token.cc */
/* Created by Enomoto Sanshiro on 24 July 1999. */
/* Last updated by Enomoto Sanshiro on 26 June 1999. */
/* Modified by Hiroyuki Nagata 2021. */

#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <string>
#include <exception>
#include <stdexcept>
#include "para_token.hh"

using namespace std;

TParaToken::TParaToken(void) {
    _type = ttEmpty;
}

TParaToken::TParaToken(const string& token_string,
                       TTokenType type) {

    _token_string = token_string;
    _type = type;
    SetAttribute();
}

TParaToken::TParaToken(const string& token_string,
                       TTokenType type,
                       TIdentiferAttribute attr) {

    _token_string = token_string;
    _type = type;
    SetAttribute(attr);
}

TParaToken::TParaToken(const TParaToken& token) {
    _token_string = token._token_string;
    _type = token._type;
    _attr = token._attr;
}

TParaToken::~TParaToken() {
}

void TParaToken::SetAttribute() {
    // https://ja.wikibooks.org/wiki/X86%E3%82%A2%E3%82%BB%E3%83%B3%E3%83%96%E3%83%A9/x86%E3%82%A2%E3%83%BC%E3%82%AD%E3%83%86%E3%82%AF%E3%83%81%E3%83%A3
    std::regex registers8 (R"(AL|BL|CL|DL|AH|BH|CH|DH)");
    std::regex registers16(R"(AX|BX|CX|DX|SP|DI|BP|SI)");
    std::regex registers32(R"(EAX|EBX|ECX|EDX|ESP|EDI|EBP|ESI)");
    std::regex registers64(R"(RAX|RBX|RCX|RDX)");
    std::regex segment_registers(R"(CS|DS|ES|SS|FS|GS)");

    if (std::regex_match(_token_string, registers8)) {
        _attr = TIdentiferAttribute::ttReg8;
    } else if (std::regex_match(_token_string, registers16)) {
        _attr = TIdentiferAttribute::ttReg16;
    } else if (std::regex_match(_token_string, registers32)) {
        _attr = TIdentiferAttribute::ttReg32;
    } else if (std::regex_match(_token_string, registers64)) {
        _attr = TIdentiferAttribute::ttReg64;
    } else if (std::regex_match(_token_string, segment_registers)) {
        _attr = TIdentiferAttribute::ttSegReg;
    } else if (IsImmediate()) {
        _attr = TIdentiferAttribute::ttImm; // 即値
    } else if (IsIdentifier()) {
        _attr = TIdentiferAttribute::ttLabel; // レジスタ以外のidentなのでラベルと判定
    } else {
        _attr = TIdentiferAttribute::ttAttrUnknown;
    }
}

void TParaToken::SetAttribute(TIdentiferAttribute attr) {
    _attr = attr;
}

std::string TParaToken::to_string() const {
    std::ostringstream oss;
    oss << "token_type: " << TTokenNames[_type]
        << ", token_string: " << _token_string
        << ", token_attribute: " << TIAttributeNames[_attr];
    return oss.str();
}

TParaToken& TParaToken::operator=(const TParaToken& token) {
    _token_string = token._token_string;
    _type = token._type;

    return *this;
}

bool TParaToken::IsKeyword(void) const {
    return (_type == TParaToken::ttKeyword);
}

bool TParaToken::IsIdentifier(void) const {
    return (_type == TParaToken::ttIdentifier);
}

bool TParaToken::IsImmediate(void) const {
    return IsInteger() || IsHex() || IsFloating();
}

bool TParaToken::IsInteger(void) const {
    return (_type == TParaToken::ttInteger);
}

bool TParaToken::IsHex(void) const {
    return (_type == TParaToken::ttHex);
}

bool TParaToken::IsFloating(void) const {
    return (_type == TParaToken::ttFloating);
}

bool TParaToken::IsSeparator(void) const {
    return (_type == TParaToken::ttSeparator);
}

bool TParaToken::IsOperator(void) const {
    return (_type == TParaToken::ttOperator);
}

bool TParaToken::IsQuote(void) const {
    return (_type == TParaToken::ttQuote);
}

bool TParaToken::IsEmpty(void) const {
    return (_type == TParaToken::ttEmpty);
}

bool TParaToken::IsComment(void) const {
    return (_type == TParaToken::ttComment);
}

bool TParaToken::IsWhiteSpace(void) const {
    return (_type == TParaToken::ttWhiteSpace);
}

bool TParaToken::Is(const string& string) const {
    return (_token_string == string);
}

bool TParaToken::IsNot(const string& string) const {
    return (_token_string != string);
}

string TParaToken::AsString(void) const {
    return _token_string;
}

int TParaToken::AsInt(void) const noexcept(false) {
    if (! IsInteger() && ! IsHex() ) {
        ThrowUnexpected("integer");
    }

    int int_value;
    if ((_token_string.size() > 2) && (tolower(_token_string[1]) == 'x')) {
        // Hex Number
        istringstream value_stream(_token_string.substr(2, string::npos));
        if (! (value_stream >> hex >> int_value)) {
            ThrowUnexpected("integer");
        }
    } else {
        // Dec Number
        istringstream value_stream(_token_string);
        if (! (value_stream >> int_value)) {
            ThrowUnexpected("integer");
        }
    }

    return int_value;
}

long TParaToken::AsLong(void) const noexcept(false) {
    if (! IsInteger() && ! IsHex() ) {
        ThrowUnexpected("integer");
    }

    long long_value;
    if ((_token_string.size() > 2) && (tolower(_token_string[1]) == 'x')) {
        // Hex Number
        istringstream value_stream(_token_string.substr(2, string::npos));
        if (! (value_stream >> hex >> long_value)) {
            ThrowUnexpected("integer");
        }
    } else {
        // Dec Number
        istringstream value_stream(_token_string);
        if (! (value_stream >> long_value)) {
            ThrowUnexpected("integer");
        }
    }

    return long_value;
}

double TParaToken::AsDouble(void) const noexcept(false) {
    if ((! IsInteger()) && (! IsFloating())) {
        ThrowUnexpected("floating number");
    }

    double double_value;
    if (IsInteger()) {
        // the token might be a HEX number, so use AsLong() conversion
        double_value = (double) AsLong();
    } else {
        istringstream value_stream(_token_string);
        if (! (value_stream >> double_value)) {
            ThrowUnexpected("floating number");
        }
    }

    return double_value;
}

std::array<uint8_t, 1> TParaToken::AsUInt8t() const {
    const int v = AsInt();
    return std::array<uint8_t, 1>{static_cast<uint8_t>(v)};
}

std::array<uint8_t, 2> TParaToken::AsUInt16t() const {
    const int word = AsInt();
    return std::array<uint8_t, 2>{
        static_cast<uint8_t>( word & 0xff ),
        static_cast<uint8_t>( (word >> 8) & 0xff ),
    };
}

std::array<uint8_t, 4> TParaToken::AsUInt32t() const {
    const long dword = AsLong();
    return std::array<uint8_t, 4>{
        static_cast<uint8_t>( dword & 0xff ),
        static_cast<uint8_t>( (dword >> 8)  & 0xff ),
        static_cast<uint8_t>( (dword >> 16) & 0xff ),
        static_cast<uint8_t>( (dword >> 24) & 0xff ),
    };
}

size_t TParaToken::GetImmSize() const {
    const std::string supplied_hex = (_token_string.substr(2).size() % 2 == 0) ?
        _token_string.substr(2) : "0" + _token_string.substr(2);
    const size_t s = supplied_hex.size() / 2;

    return s;
}

TParaToken::TTokenType TParaToken::AsType() const {
    return _type;
}

TParaToken::TIdentiferAttribute TParaToken::AsAttr() const {
    return _attr;
}

TParaToken& TParaToken::RemoveQuotation(char quoter) {

    if (_token_string.size() > 1) {
        string::iterator head = _token_string.begin();
        string::iterator tail = _token_string.end() - 1;

        if (quoter == '\0') {
            quoter = *head;
        }

        if ((*head == quoter) && (*tail == quoter)) {
            _token_string.erase(_token_string.begin());
            _token_string.erase(_token_string.end() - 1);
        }
    }

    return *this;
}

TParaToken& TParaToken::MustBe(const string& expected_string) noexcept(false) {

    if (_token_string != expected_string) {
        string message = "";
        message += "invalid token: \"" + AsString() + "\"";
        message += " (\"" + expected_string + "\" is expected)";

        throw std::runtime_error(message);
    }

    return *this;
}

TParaToken& TParaToken::MustBe(TTokenType expected_token_type) noexcept(false) {

    if (_type != expected_token_type) {
        string expected = "\?\?\?";
        switch (expected_token_type) {
        case ttKeyword:
            expected = "keyword";
            break;
        case ttIdentifier:
            expected = "identifier";
            break;
        case ttInteger:
            expected = "integer";
            break;
        case ttHex:
            expected = "hex";
            break;
        case ttFloating:
            expected = "floating";
            break;
        case ttSeparator:
            expected = "separator";
            break;
        case ttOperator:
            expected = "operator";
            break;
        case ttQuote:
            expected = "quote";
            break;
        default:
            ;
        }

        string message = "";
        message += "invalid token: \"" + AsString() + "\"";
        message += " (\"" + expected + "\" is expected)";

        throw std::runtime_error(message);
    }

    return *this;
}

void TParaToken::ThrowUnexpected(const string& expected) const noexcept(false) {

    string message = "";

    if (! IsEmpty()) {
        message += "unexpected token: \"" + AsString() + "\"";
    } else {
        message += "unexpected end-of-file";
    }

    if (! expected.empty()) {
        message += " (" + expected + " is expected)";
    }

    throw std::runtime_error(message);
}
