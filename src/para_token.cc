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

TParaToken::TParaToken(const string& token_string, TTokenType type) {
    _token_string = token_string;
    _type = type;
}

TParaToken::TParaToken(const TParaToken& token) {
    _token_string = token._token_string;
    _type = token._type;
}

TParaToken::~TParaToken() {
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

bool TParaToken::IsInteger(void) const {
    return (_type == TParaToken::ttInteger);
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

long TParaToken::AsLong(void) const noexcept(false) {
    if (! IsInteger()) {
        throw std::runtime_error("integer is expected.");
    }

    long LongValue;
    if ((_token_string.size() > 2) && (tolower(_token_string[1]) == 'x')) {
        // Hex Number
        istringstream value_stream(_token_string.substr(2, string::npos));
        if (! (value_stream >> hex >> LongValue)) {
            throw std::runtime_error("integer is expected.");
        }
    } else {
        // Dec Number
        istringstream value_stream(_token_string);
        if (! (value_stream >> LongValue)) {
            throw std::runtime_error("integer is expected.");
        }
    }

    return LongValue;
}

double TParaToken::AsDouble(void) const noexcept(false) {
    if ((! IsInteger()) && (! IsFloating())) {
        throw std::runtime_error(
            "floating number is expected."
        );
    }

    double double_value;
    if (IsInteger()) {
        // the token might be a HEX number, so use AsLong() conversion
        double_value = (double) AsLong();
    } else {
        istringstream value_stream(_token_string);
        if (! (value_stream >> double_value)) {
            throw std::runtime_error(
                "floating number is expected."
            );
        }
    }

    return double_value;
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

void TParaToken::ThrowUnexpected(const string& expected) noexcept(false) {

    string message = "";

    if (! IsEmpty()) {
        message += "unexpected token: \"" + AsString() + "\"";
    }
    else {
        message += "unexpected end-of-file";
    }

    if (! expected.empty()) {
        message += " (" + expected + " is expected)";
    }

    throw std::runtime_error(message);
}
