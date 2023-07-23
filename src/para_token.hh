/* para_token.hh */
/* Created by Enomoto Sanshiro on 24 July 1999. */
/* Last updated by Enomoto Sanshiro on 26 July 1999. */
/* Modified by Hiroyuki Nagata 2021. */

#ifndef __ParaToken_hh__
#define __ParaToken_hh__


#include <string>
#include <regex>
#include <variant>
#include <asmjit/asmjit.h>

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
        ttSreg,
        ttMem,
        ttMem8,
        ttMem16,
        ttMem32,
        ttMem64,
        ttAcc,
        ttImm,
        ttLabel,
        ttRel8,
        ttRel16,
        ttRel32,
        ttAttrUnknown,
    };

    // enumと同様の意味の文字列表現
    static constexpr const char* TIAttributeNames[] = {
        "ttReg8",
        "ttReg16",
        "ttReg32",
        "ttReg64",
        "ttSreg",
        "ttMem",
        "ttMem8",
        "ttMem16",
        "ttMem32",
        "ttMem64",
        "ttAcc",
        "ttImm",
        "ttLabel",
        "ttRel8",
        "ttRel16",
        "ttRel32",
        "ttAttrUnknown",
    };

    static std::regex registers8Hi;
    static std::regex registers8Lo;
    static std::regex registers16;
    static std::regex registers32;
    static std::regex registers64;
    static std::regex segment_registers;

public:

    TParaToken(void);
    explicit TParaToken(const std::string& token_string,
                        TTokenType type);
    explicit TParaToken(const std::string& token_string,
                        TTokenType type,
                        TIdentiferAttribute attr);
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

    // asmjit provides following datatype
    // https://github.com/asmjit/asmjit/blob/master/src/asmjit/x86/x86operand.h
    // ---
    // - \ref x86::Reg - Base class for any X86 register.
    //   - \ref x86::Gp - General purpose register:
    //     - \ref x86::GpbLo - 8-bit low register.
    //     - \ref x86::GpbHi - 8-bit high register.
    //     - \ref x86::Gpw - 16-bit register.
    //     - \ref x86::Gpd - 32-bit register.
    //     - \ref x86::Gpq - 64-bit register (X64 only).
    //   - \ref x86::Vec - Vector (SIMD) register:
    //     - \ref x86::Xmm - 128-bit SIMD register (SSE+).
    //     - \ref x86::Ymm - 256-bit SIMD register (AVX+).
    //     - \ref x86::Zmm - 512-bit SIMD register (AVX512+).
    //   - \ref x86::Mm - 64-bit MMX register.
    //   - \ref x86::St - 80-bit FPU register.
    //   - \ref x86::KReg - opmask registers (AVX512+).
    //   - \ref x86::SReg - segment register.
    //   - \ref x86::CReg - control register.
    //   - \ref x86::DReg - debug register.
    //   - \ref x86::Bnd - bound register (discontinued).
    //   - \ref x86::Rip - relative instruction pointer.
    asmjit::x86::GpbLo AsAsmJitGpbLo(void) const;
    asmjit::x86::GpbHi AsAsmJitGpbHi(void) const;
    asmjit::x86::Gpw AsAsmJitGpw(void) const;
    asmjit::x86::Gpd AsAsmJitGpd(void) const;
    bool IsAsmJitGpbLo(void) const;
    bool IsAsmJitGpbHi(void) const;
    bool IsAsmJitGpw(void) const;
    bool IsAsmJitGpd(void) const;

    uint32_t AsUInt32(void) const noexcept(false);
    int32_t AsInt32(void) const noexcept(false);
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
    std::string to_lower(const std::string& in) const;
};


#endif
