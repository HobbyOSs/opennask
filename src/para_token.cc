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
#include "matchit.h"

using namespace std;
using namespace matchit;
using namespace asmjit;


std::regex TParaToken::registers8Hi (R"(AH|BH|CH|DH)");
std::regex TParaToken::registers8Lo (R"(AL|BL|CL|DL)");
std::regex TParaToken::registers16(R"(AX|BX|CX|DX|SP|DI|BP|SI)");
std::regex TParaToken::registers32(R"(EAX|EBX|ECX|EDX|ESP|EDI|EBP|ESI)");
std::regex TParaToken::registers64(R"(RAX|RBX|RCX|RDX)");
std::regex TParaToken::segment_registers(R"(CS|DS|ES|SS|FS|GS)");
std::regex TParaToken::control_registers(R"(CR(?:[023456789]|1[012345]?))");


TParaToken::TParaToken(void) {
    _token_string = "";
    _type = ttEmpty;
    _mem = std::make_shared<x86::Mem>();
    _segment = 0;
}

TParaToken::TParaToken(const string& token_string,
                       TTokenType type) {

    _token_string = token_string;
    _type = type;
    _mem = nullptr;
    _segment = 0;
    SetAttribute();
}

TParaToken::TParaToken(const string& token_string,
                       TTokenType type,
                       TIdentiferAttribute attr) {

    _token_string = token_string;
    _type = type;
    _mem = nullptr;
    _segment = 0;
    SetAttribute(attr);
}

TParaToken::TParaToken(const TParaToken& token) {
    _token_string = token._token_string;
    _type = token._type;
    _attr = token._attr;
    _mem = token._mem;
    _segment = token._segment;
}

TParaToken::~TParaToken() {
}

TParaToken& TParaToken::SetMem(const x86::Mem& mem, const int16_t segment) {
    _mem = std::make_shared<x86::Mem>(mem);
    _segment = segment;
    return *this;
}

x86::Mem& TParaToken::AsMem() const {
    if (_mem == nullptr)
        throw std::runtime_error("memory is not set");

    return *_mem.get();
}

bool TParaToken::IsMem() const {
    return _attr == TIdentiferAttribute::ttMem8 ||
        _attr == TIdentiferAttribute::ttMem16 ||
        _attr == TIdentiferAttribute::ttMem32;
}

bool TParaToken::HasMemBase(void) const {

    if (!IsMem()) {
        return false;
    }

    if (_mem == nullptr) {
        return false;
    }

    return _mem->hasBase();
}

int16_t TParaToken::AsSegment() const {
    return _segment;
}

void TParaToken::SetAttribute() {
    // https://ja.wikibooks.org/wiki/X86%E3%82%A2%E3%82%BB%E3%83%B3%E3%83%96%E3%83%A9/x86%E3%82%A2%E3%83%BC%E3%82%AD%E3%83%86%E3%82%AF%E3%83%81%E3%83%A3
    if (std::regex_match(_token_string, registers8Hi)) {
        _attr = TIdentiferAttribute::ttReg8;
    } else if (std::regex_match(_token_string, registers8Lo)) {
        _attr = TIdentiferAttribute::ttReg8;
    } else if (std::regex_match(_token_string, registers16)) {
        _attr = TIdentiferAttribute::ttReg16;
    } else if (std::regex_match(_token_string, registers32)) {
        _attr = TIdentiferAttribute::ttReg32;
    } else if (std::regex_match(_token_string, registers64)) {
        _attr = TIdentiferAttribute::ttReg64;
    } else if (std::regex_match(_token_string, segment_registers)) {
        _attr = TIdentiferAttribute::ttSreg;
    } else if (std::regex_match(_token_string, control_registers)) {
        _attr = TIdentiferAttribute::ttCreg;
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

asmjit::x86::GpbLo TParaToken::AsAsmJitGpbLo(void) const {

    using namespace asmjit;
    std::string s(to_lower(_token_string));

    return match(s)(
        pattern | "al" = x86::al,
        pattern | "bl" = x86::bl,
        pattern | "cl" = x86::cl,
        pattern | "dl" = x86::dl
    );
}

asmjit::x86::GpbHi TParaToken::AsAsmJitGpbHi(void) const {

    using namespace asmjit;
    std::string s(to_lower(_token_string));

    return match(s)(
        pattern | "ah" = asmjit::x86::ah,
        pattern | "bh" = asmjit::x86::bh,
        pattern | "ch" = asmjit::x86::ch,
        pattern | "dh" = asmjit::x86::dh
    );
}

asmjit::x86::Gpw TParaToken::AsAsmJitGpw(void) const {

    using namespace asmjit;
    std::string s(to_lower(_token_string));

    return match(s)(
        pattern | "ax" = asmjit::x86::ax,
        pattern | "bx" = asmjit::x86::bx,
        pattern | "cx" = asmjit::x86::cx,
        pattern | "dx" = asmjit::x86::dx,
        pattern | "sp" = asmjit::x86::sp,
        pattern | "bp" = asmjit::x86::bp,
        pattern | "si" = asmjit::x86::si,
        pattern | "di" = asmjit::x86::di
    );
}

asmjit::x86::Gpd TParaToken::AsAsmJitGpd(void) const {

    using namespace asmjit;
    std::string s(to_lower(_token_string));

    return match(s)(
        pattern | "eax" = asmjit::x86::eax,
        pattern | "ebx" = asmjit::x86::ebx,
        pattern | "ecx" = asmjit::x86::ecx,
        pattern | "edx" = asmjit::x86::edx,
        pattern | "esi" = asmjit::x86::esi,
        pattern | "edi" = asmjit::x86::edi,
        pattern | "esp" = asmjit::x86::esp,
        pattern | "ebp" = asmjit::x86::ebp,
        pattern | "esi" = asmjit::x86::esi,
        pattern | "edi" = asmjit::x86::edi
    );
}

asmjit::x86::SReg TParaToken::AsAsmJitSReg(void) const {

    using namespace asmjit;
    std::string s(to_lower(_token_string));

    return match(s)(
        pattern | "cs" = asmjit::x86::cs,
        pattern | "ds" = asmjit::x86::ds,
        pattern | "es" = asmjit::x86::es,
        pattern | "ss" = asmjit::x86::ss,
        pattern | "fs" = asmjit::x86::fs,
        pattern | "gs" = asmjit::x86::gs
    );
}

asmjit::x86::CReg TParaToken::AsAsmJitCReg(void) const {

    using namespace asmjit;
    std::string s(to_lower(_token_string));

    return match(s)(
        pattern | "cr0"  = asmjit::x86::cr0 ,
        pattern | "cr1"  = asmjit::x86::cr1 ,
        pattern | "cr2"  = asmjit::x86::cr2 ,
        pattern | "cr3"  = asmjit::x86::cr3 ,
        pattern | "cr4"  = asmjit::x86::cr4 ,
        pattern | "cr5"  = asmjit::x86::cr5 ,
        pattern | "cr6"  = asmjit::x86::cr6 ,
        pattern | "cr7"  = asmjit::x86::cr7 ,
        pattern | "cr8"  = asmjit::x86::cr8 ,
        pattern | "cr9"  = asmjit::x86::cr9 ,
        pattern | "cr10" = asmjit::x86::cr10,
        pattern | "cr11" = asmjit::x86::cr11,
        pattern | "cr12" = asmjit::x86::cr12,
        pattern | "cr13" = asmjit::x86::cr13,
        pattern | "cr14" = asmjit::x86::cr14,
        pattern | "cr15" = asmjit::x86::cr15
    );
}


bool TParaToken::IsAsmJitGpbLo(void) const {
    return std::regex_match(_token_string, registers8Lo);
}

bool TParaToken::IsAsmJitGpbHi(void) const {
    return std::regex_match(_token_string, registers8Hi);
}

bool TParaToken::IsAsmJitGpw(void) const {
    return std::regex_match(_token_string, registers16);
}

bool TParaToken::IsAsmJitGpd(void) const {
    return std::regex_match(_token_string, registers32);
}

bool TParaToken::IsAsmJitSReg(void) const {
    return std::regex_match(_token_string, segment_registers);
}

bool TParaToken::IsAsmJitCReg(void) const {
    return std::regex_match(_token_string, control_registers);
}

uint32_t TParaToken::AsUInt32(void) const noexcept(false) {
    if (! IsInteger() && ! IsHex() ) {
        ThrowUnexpected("integer");
    }

    uint32_t int_value;
    if ((_token_string.size() > 2) && (tolower(_token_string[1]) == 'x')) {
        // Hex Number
        return static_cast<uint32_t>(std::stoul(_token_string, nullptr, 16));
    } else {
        // Dec Number
        istringstream value_stream(_token_string);
        if (! (value_stream >> int_value)) {
            ThrowUnexpected("integer");
        }
    }

    return int_value;
}

int32_t TParaToken::AsInt32(void) const noexcept(false) {
    if (! IsInteger() && ! IsHex() ) {
        ThrowUnexpected("integer");
    }

    if (IsInteger()) {
        // Dec Number
        int32_t value;
        istringstream value_stream(_token_string);
        if (! (value_stream >> value)) {
            ThrowUnexpected("integer");
        }
        return value;
    }
    // Hex Number
    return match(_token_string.length() - 2)(
        pattern | (_ <= 2) = [&] {
            std::istringstream iss(_token_string);
            int16_t val;
            if (! (iss >> std::hex >> val)) {
                ThrowUnexpected("integer");
            }
            // int16_t -> int8_t -> int32_t にキャストして負の数に対応
            return static_cast<int32_t>(static_cast<int8_t>(val));
        },
        pattern | (_ <= 4) = [&] {
            std::istringstream iss(_token_string);
            uint16_t val;
            if (! (iss >> std::hex >> val)) {
                ThrowUnexpected("integer");
            }
            return static_cast<int32_t>(static_cast<int16_t>(val));
        },
        pattern | _ = [&] {
            // std::stoulでunsigned longを取得してキャスト
            return static_cast<int32_t>(std::stoul(_token_string, nullptr, 16));
        }
    );
}

double TParaToken::AsDouble(void) const noexcept(false) {
    if ((! IsInteger()) && (! IsFloating())) {
        ThrowUnexpected("floating number");
    }

    double double_value;
    if (IsInteger()) {
        // the token might be a HEX number, so use AsInt32() conversion
        double_value = static_cast<double>(AsInt32());
    } else {
        istringstream value_stream(_token_string);
        if (! (value_stream >> double_value)) {
            ThrowUnexpected("floating number");
        }
    }

    return double_value;
}

std::array<uint8_t, 1> TParaToken::AsUInt8t() const {
    const int v = AsUInt32();
    return std::array<uint8_t, 1>{static_cast<uint8_t>(v)};
}

std::array<uint8_t, 2> TParaToken::AsUInt16t() const {
    const int word = AsUInt32();
    return std::array<uint8_t, 2>{
        static_cast<uint8_t>( word & 0xff ),
        static_cast<uint8_t>( (word >> 8) & 0xff ),
    };
}

std::array<uint8_t, 4> TParaToken::AsUInt32t() const {
    const unsigned long dword = AsInt32();
    return std::array<uint8_t, 4>{
        static_cast<uint8_t>( dword & 0xff ),
        static_cast<uint8_t>( (dword >> 8)  & 0xff ),
        static_cast<uint8_t>( (dword >> 16) & 0xff ),
        static_cast<uint8_t>( (dword >> 24) & 0xff ),
    };
}

size_t TParaToken::GetOffsetSize() const {
    const int32_t imm = AsInt32();

    if (-0x80 <= imm && imm <= 0x7f) {
        return 1;
    }
    if (-0x8000 <= imm && imm <= 0x7fff) {
        return 2 ;
    }
    return 4;
}

size_t TParaToken::GetImmSize() const {
    if (!IsHex()) {
        return 0;
    }

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
        std::ostringstream oss;
        oss << "invalid token: \""
            << AsString()
            << "\""
            << " (\""
            << expected_string
            << "\" is expected)";
        throw std::runtime_error(oss.str());
    }

    return *this;
}

TParaToken& TParaToken::MustBe(TTokenType expected_token_type) noexcept(false) {

    if (_type != expected_token_type) {
        std::ostringstream oss;
        oss << "invalid token: \""
            << AsString()
            << "\""
            << " (\""
            << TTokenNames[expected_token_type]
            << "\" is expected)";
        throw std::runtime_error(oss.str());
    }

    return *this;
}

TParaToken& TParaToken::MustBe(TIdentiferAttribute expected_attr) noexcept(false) {

    if (_attr != expected_attr) {
        std::ostringstream oss;
        oss << "invalid token: \""
            << AsString()
            << "\""
            << " (\""
            << TIAttributeNames[expected_attr]
            << "\" is expected)";
        throw std::runtime_error(oss.str());
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

std::string TParaToken::to_lower(const std::string& in) const {
    std::string s(in);
    std::transform(s.begin(),
                   s.end(),
                   s.begin(),
                   [](unsigned char const &c) {
                       return ::tolower(c);
                   });
    return s;
}
