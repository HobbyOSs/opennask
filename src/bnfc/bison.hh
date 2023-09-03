// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file bison.hh
 ** Define the nask_::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_NASK_BISON_HH_INCLUDED
# define YY_NASK_BISON_HH_INCLUDED
// "%code requires" blocks.
#line 15 "nask.yy"

#include "absyn.hh"


        class NaskScanner;
        class NaskDriver;


#line 58 "bison.hh"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace nask_ {
#line 193 "bison.hh"




  /// A Bison parser.
  class NaskParser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
    {
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {}

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // _INTEGER_
      char dummy1[sizeof (int)];

      // ConfigType
      char dummy2[sizeof (std::shared_ptr<ConfigType>)];

      // DataType
      char dummy3[sizeof (std::shared_ptr<DataType>)];

      // Exp
      // Exp1
      // Exp2
      // Exp3
      // Exp4
      char dummy4[sizeof (std::shared_ptr<Exp>)];

      // Factor
      char dummy5[sizeof (std::shared_ptr<Factor>)];

      // IndexExp
      char dummy6[sizeof (std::shared_ptr<IndexExp>)];

      // JumpDir
      char dummy7[sizeof (std::shared_ptr<JumpDir>)];

      // Label
      char dummy8[sizeof (std::shared_ptr<Label>)];

      // ListFactor
      char dummy9[sizeof (std::shared_ptr<ListFactor>)];

      // ListMnemonicArgs
      char dummy10[sizeof (std::shared_ptr<ListMnemonicArgs>)];

      // ListStatement
      char dummy11[sizeof (std::shared_ptr<ListStatement>)];

      // MemoryAddr
      char dummy12[sizeof (std::shared_ptr<MemoryAddr>)];

      // MnemonicArgs
      char dummy13[sizeof (std::shared_ptr<MnemonicArgs>)];

      // Opcode
      char dummy14[sizeof (std::shared_ptr<Opcode>)];

      // OpcodeNoParam
      char dummy15[sizeof (std::shared_ptr<OpcodeNoParam>)];

      // Program
      char dummy16[sizeof (std::shared_ptr<Program>)];

      // SReg
      char dummy17[sizeof (std::shared_ptr<SReg>)];

      // Statement
      // Statement1
      char dummy18[sizeof (std::shared_ptr<Statement>)];

      // UnaryOperator
      char dummy19[sizeof (std::shared_ptr<UnaryOperator>)];

      // T_Hex
      // T_Id
      // T_NaskChar
      // _STRING_
      char dummy20[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    _ERROR_ = 258,                 // _ERROR_
    _PERCENT = 259,                // _PERCENT
    _LPAREN = 260,                 // _LPAREN
    _RPAREN = 261,                 // _RPAREN
    _STAR = 262,                   // _STAR
    _PLUS = 263,                   // _PLUS
    _COMMA = 264,                  // _COMMA
    _MINUS = 265,                  // _MINUS
    _SLASH = 266,                  // _SLASH
    _COLON = 267,                  // _COLON
    _KW_AAA = 268,                 // _KW_AAA
    _KW_AAD = 269,                 // _KW_AAD
    _KW_AAM = 270,                 // _KW_AAM
    _KW_AAS = 271,                 // _KW_AAS
    _KW_ABSOLUTE = 272,            // _KW_ABSOLUTE
    _KW_ADC = 273,                 // _KW_ADC
    _KW_ADD = 274,                 // _KW_ADD
    _KW_ALIGN = 275,               // _KW_ALIGN
    _KW_ALIGNB = 276,              // _KW_ALIGNB
    _KW_AND = 277,                 // _KW_AND
    _KW_ARPL = 278,                // _KW_ARPL
    _KW_BITS = 279,                // _KW_BITS
    _KW_BOUND = 280,               // _KW_BOUND
    _KW_BSF = 281,                 // _KW_BSF
    _KW_BSR = 282,                 // _KW_BSR
    _KW_BSWAP = 283,               // _KW_BSWAP
    _KW_BT = 284,                  // _KW_BT
    _KW_BTC = 285,                 // _KW_BTC
    _KW_BTR = 286,                 // _KW_BTR
    _KW_BTS = 287,                 // _KW_BTS
    _KW_BYTE = 288,                // _KW_BYTE
    _KW_CALL = 289,                // _KW_CALL
    _KW_CBW = 290,                 // _KW_CBW
    _KW_CDQ = 291,                 // _KW_CDQ
    _KW_CLC = 292,                 // _KW_CLC
    _KW_CLD = 293,                 // _KW_CLD
    _KW_CLI = 294,                 // _KW_CLI
    _KW_CLTS = 295,                // _KW_CLTS
    _KW_CMC = 296,                 // _KW_CMC
    _KW_CMP = 297,                 // _KW_CMP
    _KW_CMPSB = 298,               // _KW_CMPSB
    _KW_CMPSD = 299,               // _KW_CMPSD
    _KW_CMPSW = 300,               // _KW_CMPSW
    _KW_CMPXCHG = 301,             // _KW_CMPXCHG
    _KW_CPUID = 302,               // _KW_CPUID
    _KW_CS = 303,                  // _KW_CS
    _KW_CWD = 304,                 // _KW_CWD
    _KW_CWDE = 305,                // _KW_CWDE
    _KW_DAA = 306,                 // _KW_DAA
    _KW_DAS = 307,                 // _KW_DAS
    _KW_DB = 308,                  // _KW_DB
    _KW_DD = 309,                  // _KW_DD
    _KW_DEC = 310,                 // _KW_DEC
    _KW_DIV = 311,                 // _KW_DIV
    _KW_DQ = 312,                  // _KW_DQ
    _KW_DS = 313,                  // _KW_DS
    _KW_DT = 314,                  // _KW_DT
    _KW_DW = 315,                  // _KW_DW
    _KW_DWORD = 316,               // _KW_DWORD
    _KW_END = 317,                 // _KW_END
    _KW_ENTER = 318,               // _KW_ENTER
    _KW_EQU = 319,                 // _KW_EQU
    _KW_ES = 320,                  // _KW_ES
    _KW_EXTERN = 321,              // _KW_EXTERN
    _KW_F2XM1 = 322,               // _KW_F2XM1
    _KW_FABS = 323,                // _KW_FABS
    _KW_FADD = 324,                // _KW_FADD
    _KW_FADDP = 325,               // _KW_FADDP
    _KW_FAR = 326,                 // _KW_FAR
    _KW_FBLD = 327,                // _KW_FBLD
    _KW_FBSTP = 328,               // _KW_FBSTP
    _KW_FCHS = 329,                // _KW_FCHS
    _KW_FCLEX = 330,               // _KW_FCLEX
    _KW_FCOM = 331,                // _KW_FCOM
    _KW_FCOMP = 332,               // _KW_FCOMP
    _KW_FCOMPP = 333,              // _KW_FCOMPP
    _KW_FCOS = 334,                // _KW_FCOS
    _KW_FDECSTP = 335,             // _KW_FDECSTP
    _KW_FDISI = 336,               // _KW_FDISI
    _KW_FDIV = 337,                // _KW_FDIV
    _KW_FDIVP = 338,               // _KW_FDIVP
    _KW_FDIVR = 339,               // _KW_FDIVR
    _KW_FDIVRP = 340,              // _KW_FDIVRP
    _KW_FENI = 341,                // _KW_FENI
    _KW_FFREE = 342,               // _KW_FFREE
    _KW_FIADD = 343,               // _KW_FIADD
    _KW_FICOM = 344,               // _KW_FICOM
    _KW_FICOMP = 345,              // _KW_FICOMP
    _KW_FIDIV = 346,               // _KW_FIDIV
    _KW_FIDIVR = 347,              // _KW_FIDIVR
    _KW_FILD = 348,                // _KW_FILD
    _KW_FILE = 349,                // _KW_FILE
    _KW_FIMUL = 350,               // _KW_FIMUL
    _KW_FINCSTP = 351,             // _KW_FINCSTP
    _KW_FINIT = 352,               // _KW_FINIT
    _KW_FIST = 353,                // _KW_FIST
    _KW_FISTP = 354,               // _KW_FISTP
    _KW_FISUB = 355,               // _KW_FISUB
    _KW_FISUBR = 356,              // _KW_FISUBR
    _KW_FLD = 357,                 // _KW_FLD
    _KW_FLD1 = 358,                // _KW_FLD1
    _KW_FLDCW = 359,               // _KW_FLDCW
    _KW_FLDENV = 360,              // _KW_FLDENV
    _KW_FLDL2E = 361,              // _KW_FLDL2E
    _KW_FLDL2T = 362,              // _KW_FLDL2T
    _KW_FLDLG2 = 363,              // _KW_FLDLG2
    _KW_FLDLN2 = 364,              // _KW_FLDLN2
    _KW_FLDPI = 365,               // _KW_FLDPI
    _KW_FLDZ = 366,                // _KW_FLDZ
    _KW_FMUL = 367,                // _KW_FMUL
    _KW_FMULP = 368,               // _KW_FMULP
    _KW_FNCLEX = 369,              // _KW_FNCLEX
    _KW_FNDISI = 370,              // _KW_FNDISI
    _KW_FNENI = 371,               // _KW_FNENI
    _KW_FNINIT = 372,              // _KW_FNINIT
    _KW_FNOP = 373,                // _KW_FNOP
    _KW_FNSAVE = 374,              // _KW_FNSAVE
    _KW_FNSTCW = 375,              // _KW_FNSTCW
    _KW_FNSTENV = 376,             // _KW_FNSTENV
    _KW_FNSTSW = 377,              // _KW_FNSTSW
    _KW_FORMAT = 378,              // _KW_FORMAT
    _KW_FPATAN = 379,              // _KW_FPATAN
    _KW_FPREM = 380,               // _KW_FPREM
    _KW_FPREM1 = 381,              // _KW_FPREM1
    _KW_FPTAN = 382,               // _KW_FPTAN
    _KW_FRNDINT = 383,             // _KW_FRNDINT
    _KW_FRSTOR = 384,              // _KW_FRSTOR
    _KW_FS = 385,                  // _KW_FS
    _KW_FSAVE = 386,               // _KW_FSAVE
    _KW_FSCALE = 387,              // _KW_FSCALE
    _KW_FSETPM = 388,              // _KW_FSETPM
    _KW_FSIN = 389,                // _KW_FSIN
    _KW_FSINCOS = 390,             // _KW_FSINCOS
    _KW_FSQRT = 391,               // _KW_FSQRT
    _KW_FST = 392,                 // _KW_FST
    _KW_FSTCW = 393,               // _KW_FSTCW
    _KW_FSTENV = 394,              // _KW_FSTENV
    _KW_FSTP = 395,                // _KW_FSTP
    _KW_FSTSW = 396,               // _KW_FSTSW
    _KW_FSUB = 397,                // _KW_FSUB
    _KW_FSUBP = 398,               // _KW_FSUBP
    _KW_FSUBR = 399,               // _KW_FSUBR
    _KW_FSUBRP = 400,              // _KW_FSUBRP
    _KW_FTST = 401,                // _KW_FTST
    _KW_FUCOM = 402,               // _KW_FUCOM
    _KW_FUCOMP = 403,              // _KW_FUCOMP
    _KW_FUCOMPP = 404,             // _KW_FUCOMPP
    _KW_FXAM = 405,                // _KW_FXAM
    _KW_FXCH = 406,                // _KW_FXCH
    _KW_FXTRACT = 407,             // _KW_FXTRACT
    _KW_FYL2X = 408,               // _KW_FYL2X
    _KW_FYL2XP1 = 409,             // _KW_FYL2XP1
    _KW_GLOBAL = 410,              // _KW_GLOBAL
    _KW_GS = 411,                  // _KW_GS
    _KW_HLT = 412,                 // _KW_HLT
    _KW_IDIV = 413,                // _KW_IDIV
    _KW_IMUL = 414,                // _KW_IMUL
    _KW_IN = 415,                  // _KW_IN
    _KW_INC = 416,                 // _KW_INC
    _KW_INCO = 417,                // _KW_INCO
    _KW_INSB = 418,                // _KW_INSB
    _KW_INSD = 419,                // _KW_INSD
    _KW_INSTRSET = 420,            // _KW_INSTRSET
    _KW_INSW = 421,                // _KW_INSW
    _KW_INT = 422,                 // _KW_INT
    _KW_INT3 = 423,                // _KW_INT3
    _KW_INTO = 424,                // _KW_INTO
    _KW_INVD = 425,                // _KW_INVD
    _KW_INVLPG = 426,              // _KW_INVLPG
    _KW_IRET = 427,                // _KW_IRET
    _KW_IRETD = 428,               // _KW_IRETD
    _KW_IRETW = 429,               // _KW_IRETW
    _KW_JA = 430,                  // _KW_JA
    _KW_JAE = 431,                 // _KW_JAE
    _KW_JB = 432,                  // _KW_JB
    _KW_JBE = 433,                 // _KW_JBE
    _KW_JC = 434,                  // _KW_JC
    _KW_JCXZ = 435,                // _KW_JCXZ
    _KW_JE = 436,                  // _KW_JE
    _KW_JECXZ = 437,               // _KW_JECXZ
    _KW_JG = 438,                  // _KW_JG
    _KW_JGE = 439,                 // _KW_JGE
    _KW_JL = 440,                  // _KW_JL
    _KW_JLE = 441,                 // _KW_JLE
    _KW_JMP = 442,                 // _KW_JMP
    _KW_JNA = 443,                 // _KW_JNA
    _KW_JNAE = 444,                // _KW_JNAE
    _KW_JNB = 445,                 // _KW_JNB
    _KW_JNBE = 446,                // _KW_JNBE
    _KW_JNC = 447,                 // _KW_JNC
    _KW_JNE = 448,                 // _KW_JNE
    _KW_JNG = 449,                 // _KW_JNG
    _KW_JNGE = 450,                // _KW_JNGE
    _KW_JNL = 451,                 // _KW_JNL
    _KW_JNLE = 452,                // _KW_JNLE
    _KW_JNO = 453,                 // _KW_JNO
    _KW_JNP = 454,                 // _KW_JNP
    _KW_JNS = 455,                 // _KW_JNS
    _KW_JNZ = 456,                 // _KW_JNZ
    _KW_JO = 457,                  // _KW_JO
    _KW_JP = 458,                  // _KW_JP
    _KW_JPE = 459,                 // _KW_JPE
    _KW_JPO = 460,                 // _KW_JPO
    _KW_JS = 461,                  // _KW_JS
    _KW_JZ = 462,                  // _KW_JZ
    _KW_LAHF = 463,                // _KW_LAHF
    _KW_LAR = 464,                 // _KW_LAR
    _KW_LDS = 465,                 // _KW_LDS
    _KW_LEA = 466,                 // _KW_LEA
    _KW_LEAVE = 467,               // _KW_LEAVE
    _KW_LES = 468,                 // _KW_LES
    _KW_LFS = 469,                 // _KW_LFS
    _KW_LGDT = 470,                // _KW_LGDT
    _KW_LGS = 471,                 // _KW_LGS
    _KW_LIDT = 472,                // _KW_LIDT
    _KW_LLDT = 473,                // _KW_LLDT
    _KW_LMSW = 474,                // _KW_LMSW
    _KW_LOCK = 475,                // _KW_LOCK
    _KW_LODSB = 476,               // _KW_LODSB
    _KW_LODSD = 477,               // _KW_LODSD
    _KW_LODSW = 478,               // _KW_LODSW
    _KW_LOOP = 479,                // _KW_LOOP
    _KW_LOOPE = 480,               // _KW_LOOPE
    _KW_LOOPNE = 481,              // _KW_LOOPNE
    _KW_LOOPNZ = 482,              // _KW_LOOPNZ
    _KW_LOOPZ = 483,               // _KW_LOOPZ
    _KW_LSL = 484,                 // _KW_LSL
    _KW_LSS = 485,                 // _KW_LSS
    _KW_LTR = 486,                 // _KW_LTR
    _KW_MOV = 487,                 // _KW_MOV
    _KW_MOVSB = 488,               // _KW_MOVSB
    _KW_MOVSD = 489,               // _KW_MOVSD
    _KW_MOVSW = 490,               // _KW_MOVSW
    _KW_MOVSX = 491,               // _KW_MOVSX
    _KW_MOVZX = 492,               // _KW_MOVZX
    _KW_MUL = 493,                 // _KW_MUL
    _KW_NEAR = 494,                // _KW_NEAR
    _KW_NEG = 495,                 // _KW_NEG
    _KW_NOP = 496,                 // _KW_NOP
    _KW_NOT = 497,                 // _KW_NOT
    _KW_OPTIMIZE = 498,            // _KW_OPTIMIZE
    _KW_OR = 499,                  // _KW_OR
    _KW_ORG = 500,                 // _KW_ORG
    _KW_OUT = 501,                 // _KW_OUT
    _KW_OUTSB = 502,               // _KW_OUTSB
    _KW_OUTSD = 503,               // _KW_OUTSD
    _KW_OUTSW = 504,               // _KW_OUTSW
    _KW_PADDING = 505,             // _KW_PADDING
    _KW_PADSET = 506,              // _KW_PADSET
    _KW_POP = 507,                 // _KW_POP
    _KW_POPA = 508,                // _KW_POPA
    _KW_POPAD = 509,               // _KW_POPAD
    _KW_POPAW = 510,               // _KW_POPAW
    _KW_POPF = 511,                // _KW_POPF
    _KW_POPFD = 512,               // _KW_POPFD
    _KW_POPFW = 513,               // _KW_POPFW
    _KW_PUSH = 514,                // _KW_PUSH
    _KW_PUSHA = 515,               // _KW_PUSHA
    _KW_PUSHAD = 516,              // _KW_PUSHAD
    _KW_PUSHAW = 517,              // _KW_PUSHAW
    _KW_PUSHD = 518,               // _KW_PUSHD
    _KW_PUSHF = 519,               // _KW_PUSHF
    _KW_PUSHFD = 520,              // _KW_PUSHFD
    _KW_PUSHFW = 521,              // _KW_PUSHFW
    _KW_RCL = 522,                 // _KW_RCL
    _KW_RCR = 523,                 // _KW_RCR
    _KW_RDMSR = 524,               // _KW_RDMSR
    _KW_RDPMC = 525,               // _KW_RDPMC
    _KW_REP = 526,                 // _KW_REP
    _KW_REPE = 527,                // _KW_REPE
    _KW_REPNE = 528,               // _KW_REPNE
    _KW_REPNZ = 529,               // _KW_REPNZ
    _KW_REPZ = 530,                // _KW_REPZ
    _KW_RESB = 531,                // _KW_RESB
    _KW_RESD = 532,                // _KW_RESD
    _KW_RESQ = 533,                // _KW_RESQ
    _KW_REST = 534,                // _KW_REST
    _KW_RESW = 535,                // _KW_RESW
    _KW_RET = 536,                 // _KW_RET
    _KW_RETF = 537,                // _KW_RETF
    _KW_RETN = 538,                // _KW_RETN
    _KW_ROL = 539,                 // _KW_ROL
    _KW_ROR = 540,                 // _KW_ROR
    _KW_RSM = 541,                 // _KW_RSM
    _KW_SAHF = 542,                // _KW_SAHF
    _KW_SAL = 543,                 // _KW_SAL
    _KW_SAR = 544,                 // _KW_SAR
    _KW_SBB = 545,                 // _KW_SBB
    _KW_SCASB = 546,               // _KW_SCASB
    _KW_SCASD = 547,               // _KW_SCASD
    _KW_SCASW = 548,               // _KW_SCASW
    _KW_SECTION = 549,             // _KW_SECTION
    _KW_SETA = 550,                // _KW_SETA
    _KW_SETAE = 551,               // _KW_SETAE
    _KW_SETB = 552,                // _KW_SETB
    _KW_SETBE = 553,               // _KW_SETBE
    _KW_SETC = 554,                // _KW_SETC
    _KW_SETE = 555,                // _KW_SETE
    _KW_SETG = 556,                // _KW_SETG
    _KW_SETGE = 557,               // _KW_SETGE
    _KW_SETL = 558,                // _KW_SETL
    _KW_SETLE = 559,               // _KW_SETLE
    _KW_SETNA = 560,               // _KW_SETNA
    _KW_SETNAE = 561,              // _KW_SETNAE
    _KW_SETNB = 562,               // _KW_SETNB
    _KW_SETNBE = 563,              // _KW_SETNBE
    _KW_SETNC = 564,               // _KW_SETNC
    _KW_SETNE = 565,               // _KW_SETNE
    _KW_SETNG = 566,               // _KW_SETNG
    _KW_SETNGE = 567,              // _KW_SETNGE
    _KW_SETNL = 568,               // _KW_SETNL
    _KW_SETNLE = 569,              // _KW_SETNLE
    _KW_SETNO = 570,               // _KW_SETNO
    _KW_SETNP = 571,               // _KW_SETNP
    _KW_SETNS = 572,               // _KW_SETNS
    _KW_SETNZ = 573,               // _KW_SETNZ
    _KW_SETO = 574,                // _KW_SETO
    _KW_SETP = 575,                // _KW_SETP
    _KW_SETPE = 576,               // _KW_SETPE
    _KW_SETPO = 577,               // _KW_SETPO
    _KW_SETS = 578,                // _KW_SETS
    _KW_SETZ = 579,                // _KW_SETZ
    _KW_SGDT = 580,                // _KW_SGDT
    _KW_SHL = 581,                 // _KW_SHL
    _KW_SHLD = 582,                // _KW_SHLD
    _KW_SHORT = 583,               // _KW_SHORT
    _KW_SHR = 584,                 // _KW_SHR
    _KW_SHRD = 585,                // _KW_SHRD
    _KW_SIDT = 586,                // _KW_SIDT
    _KW_SLDT = 587,                // _KW_SLDT
    _KW_SMSW = 588,                // _KW_SMSW
    _KW_SS = 589,                  // _KW_SS
    _KW_STC = 590,                 // _KW_STC
    _KW_STD = 591,                 // _KW_STD
    _KW_STI = 592,                 // _KW_STI
    _KW_STOSB = 593,               // _KW_STOSB
    _KW_STOSD = 594,               // _KW_STOSD
    _KW_STOSW = 595,               // _KW_STOSW
    _KW_STR = 596,                 // _KW_STR
    _KW_SUB = 597,                 // _KW_SUB
    _KW_TEST = 598,                // _KW_TEST
    _KW_TIMES = 599,               // _KW_TIMES
    _KW_UD2 = 600,                 // _KW_UD2
    _KW_VERR = 601,                // _KW_VERR
    _KW_VERW = 602,                // _KW_VERW
    _KW_WAIT = 603,                // _KW_WAIT
    _KW_WBINVD = 604,              // _KW_WBINVD
    _KW_WORD = 605,                // _KW_WORD
    _KW_WRMSR = 606,               // _KW_WRMSR
    _KW_XADD = 607,                // _KW_XADD
    _KW_XCHG = 608,                // _KW_XCHG
    _KW_XLATB = 609,               // _KW_XLATB
    _KW_XOR = 610,                 // _KW_XOR
    _LBRACK = 611,                 // _LBRACK
    _RBRACK = 612,                 // _RBRACK
    T_Hex = 613,                   // T_Hex
    T_Id = 614,                    // T_Id
    T_NaskChar = 615,              // T_NaskChar
    _STRING_ = 616,                // _STRING_
    _INTEGER_ = 617                // _INTEGER_
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 363, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S__ERROR_ = 3,                           // _ERROR_
        S__PERCENT = 4,                          // _PERCENT
        S__LPAREN = 5,                           // _LPAREN
        S__RPAREN = 6,                           // _RPAREN
        S__STAR = 7,                             // _STAR
        S__PLUS = 8,                             // _PLUS
        S__COMMA = 9,                            // _COMMA
        S__MINUS = 10,                           // _MINUS
        S__SLASH = 11,                           // _SLASH
        S__COLON = 12,                           // _COLON
        S__KW_AAA = 13,                          // _KW_AAA
        S__KW_AAD = 14,                          // _KW_AAD
        S__KW_AAM = 15,                          // _KW_AAM
        S__KW_AAS = 16,                          // _KW_AAS
        S__KW_ABSOLUTE = 17,                     // _KW_ABSOLUTE
        S__KW_ADC = 18,                          // _KW_ADC
        S__KW_ADD = 19,                          // _KW_ADD
        S__KW_ALIGN = 20,                        // _KW_ALIGN
        S__KW_ALIGNB = 21,                       // _KW_ALIGNB
        S__KW_AND = 22,                          // _KW_AND
        S__KW_ARPL = 23,                         // _KW_ARPL
        S__KW_BITS = 24,                         // _KW_BITS
        S__KW_BOUND = 25,                        // _KW_BOUND
        S__KW_BSF = 26,                          // _KW_BSF
        S__KW_BSR = 27,                          // _KW_BSR
        S__KW_BSWAP = 28,                        // _KW_BSWAP
        S__KW_BT = 29,                           // _KW_BT
        S__KW_BTC = 30,                          // _KW_BTC
        S__KW_BTR = 31,                          // _KW_BTR
        S__KW_BTS = 32,                          // _KW_BTS
        S__KW_BYTE = 33,                         // _KW_BYTE
        S__KW_CALL = 34,                         // _KW_CALL
        S__KW_CBW = 35,                          // _KW_CBW
        S__KW_CDQ = 36,                          // _KW_CDQ
        S__KW_CLC = 37,                          // _KW_CLC
        S__KW_CLD = 38,                          // _KW_CLD
        S__KW_CLI = 39,                          // _KW_CLI
        S__KW_CLTS = 40,                         // _KW_CLTS
        S__KW_CMC = 41,                          // _KW_CMC
        S__KW_CMP = 42,                          // _KW_CMP
        S__KW_CMPSB = 43,                        // _KW_CMPSB
        S__KW_CMPSD = 44,                        // _KW_CMPSD
        S__KW_CMPSW = 45,                        // _KW_CMPSW
        S__KW_CMPXCHG = 46,                      // _KW_CMPXCHG
        S__KW_CPUID = 47,                        // _KW_CPUID
        S__KW_CS = 48,                           // _KW_CS
        S__KW_CWD = 49,                          // _KW_CWD
        S__KW_CWDE = 50,                         // _KW_CWDE
        S__KW_DAA = 51,                          // _KW_DAA
        S__KW_DAS = 52,                          // _KW_DAS
        S__KW_DB = 53,                           // _KW_DB
        S__KW_DD = 54,                           // _KW_DD
        S__KW_DEC = 55,                          // _KW_DEC
        S__KW_DIV = 56,                          // _KW_DIV
        S__KW_DQ = 57,                           // _KW_DQ
        S__KW_DS = 58,                           // _KW_DS
        S__KW_DT = 59,                           // _KW_DT
        S__KW_DW = 60,                           // _KW_DW
        S__KW_DWORD = 61,                        // _KW_DWORD
        S__KW_END = 62,                          // _KW_END
        S__KW_ENTER = 63,                        // _KW_ENTER
        S__KW_EQU = 64,                          // _KW_EQU
        S__KW_ES = 65,                           // _KW_ES
        S__KW_EXTERN = 66,                       // _KW_EXTERN
        S__KW_F2XM1 = 67,                        // _KW_F2XM1
        S__KW_FABS = 68,                         // _KW_FABS
        S__KW_FADD = 69,                         // _KW_FADD
        S__KW_FADDP = 70,                        // _KW_FADDP
        S__KW_FAR = 71,                          // _KW_FAR
        S__KW_FBLD = 72,                         // _KW_FBLD
        S__KW_FBSTP = 73,                        // _KW_FBSTP
        S__KW_FCHS = 74,                         // _KW_FCHS
        S__KW_FCLEX = 75,                        // _KW_FCLEX
        S__KW_FCOM = 76,                         // _KW_FCOM
        S__KW_FCOMP = 77,                        // _KW_FCOMP
        S__KW_FCOMPP = 78,                       // _KW_FCOMPP
        S__KW_FCOS = 79,                         // _KW_FCOS
        S__KW_FDECSTP = 80,                      // _KW_FDECSTP
        S__KW_FDISI = 81,                        // _KW_FDISI
        S__KW_FDIV = 82,                         // _KW_FDIV
        S__KW_FDIVP = 83,                        // _KW_FDIVP
        S__KW_FDIVR = 84,                        // _KW_FDIVR
        S__KW_FDIVRP = 85,                       // _KW_FDIVRP
        S__KW_FENI = 86,                         // _KW_FENI
        S__KW_FFREE = 87,                        // _KW_FFREE
        S__KW_FIADD = 88,                        // _KW_FIADD
        S__KW_FICOM = 89,                        // _KW_FICOM
        S__KW_FICOMP = 90,                       // _KW_FICOMP
        S__KW_FIDIV = 91,                        // _KW_FIDIV
        S__KW_FIDIVR = 92,                       // _KW_FIDIVR
        S__KW_FILD = 93,                         // _KW_FILD
        S__KW_FILE = 94,                         // _KW_FILE
        S__KW_FIMUL = 95,                        // _KW_FIMUL
        S__KW_FINCSTP = 96,                      // _KW_FINCSTP
        S__KW_FINIT = 97,                        // _KW_FINIT
        S__KW_FIST = 98,                         // _KW_FIST
        S__KW_FISTP = 99,                        // _KW_FISTP
        S__KW_FISUB = 100,                       // _KW_FISUB
        S__KW_FISUBR = 101,                      // _KW_FISUBR
        S__KW_FLD = 102,                         // _KW_FLD
        S__KW_FLD1 = 103,                        // _KW_FLD1
        S__KW_FLDCW = 104,                       // _KW_FLDCW
        S__KW_FLDENV = 105,                      // _KW_FLDENV
        S__KW_FLDL2E = 106,                      // _KW_FLDL2E
        S__KW_FLDL2T = 107,                      // _KW_FLDL2T
        S__KW_FLDLG2 = 108,                      // _KW_FLDLG2
        S__KW_FLDLN2 = 109,                      // _KW_FLDLN2
        S__KW_FLDPI = 110,                       // _KW_FLDPI
        S__KW_FLDZ = 111,                        // _KW_FLDZ
        S__KW_FMUL = 112,                        // _KW_FMUL
        S__KW_FMULP = 113,                       // _KW_FMULP
        S__KW_FNCLEX = 114,                      // _KW_FNCLEX
        S__KW_FNDISI = 115,                      // _KW_FNDISI
        S__KW_FNENI = 116,                       // _KW_FNENI
        S__KW_FNINIT = 117,                      // _KW_FNINIT
        S__KW_FNOP = 118,                        // _KW_FNOP
        S__KW_FNSAVE = 119,                      // _KW_FNSAVE
        S__KW_FNSTCW = 120,                      // _KW_FNSTCW
        S__KW_FNSTENV = 121,                     // _KW_FNSTENV
        S__KW_FNSTSW = 122,                      // _KW_FNSTSW
        S__KW_FORMAT = 123,                      // _KW_FORMAT
        S__KW_FPATAN = 124,                      // _KW_FPATAN
        S__KW_FPREM = 125,                       // _KW_FPREM
        S__KW_FPREM1 = 126,                      // _KW_FPREM1
        S__KW_FPTAN = 127,                       // _KW_FPTAN
        S__KW_FRNDINT = 128,                     // _KW_FRNDINT
        S__KW_FRSTOR = 129,                      // _KW_FRSTOR
        S__KW_FS = 130,                          // _KW_FS
        S__KW_FSAVE = 131,                       // _KW_FSAVE
        S__KW_FSCALE = 132,                      // _KW_FSCALE
        S__KW_FSETPM = 133,                      // _KW_FSETPM
        S__KW_FSIN = 134,                        // _KW_FSIN
        S__KW_FSINCOS = 135,                     // _KW_FSINCOS
        S__KW_FSQRT = 136,                       // _KW_FSQRT
        S__KW_FST = 137,                         // _KW_FST
        S__KW_FSTCW = 138,                       // _KW_FSTCW
        S__KW_FSTENV = 139,                      // _KW_FSTENV
        S__KW_FSTP = 140,                        // _KW_FSTP
        S__KW_FSTSW = 141,                       // _KW_FSTSW
        S__KW_FSUB = 142,                        // _KW_FSUB
        S__KW_FSUBP = 143,                       // _KW_FSUBP
        S__KW_FSUBR = 144,                       // _KW_FSUBR
        S__KW_FSUBRP = 145,                      // _KW_FSUBRP
        S__KW_FTST = 146,                        // _KW_FTST
        S__KW_FUCOM = 147,                       // _KW_FUCOM
        S__KW_FUCOMP = 148,                      // _KW_FUCOMP
        S__KW_FUCOMPP = 149,                     // _KW_FUCOMPP
        S__KW_FXAM = 150,                        // _KW_FXAM
        S__KW_FXCH = 151,                        // _KW_FXCH
        S__KW_FXTRACT = 152,                     // _KW_FXTRACT
        S__KW_FYL2X = 153,                       // _KW_FYL2X
        S__KW_FYL2XP1 = 154,                     // _KW_FYL2XP1
        S__KW_GLOBAL = 155,                      // _KW_GLOBAL
        S__KW_GS = 156,                          // _KW_GS
        S__KW_HLT = 157,                         // _KW_HLT
        S__KW_IDIV = 158,                        // _KW_IDIV
        S__KW_IMUL = 159,                        // _KW_IMUL
        S__KW_IN = 160,                          // _KW_IN
        S__KW_INC = 161,                         // _KW_INC
        S__KW_INCO = 162,                        // _KW_INCO
        S__KW_INSB = 163,                        // _KW_INSB
        S__KW_INSD = 164,                        // _KW_INSD
        S__KW_INSTRSET = 165,                    // _KW_INSTRSET
        S__KW_INSW = 166,                        // _KW_INSW
        S__KW_INT = 167,                         // _KW_INT
        S__KW_INT3 = 168,                        // _KW_INT3
        S__KW_INTO = 169,                        // _KW_INTO
        S__KW_INVD = 170,                        // _KW_INVD
        S__KW_INVLPG = 171,                      // _KW_INVLPG
        S__KW_IRET = 172,                        // _KW_IRET
        S__KW_IRETD = 173,                       // _KW_IRETD
        S__KW_IRETW = 174,                       // _KW_IRETW
        S__KW_JA = 175,                          // _KW_JA
        S__KW_JAE = 176,                         // _KW_JAE
        S__KW_JB = 177,                          // _KW_JB
        S__KW_JBE = 178,                         // _KW_JBE
        S__KW_JC = 179,                          // _KW_JC
        S__KW_JCXZ = 180,                        // _KW_JCXZ
        S__KW_JE = 181,                          // _KW_JE
        S__KW_JECXZ = 182,                       // _KW_JECXZ
        S__KW_JG = 183,                          // _KW_JG
        S__KW_JGE = 184,                         // _KW_JGE
        S__KW_JL = 185,                          // _KW_JL
        S__KW_JLE = 186,                         // _KW_JLE
        S__KW_JMP = 187,                         // _KW_JMP
        S__KW_JNA = 188,                         // _KW_JNA
        S__KW_JNAE = 189,                        // _KW_JNAE
        S__KW_JNB = 190,                         // _KW_JNB
        S__KW_JNBE = 191,                        // _KW_JNBE
        S__KW_JNC = 192,                         // _KW_JNC
        S__KW_JNE = 193,                         // _KW_JNE
        S__KW_JNG = 194,                         // _KW_JNG
        S__KW_JNGE = 195,                        // _KW_JNGE
        S__KW_JNL = 196,                         // _KW_JNL
        S__KW_JNLE = 197,                        // _KW_JNLE
        S__KW_JNO = 198,                         // _KW_JNO
        S__KW_JNP = 199,                         // _KW_JNP
        S__KW_JNS = 200,                         // _KW_JNS
        S__KW_JNZ = 201,                         // _KW_JNZ
        S__KW_JO = 202,                          // _KW_JO
        S__KW_JP = 203,                          // _KW_JP
        S__KW_JPE = 204,                         // _KW_JPE
        S__KW_JPO = 205,                         // _KW_JPO
        S__KW_JS = 206,                          // _KW_JS
        S__KW_JZ = 207,                          // _KW_JZ
        S__KW_LAHF = 208,                        // _KW_LAHF
        S__KW_LAR = 209,                         // _KW_LAR
        S__KW_LDS = 210,                         // _KW_LDS
        S__KW_LEA = 211,                         // _KW_LEA
        S__KW_LEAVE = 212,                       // _KW_LEAVE
        S__KW_LES = 213,                         // _KW_LES
        S__KW_LFS = 214,                         // _KW_LFS
        S__KW_LGDT = 215,                        // _KW_LGDT
        S__KW_LGS = 216,                         // _KW_LGS
        S__KW_LIDT = 217,                        // _KW_LIDT
        S__KW_LLDT = 218,                        // _KW_LLDT
        S__KW_LMSW = 219,                        // _KW_LMSW
        S__KW_LOCK = 220,                        // _KW_LOCK
        S__KW_LODSB = 221,                       // _KW_LODSB
        S__KW_LODSD = 222,                       // _KW_LODSD
        S__KW_LODSW = 223,                       // _KW_LODSW
        S__KW_LOOP = 224,                        // _KW_LOOP
        S__KW_LOOPE = 225,                       // _KW_LOOPE
        S__KW_LOOPNE = 226,                      // _KW_LOOPNE
        S__KW_LOOPNZ = 227,                      // _KW_LOOPNZ
        S__KW_LOOPZ = 228,                       // _KW_LOOPZ
        S__KW_LSL = 229,                         // _KW_LSL
        S__KW_LSS = 230,                         // _KW_LSS
        S__KW_LTR = 231,                         // _KW_LTR
        S__KW_MOV = 232,                         // _KW_MOV
        S__KW_MOVSB = 233,                       // _KW_MOVSB
        S__KW_MOVSD = 234,                       // _KW_MOVSD
        S__KW_MOVSW = 235,                       // _KW_MOVSW
        S__KW_MOVSX = 236,                       // _KW_MOVSX
        S__KW_MOVZX = 237,                       // _KW_MOVZX
        S__KW_MUL = 238,                         // _KW_MUL
        S__KW_NEAR = 239,                        // _KW_NEAR
        S__KW_NEG = 240,                         // _KW_NEG
        S__KW_NOP = 241,                         // _KW_NOP
        S__KW_NOT = 242,                         // _KW_NOT
        S__KW_OPTIMIZE = 243,                    // _KW_OPTIMIZE
        S__KW_OR = 244,                          // _KW_OR
        S__KW_ORG = 245,                         // _KW_ORG
        S__KW_OUT = 246,                         // _KW_OUT
        S__KW_OUTSB = 247,                       // _KW_OUTSB
        S__KW_OUTSD = 248,                       // _KW_OUTSD
        S__KW_OUTSW = 249,                       // _KW_OUTSW
        S__KW_PADDING = 250,                     // _KW_PADDING
        S__KW_PADSET = 251,                      // _KW_PADSET
        S__KW_POP = 252,                         // _KW_POP
        S__KW_POPA = 253,                        // _KW_POPA
        S__KW_POPAD = 254,                       // _KW_POPAD
        S__KW_POPAW = 255,                       // _KW_POPAW
        S__KW_POPF = 256,                        // _KW_POPF
        S__KW_POPFD = 257,                       // _KW_POPFD
        S__KW_POPFW = 258,                       // _KW_POPFW
        S__KW_PUSH = 259,                        // _KW_PUSH
        S__KW_PUSHA = 260,                       // _KW_PUSHA
        S__KW_PUSHAD = 261,                      // _KW_PUSHAD
        S__KW_PUSHAW = 262,                      // _KW_PUSHAW
        S__KW_PUSHD = 263,                       // _KW_PUSHD
        S__KW_PUSHF = 264,                       // _KW_PUSHF
        S__KW_PUSHFD = 265,                      // _KW_PUSHFD
        S__KW_PUSHFW = 266,                      // _KW_PUSHFW
        S__KW_RCL = 267,                         // _KW_RCL
        S__KW_RCR = 268,                         // _KW_RCR
        S__KW_RDMSR = 269,                       // _KW_RDMSR
        S__KW_RDPMC = 270,                       // _KW_RDPMC
        S__KW_REP = 271,                         // _KW_REP
        S__KW_REPE = 272,                        // _KW_REPE
        S__KW_REPNE = 273,                       // _KW_REPNE
        S__KW_REPNZ = 274,                       // _KW_REPNZ
        S__KW_REPZ = 275,                        // _KW_REPZ
        S__KW_RESB = 276,                        // _KW_RESB
        S__KW_RESD = 277,                        // _KW_RESD
        S__KW_RESQ = 278,                        // _KW_RESQ
        S__KW_REST = 279,                        // _KW_REST
        S__KW_RESW = 280,                        // _KW_RESW
        S__KW_RET = 281,                         // _KW_RET
        S__KW_RETF = 282,                        // _KW_RETF
        S__KW_RETN = 283,                        // _KW_RETN
        S__KW_ROL = 284,                         // _KW_ROL
        S__KW_ROR = 285,                         // _KW_ROR
        S__KW_RSM = 286,                         // _KW_RSM
        S__KW_SAHF = 287,                        // _KW_SAHF
        S__KW_SAL = 288,                         // _KW_SAL
        S__KW_SAR = 289,                         // _KW_SAR
        S__KW_SBB = 290,                         // _KW_SBB
        S__KW_SCASB = 291,                       // _KW_SCASB
        S__KW_SCASD = 292,                       // _KW_SCASD
        S__KW_SCASW = 293,                       // _KW_SCASW
        S__KW_SECTION = 294,                     // _KW_SECTION
        S__KW_SETA = 295,                        // _KW_SETA
        S__KW_SETAE = 296,                       // _KW_SETAE
        S__KW_SETB = 297,                        // _KW_SETB
        S__KW_SETBE = 298,                       // _KW_SETBE
        S__KW_SETC = 299,                        // _KW_SETC
        S__KW_SETE = 300,                        // _KW_SETE
        S__KW_SETG = 301,                        // _KW_SETG
        S__KW_SETGE = 302,                       // _KW_SETGE
        S__KW_SETL = 303,                        // _KW_SETL
        S__KW_SETLE = 304,                       // _KW_SETLE
        S__KW_SETNA = 305,                       // _KW_SETNA
        S__KW_SETNAE = 306,                      // _KW_SETNAE
        S__KW_SETNB = 307,                       // _KW_SETNB
        S__KW_SETNBE = 308,                      // _KW_SETNBE
        S__KW_SETNC = 309,                       // _KW_SETNC
        S__KW_SETNE = 310,                       // _KW_SETNE
        S__KW_SETNG = 311,                       // _KW_SETNG
        S__KW_SETNGE = 312,                      // _KW_SETNGE
        S__KW_SETNL = 313,                       // _KW_SETNL
        S__KW_SETNLE = 314,                      // _KW_SETNLE
        S__KW_SETNO = 315,                       // _KW_SETNO
        S__KW_SETNP = 316,                       // _KW_SETNP
        S__KW_SETNS = 317,                       // _KW_SETNS
        S__KW_SETNZ = 318,                       // _KW_SETNZ
        S__KW_SETO = 319,                        // _KW_SETO
        S__KW_SETP = 320,                        // _KW_SETP
        S__KW_SETPE = 321,                       // _KW_SETPE
        S__KW_SETPO = 322,                       // _KW_SETPO
        S__KW_SETS = 323,                        // _KW_SETS
        S__KW_SETZ = 324,                        // _KW_SETZ
        S__KW_SGDT = 325,                        // _KW_SGDT
        S__KW_SHL = 326,                         // _KW_SHL
        S__KW_SHLD = 327,                        // _KW_SHLD
        S__KW_SHORT = 328,                       // _KW_SHORT
        S__KW_SHR = 329,                         // _KW_SHR
        S__KW_SHRD = 330,                        // _KW_SHRD
        S__KW_SIDT = 331,                        // _KW_SIDT
        S__KW_SLDT = 332,                        // _KW_SLDT
        S__KW_SMSW = 333,                        // _KW_SMSW
        S__KW_SS = 334,                          // _KW_SS
        S__KW_STC = 335,                         // _KW_STC
        S__KW_STD = 336,                         // _KW_STD
        S__KW_STI = 337,                         // _KW_STI
        S__KW_STOSB = 338,                       // _KW_STOSB
        S__KW_STOSD = 339,                       // _KW_STOSD
        S__KW_STOSW = 340,                       // _KW_STOSW
        S__KW_STR = 341,                         // _KW_STR
        S__KW_SUB = 342,                         // _KW_SUB
        S__KW_TEST = 343,                        // _KW_TEST
        S__KW_TIMES = 344,                       // _KW_TIMES
        S__KW_UD2 = 345,                         // _KW_UD2
        S__KW_VERR = 346,                        // _KW_VERR
        S__KW_VERW = 347,                        // _KW_VERW
        S__KW_WAIT = 348,                        // _KW_WAIT
        S__KW_WBINVD = 349,                      // _KW_WBINVD
        S__KW_WORD = 350,                        // _KW_WORD
        S__KW_WRMSR = 351,                       // _KW_WRMSR
        S__KW_XADD = 352,                        // _KW_XADD
        S__KW_XCHG = 353,                        // _KW_XCHG
        S__KW_XLATB = 354,                       // _KW_XLATB
        S__KW_XOR = 355,                         // _KW_XOR
        S__LBRACK = 356,                         // _LBRACK
        S__RBRACK = 357,                         // _RBRACK
        S_T_Hex = 358,                           // T_Hex
        S_T_Id = 359,                            // T_Id
        S_T_NaskChar = 360,                      // T_NaskChar
        S__STRING_ = 361,                        // _STRING_
        S__INTEGER_ = 362,                       // _INTEGER_
        S_YYACCEPT = 363,                        // $accept
        S_Program = 364,                         // Program
        S_ListStatement = 365,                   // ListStatement
        S_Statement = 366,                       // Statement
        S_Statement1 = 367,                      // Statement1
        S_ListFactor = 368,                      // ListFactor
        S_ListMnemonicArgs = 369,                // ListMnemonicArgs
        S_MnemonicArgs = 370,                    // MnemonicArgs
        S_Exp = 371,                             // Exp
        S_Exp1 = 372,                            // Exp1
        S_Label = 373,                           // Label
        S_Exp2 = 374,                            // Exp2
        S_Exp3 = 375,                            // Exp3
        S_Exp4 = 376,                            // Exp4
        S_MemoryAddr = 377,                      // MemoryAddr
        S_IndexExp = 378,                        // IndexExp
        S_UnaryOperator = 379,                   // UnaryOperator
        S_Factor = 380,                          // Factor
        S_JumpDir = 381,                         // JumpDir
        S_SReg = 382,                            // SReg
        S_ConfigType = 383,                      // ConfigType
        S_DataType = 384,                        // DataType
        S_OpcodeNoParam = 385,                   // OpcodeNoParam
        S_Opcode = 386                           // Opcode
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.move< int > (std::move (that.value));
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.move< std::shared_ptr<ConfigType> > (std::move (that.value));
        break;

      case symbol_kind::S_DataType: // DataType
        value.move< std::shared_ptr<DataType> > (std::move (that.value));
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
      case symbol_kind::S_Exp3: // Exp3
      case symbol_kind::S_Exp4: // Exp4
        value.move< std::shared_ptr<Exp> > (std::move (that.value));
        break;

      case symbol_kind::S_Factor: // Factor
        value.move< std::shared_ptr<Factor> > (std::move (that.value));
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.move< std::shared_ptr<IndexExp> > (std::move (that.value));
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.move< std::shared_ptr<JumpDir> > (std::move (that.value));
        break;

      case symbol_kind::S_Label: // Label
        value.move< std::shared_ptr<Label> > (std::move (that.value));
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.move< std::shared_ptr<ListFactor> > (std::move (that.value));
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.move< std::shared_ptr<ListMnemonicArgs> > (std::move (that.value));
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.move< std::shared_ptr<ListStatement> > (std::move (that.value));
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.move< std::shared_ptr<MemoryAddr> > (std::move (that.value));
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.move< std::shared_ptr<MnemonicArgs> > (std::move (that.value));
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.move< std::shared_ptr<Opcode> > (std::move (that.value));
        break;

      case symbol_kind::S_OpcodeNoParam: // OpcodeNoParam
        value.move< std::shared_ptr<OpcodeNoParam> > (std::move (that.value));
        break;

      case symbol_kind::S_Program: // Program
        value.move< std::shared_ptr<Program> > (std::move (that.value));
        break;

      case symbol_kind::S_SReg: // SReg
        value.move< std::shared_ptr<SReg> > (std::move (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_Statement1: // Statement1
        value.move< std::shared_ptr<Statement> > (std::move (that.value));
        break;

      case symbol_kind::S_UnaryOperator: // UnaryOperator
        value.move< std::shared_ptr<UnaryOperator> > (std::move (that.value));
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<ConfigType>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<ConfigType>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<DataType>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<DataType>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<Exp>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<Exp>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<Factor>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<Factor>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<IndexExp>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<IndexExp>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<JumpDir>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<JumpDir>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<Label>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<Label>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<ListFactor>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<ListFactor>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<ListMnemonicArgs>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<ListMnemonicArgs>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<ListStatement>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<ListStatement>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<MemoryAddr>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<MemoryAddr>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<MnemonicArgs>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<MnemonicArgs>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<Opcode>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<Opcode>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<OpcodeNoParam>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<OpcodeNoParam>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<Program>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<Program>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<SReg>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<SReg>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<Statement>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<Statement>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<UnaryOperator>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<UnaryOperator>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.template destroy< int > ();
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.template destroy< std::shared_ptr<ConfigType> > ();
        break;

      case symbol_kind::S_DataType: // DataType
        value.template destroy< std::shared_ptr<DataType> > ();
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
      case symbol_kind::S_Exp3: // Exp3
      case symbol_kind::S_Exp4: // Exp4
        value.template destroy< std::shared_ptr<Exp> > ();
        break;

      case symbol_kind::S_Factor: // Factor
        value.template destroy< std::shared_ptr<Factor> > ();
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.template destroy< std::shared_ptr<IndexExp> > ();
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.template destroy< std::shared_ptr<JumpDir> > ();
        break;

      case symbol_kind::S_Label: // Label
        value.template destroy< std::shared_ptr<Label> > ();
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.template destroy< std::shared_ptr<ListFactor> > ();
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.template destroy< std::shared_ptr<ListMnemonicArgs> > ();
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.template destroy< std::shared_ptr<ListStatement> > ();
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.template destroy< std::shared_ptr<MemoryAddr> > ();
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.template destroy< std::shared_ptr<MnemonicArgs> > ();
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.template destroy< std::shared_ptr<Opcode> > ();
        break;

      case symbol_kind::S_OpcodeNoParam: // OpcodeNoParam
        value.template destroy< std::shared_ptr<OpcodeNoParam> > ();
        break;

      case symbol_kind::S_Program: // Program
        value.template destroy< std::shared_ptr<Program> > ();
        break;

      case symbol_kind::S_SReg: // SReg
        value.template destroy< std::shared_ptr<SReg> > ();
        break;

      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_Statement1: // Statement1
        value.template destroy< std::shared_ptr<Statement> > ();
        break;

      case symbol_kind::S_UnaryOperator: // UnaryOperator
        value.template destroy< std::shared_ptr<UnaryOperator> > ();
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

#if YYDEBUG || 0
      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return NaskParser::symbol_name (this->kind ());
      }
#endif // #if YYDEBUG || 0


      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {}
    };

    /// Build a parser object.
    NaskParser (NaskScanner  &scanner_yyarg, NaskDriver  &driver_yyarg);
    virtual ~NaskParser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    NaskParser (const NaskParser&) = delete;
    /// Non copyable.
    NaskParser& operator= (const NaskParser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

#if YYDEBUG || 0
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if YYDEBUG || 0


    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF (location_type l)
      {
        return symbol_type (token::YYEOF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYEOF (const location_type& l)
      {
        return symbol_type (token::YYEOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__ERROR_ (location_type l)
      {
        return symbol_type (token::_ERROR_, std::move (l));
      }
#else
      static
      symbol_type
      make__ERROR_ (const location_type& l)
      {
        return symbol_type (token::_ERROR_, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__PERCENT (location_type l)
      {
        return symbol_type (token::_PERCENT, std::move (l));
      }
#else
      static
      symbol_type
      make__PERCENT (const location_type& l)
      {
        return symbol_type (token::_PERCENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__LPAREN (location_type l)
      {
        return symbol_type (token::_LPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make__LPAREN (const location_type& l)
      {
        return symbol_type (token::_LPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__RPAREN (location_type l)
      {
        return symbol_type (token::_RPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make__RPAREN (const location_type& l)
      {
        return symbol_type (token::_RPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__STAR (location_type l)
      {
        return symbol_type (token::_STAR, std::move (l));
      }
#else
      static
      symbol_type
      make__STAR (const location_type& l)
      {
        return symbol_type (token::_STAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__PLUS (location_type l)
      {
        return symbol_type (token::_PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make__PLUS (const location_type& l)
      {
        return symbol_type (token::_PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__COMMA (location_type l)
      {
        return symbol_type (token::_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make__COMMA (const location_type& l)
      {
        return symbol_type (token::_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__MINUS (location_type l)
      {
        return symbol_type (token::_MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make__MINUS (const location_type& l)
      {
        return symbol_type (token::_MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__SLASH (location_type l)
      {
        return symbol_type (token::_SLASH, std::move (l));
      }
#else
      static
      symbol_type
      make__SLASH (const location_type& l)
      {
        return symbol_type (token::_SLASH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__COLON (location_type l)
      {
        return symbol_type (token::_COLON, std::move (l));
      }
#else
      static
      symbol_type
      make__COLON (const location_type& l)
      {
        return symbol_type (token::_COLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_AAA (location_type l)
      {
        return symbol_type (token::_KW_AAA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_AAA (const location_type& l)
      {
        return symbol_type (token::_KW_AAA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_AAD (location_type l)
      {
        return symbol_type (token::_KW_AAD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_AAD (const location_type& l)
      {
        return symbol_type (token::_KW_AAD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_AAM (location_type l)
      {
        return symbol_type (token::_KW_AAM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_AAM (const location_type& l)
      {
        return symbol_type (token::_KW_AAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_AAS (location_type l)
      {
        return symbol_type (token::_KW_AAS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_AAS (const location_type& l)
      {
        return symbol_type (token::_KW_AAS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ABSOLUTE (location_type l)
      {
        return symbol_type (token::_KW_ABSOLUTE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ABSOLUTE (const location_type& l)
      {
        return symbol_type (token::_KW_ABSOLUTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ADC (location_type l)
      {
        return symbol_type (token::_KW_ADC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ADC (const location_type& l)
      {
        return symbol_type (token::_KW_ADC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ADD (location_type l)
      {
        return symbol_type (token::_KW_ADD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ADD (const location_type& l)
      {
        return symbol_type (token::_KW_ADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ALIGN (location_type l)
      {
        return symbol_type (token::_KW_ALIGN, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ALIGN (const location_type& l)
      {
        return symbol_type (token::_KW_ALIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ALIGNB (location_type l)
      {
        return symbol_type (token::_KW_ALIGNB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ALIGNB (const location_type& l)
      {
        return symbol_type (token::_KW_ALIGNB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_AND (location_type l)
      {
        return symbol_type (token::_KW_AND, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_AND (const location_type& l)
      {
        return symbol_type (token::_KW_AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ARPL (location_type l)
      {
        return symbol_type (token::_KW_ARPL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ARPL (const location_type& l)
      {
        return symbol_type (token::_KW_ARPL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BITS (location_type l)
      {
        return symbol_type (token::_KW_BITS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BITS (const location_type& l)
      {
        return symbol_type (token::_KW_BITS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BOUND (location_type l)
      {
        return symbol_type (token::_KW_BOUND, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BOUND (const location_type& l)
      {
        return symbol_type (token::_KW_BOUND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BSF (location_type l)
      {
        return symbol_type (token::_KW_BSF, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BSF (const location_type& l)
      {
        return symbol_type (token::_KW_BSF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BSR (location_type l)
      {
        return symbol_type (token::_KW_BSR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BSR (const location_type& l)
      {
        return symbol_type (token::_KW_BSR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BSWAP (location_type l)
      {
        return symbol_type (token::_KW_BSWAP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BSWAP (const location_type& l)
      {
        return symbol_type (token::_KW_BSWAP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BT (location_type l)
      {
        return symbol_type (token::_KW_BT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BT (const location_type& l)
      {
        return symbol_type (token::_KW_BT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BTC (location_type l)
      {
        return symbol_type (token::_KW_BTC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BTC (const location_type& l)
      {
        return symbol_type (token::_KW_BTC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BTR (location_type l)
      {
        return symbol_type (token::_KW_BTR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BTR (const location_type& l)
      {
        return symbol_type (token::_KW_BTR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BTS (location_type l)
      {
        return symbol_type (token::_KW_BTS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BTS (const location_type& l)
      {
        return symbol_type (token::_KW_BTS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_BYTE (location_type l)
      {
        return symbol_type (token::_KW_BYTE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_BYTE (const location_type& l)
      {
        return symbol_type (token::_KW_BYTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CALL (location_type l)
      {
        return symbol_type (token::_KW_CALL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CALL (const location_type& l)
      {
        return symbol_type (token::_KW_CALL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CBW (location_type l)
      {
        return symbol_type (token::_KW_CBW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CBW (const location_type& l)
      {
        return symbol_type (token::_KW_CBW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CDQ (location_type l)
      {
        return symbol_type (token::_KW_CDQ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CDQ (const location_type& l)
      {
        return symbol_type (token::_KW_CDQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CLC (location_type l)
      {
        return symbol_type (token::_KW_CLC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CLC (const location_type& l)
      {
        return symbol_type (token::_KW_CLC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CLD (location_type l)
      {
        return symbol_type (token::_KW_CLD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CLD (const location_type& l)
      {
        return symbol_type (token::_KW_CLD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CLI (location_type l)
      {
        return symbol_type (token::_KW_CLI, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CLI (const location_type& l)
      {
        return symbol_type (token::_KW_CLI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CLTS (location_type l)
      {
        return symbol_type (token::_KW_CLTS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CLTS (const location_type& l)
      {
        return symbol_type (token::_KW_CLTS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CMC (location_type l)
      {
        return symbol_type (token::_KW_CMC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CMC (const location_type& l)
      {
        return symbol_type (token::_KW_CMC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CMP (location_type l)
      {
        return symbol_type (token::_KW_CMP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CMP (const location_type& l)
      {
        return symbol_type (token::_KW_CMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CMPSB (location_type l)
      {
        return symbol_type (token::_KW_CMPSB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CMPSB (const location_type& l)
      {
        return symbol_type (token::_KW_CMPSB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CMPSD (location_type l)
      {
        return symbol_type (token::_KW_CMPSD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CMPSD (const location_type& l)
      {
        return symbol_type (token::_KW_CMPSD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CMPSW (location_type l)
      {
        return symbol_type (token::_KW_CMPSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CMPSW (const location_type& l)
      {
        return symbol_type (token::_KW_CMPSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CMPXCHG (location_type l)
      {
        return symbol_type (token::_KW_CMPXCHG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CMPXCHG (const location_type& l)
      {
        return symbol_type (token::_KW_CMPXCHG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CPUID (location_type l)
      {
        return symbol_type (token::_KW_CPUID, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CPUID (const location_type& l)
      {
        return symbol_type (token::_KW_CPUID, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CS (location_type l)
      {
        return symbol_type (token::_KW_CS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CS (const location_type& l)
      {
        return symbol_type (token::_KW_CS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CWD (location_type l)
      {
        return symbol_type (token::_KW_CWD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CWD (const location_type& l)
      {
        return symbol_type (token::_KW_CWD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_CWDE (location_type l)
      {
        return symbol_type (token::_KW_CWDE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_CWDE (const location_type& l)
      {
        return symbol_type (token::_KW_CWDE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DAA (location_type l)
      {
        return symbol_type (token::_KW_DAA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DAA (const location_type& l)
      {
        return symbol_type (token::_KW_DAA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DAS (location_type l)
      {
        return symbol_type (token::_KW_DAS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DAS (const location_type& l)
      {
        return symbol_type (token::_KW_DAS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DB (location_type l)
      {
        return symbol_type (token::_KW_DB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DB (const location_type& l)
      {
        return symbol_type (token::_KW_DB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DD (location_type l)
      {
        return symbol_type (token::_KW_DD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DD (const location_type& l)
      {
        return symbol_type (token::_KW_DD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DEC (location_type l)
      {
        return symbol_type (token::_KW_DEC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DEC (const location_type& l)
      {
        return symbol_type (token::_KW_DEC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DIV (location_type l)
      {
        return symbol_type (token::_KW_DIV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DIV (const location_type& l)
      {
        return symbol_type (token::_KW_DIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DQ (location_type l)
      {
        return symbol_type (token::_KW_DQ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DQ (const location_type& l)
      {
        return symbol_type (token::_KW_DQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DS (location_type l)
      {
        return symbol_type (token::_KW_DS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DS (const location_type& l)
      {
        return symbol_type (token::_KW_DS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DT (location_type l)
      {
        return symbol_type (token::_KW_DT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DT (const location_type& l)
      {
        return symbol_type (token::_KW_DT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DW (location_type l)
      {
        return symbol_type (token::_KW_DW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DW (const location_type& l)
      {
        return symbol_type (token::_KW_DW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_DWORD (location_type l)
      {
        return symbol_type (token::_KW_DWORD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_DWORD (const location_type& l)
      {
        return symbol_type (token::_KW_DWORD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_END (location_type l)
      {
        return symbol_type (token::_KW_END, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_END (const location_type& l)
      {
        return symbol_type (token::_KW_END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ENTER (location_type l)
      {
        return symbol_type (token::_KW_ENTER, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ENTER (const location_type& l)
      {
        return symbol_type (token::_KW_ENTER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_EQU (location_type l)
      {
        return symbol_type (token::_KW_EQU, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_EQU (const location_type& l)
      {
        return symbol_type (token::_KW_EQU, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ES (location_type l)
      {
        return symbol_type (token::_KW_ES, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ES (const location_type& l)
      {
        return symbol_type (token::_KW_ES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_EXTERN (location_type l)
      {
        return symbol_type (token::_KW_EXTERN, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_EXTERN (const location_type& l)
      {
        return symbol_type (token::_KW_EXTERN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_F2XM1 (location_type l)
      {
        return symbol_type (token::_KW_F2XM1, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_F2XM1 (const location_type& l)
      {
        return symbol_type (token::_KW_F2XM1, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FABS (location_type l)
      {
        return symbol_type (token::_KW_FABS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FABS (const location_type& l)
      {
        return symbol_type (token::_KW_FABS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FADD (location_type l)
      {
        return symbol_type (token::_KW_FADD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FADD (const location_type& l)
      {
        return symbol_type (token::_KW_FADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FADDP (location_type l)
      {
        return symbol_type (token::_KW_FADDP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FADDP (const location_type& l)
      {
        return symbol_type (token::_KW_FADDP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FAR (location_type l)
      {
        return symbol_type (token::_KW_FAR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FAR (const location_type& l)
      {
        return symbol_type (token::_KW_FAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FBLD (location_type l)
      {
        return symbol_type (token::_KW_FBLD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FBLD (const location_type& l)
      {
        return symbol_type (token::_KW_FBLD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FBSTP (location_type l)
      {
        return symbol_type (token::_KW_FBSTP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FBSTP (const location_type& l)
      {
        return symbol_type (token::_KW_FBSTP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FCHS (location_type l)
      {
        return symbol_type (token::_KW_FCHS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FCHS (const location_type& l)
      {
        return symbol_type (token::_KW_FCHS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FCLEX (location_type l)
      {
        return symbol_type (token::_KW_FCLEX, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FCLEX (const location_type& l)
      {
        return symbol_type (token::_KW_FCLEX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FCOM (location_type l)
      {
        return symbol_type (token::_KW_FCOM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FCOM (const location_type& l)
      {
        return symbol_type (token::_KW_FCOM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FCOMP (location_type l)
      {
        return symbol_type (token::_KW_FCOMP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FCOMP (const location_type& l)
      {
        return symbol_type (token::_KW_FCOMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FCOMPP (location_type l)
      {
        return symbol_type (token::_KW_FCOMPP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FCOMPP (const location_type& l)
      {
        return symbol_type (token::_KW_FCOMPP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FCOS (location_type l)
      {
        return symbol_type (token::_KW_FCOS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FCOS (const location_type& l)
      {
        return symbol_type (token::_KW_FCOS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FDECSTP (location_type l)
      {
        return symbol_type (token::_KW_FDECSTP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FDECSTP (const location_type& l)
      {
        return symbol_type (token::_KW_FDECSTP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FDISI (location_type l)
      {
        return symbol_type (token::_KW_FDISI, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FDISI (const location_type& l)
      {
        return symbol_type (token::_KW_FDISI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FDIV (location_type l)
      {
        return symbol_type (token::_KW_FDIV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FDIV (const location_type& l)
      {
        return symbol_type (token::_KW_FDIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FDIVP (location_type l)
      {
        return symbol_type (token::_KW_FDIVP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FDIVP (const location_type& l)
      {
        return symbol_type (token::_KW_FDIVP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FDIVR (location_type l)
      {
        return symbol_type (token::_KW_FDIVR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FDIVR (const location_type& l)
      {
        return symbol_type (token::_KW_FDIVR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FDIVRP (location_type l)
      {
        return symbol_type (token::_KW_FDIVRP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FDIVRP (const location_type& l)
      {
        return symbol_type (token::_KW_FDIVRP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FENI (location_type l)
      {
        return symbol_type (token::_KW_FENI, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FENI (const location_type& l)
      {
        return symbol_type (token::_KW_FENI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FFREE (location_type l)
      {
        return symbol_type (token::_KW_FFREE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FFREE (const location_type& l)
      {
        return symbol_type (token::_KW_FFREE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FIADD (location_type l)
      {
        return symbol_type (token::_KW_FIADD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FIADD (const location_type& l)
      {
        return symbol_type (token::_KW_FIADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FICOM (location_type l)
      {
        return symbol_type (token::_KW_FICOM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FICOM (const location_type& l)
      {
        return symbol_type (token::_KW_FICOM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FICOMP (location_type l)
      {
        return symbol_type (token::_KW_FICOMP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FICOMP (const location_type& l)
      {
        return symbol_type (token::_KW_FICOMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FIDIV (location_type l)
      {
        return symbol_type (token::_KW_FIDIV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FIDIV (const location_type& l)
      {
        return symbol_type (token::_KW_FIDIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FIDIVR (location_type l)
      {
        return symbol_type (token::_KW_FIDIVR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FIDIVR (const location_type& l)
      {
        return symbol_type (token::_KW_FIDIVR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FILD (location_type l)
      {
        return symbol_type (token::_KW_FILD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FILD (const location_type& l)
      {
        return symbol_type (token::_KW_FILD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FILE (location_type l)
      {
        return symbol_type (token::_KW_FILE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FILE (const location_type& l)
      {
        return symbol_type (token::_KW_FILE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FIMUL (location_type l)
      {
        return symbol_type (token::_KW_FIMUL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FIMUL (const location_type& l)
      {
        return symbol_type (token::_KW_FIMUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FINCSTP (location_type l)
      {
        return symbol_type (token::_KW_FINCSTP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FINCSTP (const location_type& l)
      {
        return symbol_type (token::_KW_FINCSTP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FINIT (location_type l)
      {
        return symbol_type (token::_KW_FINIT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FINIT (const location_type& l)
      {
        return symbol_type (token::_KW_FINIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FIST (location_type l)
      {
        return symbol_type (token::_KW_FIST, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FIST (const location_type& l)
      {
        return symbol_type (token::_KW_FIST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FISTP (location_type l)
      {
        return symbol_type (token::_KW_FISTP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FISTP (const location_type& l)
      {
        return symbol_type (token::_KW_FISTP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FISUB (location_type l)
      {
        return symbol_type (token::_KW_FISUB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FISUB (const location_type& l)
      {
        return symbol_type (token::_KW_FISUB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FISUBR (location_type l)
      {
        return symbol_type (token::_KW_FISUBR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FISUBR (const location_type& l)
      {
        return symbol_type (token::_KW_FISUBR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLD (location_type l)
      {
        return symbol_type (token::_KW_FLD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLD (const location_type& l)
      {
        return symbol_type (token::_KW_FLD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLD1 (location_type l)
      {
        return symbol_type (token::_KW_FLD1, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLD1 (const location_type& l)
      {
        return symbol_type (token::_KW_FLD1, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDCW (location_type l)
      {
        return symbol_type (token::_KW_FLDCW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDCW (const location_type& l)
      {
        return symbol_type (token::_KW_FLDCW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDENV (location_type l)
      {
        return symbol_type (token::_KW_FLDENV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDENV (const location_type& l)
      {
        return symbol_type (token::_KW_FLDENV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDL2E (location_type l)
      {
        return symbol_type (token::_KW_FLDL2E, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDL2E (const location_type& l)
      {
        return symbol_type (token::_KW_FLDL2E, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDL2T (location_type l)
      {
        return symbol_type (token::_KW_FLDL2T, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDL2T (const location_type& l)
      {
        return symbol_type (token::_KW_FLDL2T, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDLG2 (location_type l)
      {
        return symbol_type (token::_KW_FLDLG2, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDLG2 (const location_type& l)
      {
        return symbol_type (token::_KW_FLDLG2, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDLN2 (location_type l)
      {
        return symbol_type (token::_KW_FLDLN2, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDLN2 (const location_type& l)
      {
        return symbol_type (token::_KW_FLDLN2, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDPI (location_type l)
      {
        return symbol_type (token::_KW_FLDPI, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDPI (const location_type& l)
      {
        return symbol_type (token::_KW_FLDPI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FLDZ (location_type l)
      {
        return symbol_type (token::_KW_FLDZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FLDZ (const location_type& l)
      {
        return symbol_type (token::_KW_FLDZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FMUL (location_type l)
      {
        return symbol_type (token::_KW_FMUL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FMUL (const location_type& l)
      {
        return symbol_type (token::_KW_FMUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FMULP (location_type l)
      {
        return symbol_type (token::_KW_FMULP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FMULP (const location_type& l)
      {
        return symbol_type (token::_KW_FMULP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNCLEX (location_type l)
      {
        return symbol_type (token::_KW_FNCLEX, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNCLEX (const location_type& l)
      {
        return symbol_type (token::_KW_FNCLEX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNDISI (location_type l)
      {
        return symbol_type (token::_KW_FNDISI, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNDISI (const location_type& l)
      {
        return symbol_type (token::_KW_FNDISI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNENI (location_type l)
      {
        return symbol_type (token::_KW_FNENI, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNENI (const location_type& l)
      {
        return symbol_type (token::_KW_FNENI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNINIT (location_type l)
      {
        return symbol_type (token::_KW_FNINIT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNINIT (const location_type& l)
      {
        return symbol_type (token::_KW_FNINIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNOP (location_type l)
      {
        return symbol_type (token::_KW_FNOP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNOP (const location_type& l)
      {
        return symbol_type (token::_KW_FNOP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNSAVE (location_type l)
      {
        return symbol_type (token::_KW_FNSAVE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNSAVE (const location_type& l)
      {
        return symbol_type (token::_KW_FNSAVE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNSTCW (location_type l)
      {
        return symbol_type (token::_KW_FNSTCW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNSTCW (const location_type& l)
      {
        return symbol_type (token::_KW_FNSTCW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNSTENV (location_type l)
      {
        return symbol_type (token::_KW_FNSTENV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNSTENV (const location_type& l)
      {
        return symbol_type (token::_KW_FNSTENV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FNSTSW (location_type l)
      {
        return symbol_type (token::_KW_FNSTSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FNSTSW (const location_type& l)
      {
        return symbol_type (token::_KW_FNSTSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FORMAT (location_type l)
      {
        return symbol_type (token::_KW_FORMAT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FORMAT (const location_type& l)
      {
        return symbol_type (token::_KW_FORMAT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FPATAN (location_type l)
      {
        return symbol_type (token::_KW_FPATAN, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FPATAN (const location_type& l)
      {
        return symbol_type (token::_KW_FPATAN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FPREM (location_type l)
      {
        return symbol_type (token::_KW_FPREM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FPREM (const location_type& l)
      {
        return symbol_type (token::_KW_FPREM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FPREM1 (location_type l)
      {
        return symbol_type (token::_KW_FPREM1, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FPREM1 (const location_type& l)
      {
        return symbol_type (token::_KW_FPREM1, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FPTAN (location_type l)
      {
        return symbol_type (token::_KW_FPTAN, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FPTAN (const location_type& l)
      {
        return symbol_type (token::_KW_FPTAN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FRNDINT (location_type l)
      {
        return symbol_type (token::_KW_FRNDINT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FRNDINT (const location_type& l)
      {
        return symbol_type (token::_KW_FRNDINT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FRSTOR (location_type l)
      {
        return symbol_type (token::_KW_FRSTOR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FRSTOR (const location_type& l)
      {
        return symbol_type (token::_KW_FRSTOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FS (location_type l)
      {
        return symbol_type (token::_KW_FS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FS (const location_type& l)
      {
        return symbol_type (token::_KW_FS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSAVE (location_type l)
      {
        return symbol_type (token::_KW_FSAVE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSAVE (const location_type& l)
      {
        return symbol_type (token::_KW_FSAVE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSCALE (location_type l)
      {
        return symbol_type (token::_KW_FSCALE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSCALE (const location_type& l)
      {
        return symbol_type (token::_KW_FSCALE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSETPM (location_type l)
      {
        return symbol_type (token::_KW_FSETPM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSETPM (const location_type& l)
      {
        return symbol_type (token::_KW_FSETPM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSIN (location_type l)
      {
        return symbol_type (token::_KW_FSIN, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSIN (const location_type& l)
      {
        return symbol_type (token::_KW_FSIN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSINCOS (location_type l)
      {
        return symbol_type (token::_KW_FSINCOS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSINCOS (const location_type& l)
      {
        return symbol_type (token::_KW_FSINCOS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSQRT (location_type l)
      {
        return symbol_type (token::_KW_FSQRT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSQRT (const location_type& l)
      {
        return symbol_type (token::_KW_FSQRT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FST (location_type l)
      {
        return symbol_type (token::_KW_FST, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FST (const location_type& l)
      {
        return symbol_type (token::_KW_FST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSTCW (location_type l)
      {
        return symbol_type (token::_KW_FSTCW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSTCW (const location_type& l)
      {
        return symbol_type (token::_KW_FSTCW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSTENV (location_type l)
      {
        return symbol_type (token::_KW_FSTENV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSTENV (const location_type& l)
      {
        return symbol_type (token::_KW_FSTENV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSTP (location_type l)
      {
        return symbol_type (token::_KW_FSTP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSTP (const location_type& l)
      {
        return symbol_type (token::_KW_FSTP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSTSW (location_type l)
      {
        return symbol_type (token::_KW_FSTSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSTSW (const location_type& l)
      {
        return symbol_type (token::_KW_FSTSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSUB (location_type l)
      {
        return symbol_type (token::_KW_FSUB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSUB (const location_type& l)
      {
        return symbol_type (token::_KW_FSUB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSUBP (location_type l)
      {
        return symbol_type (token::_KW_FSUBP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSUBP (const location_type& l)
      {
        return symbol_type (token::_KW_FSUBP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSUBR (location_type l)
      {
        return symbol_type (token::_KW_FSUBR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSUBR (const location_type& l)
      {
        return symbol_type (token::_KW_FSUBR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FSUBRP (location_type l)
      {
        return symbol_type (token::_KW_FSUBRP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FSUBRP (const location_type& l)
      {
        return symbol_type (token::_KW_FSUBRP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FTST (location_type l)
      {
        return symbol_type (token::_KW_FTST, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FTST (const location_type& l)
      {
        return symbol_type (token::_KW_FTST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FUCOM (location_type l)
      {
        return symbol_type (token::_KW_FUCOM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FUCOM (const location_type& l)
      {
        return symbol_type (token::_KW_FUCOM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FUCOMP (location_type l)
      {
        return symbol_type (token::_KW_FUCOMP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FUCOMP (const location_type& l)
      {
        return symbol_type (token::_KW_FUCOMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FUCOMPP (location_type l)
      {
        return symbol_type (token::_KW_FUCOMPP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FUCOMPP (const location_type& l)
      {
        return symbol_type (token::_KW_FUCOMPP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FXAM (location_type l)
      {
        return symbol_type (token::_KW_FXAM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FXAM (const location_type& l)
      {
        return symbol_type (token::_KW_FXAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FXCH (location_type l)
      {
        return symbol_type (token::_KW_FXCH, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FXCH (const location_type& l)
      {
        return symbol_type (token::_KW_FXCH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FXTRACT (location_type l)
      {
        return symbol_type (token::_KW_FXTRACT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FXTRACT (const location_type& l)
      {
        return symbol_type (token::_KW_FXTRACT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FYL2X (location_type l)
      {
        return symbol_type (token::_KW_FYL2X, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FYL2X (const location_type& l)
      {
        return symbol_type (token::_KW_FYL2X, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_FYL2XP1 (location_type l)
      {
        return symbol_type (token::_KW_FYL2XP1, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_FYL2XP1 (const location_type& l)
      {
        return symbol_type (token::_KW_FYL2XP1, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_GLOBAL (location_type l)
      {
        return symbol_type (token::_KW_GLOBAL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_GLOBAL (const location_type& l)
      {
        return symbol_type (token::_KW_GLOBAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_GS (location_type l)
      {
        return symbol_type (token::_KW_GS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_GS (const location_type& l)
      {
        return symbol_type (token::_KW_GS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_HLT (location_type l)
      {
        return symbol_type (token::_KW_HLT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_HLT (const location_type& l)
      {
        return symbol_type (token::_KW_HLT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_IDIV (location_type l)
      {
        return symbol_type (token::_KW_IDIV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_IDIV (const location_type& l)
      {
        return symbol_type (token::_KW_IDIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_IMUL (location_type l)
      {
        return symbol_type (token::_KW_IMUL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_IMUL (const location_type& l)
      {
        return symbol_type (token::_KW_IMUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_IN (location_type l)
      {
        return symbol_type (token::_KW_IN, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_IN (const location_type& l)
      {
        return symbol_type (token::_KW_IN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INC (location_type l)
      {
        return symbol_type (token::_KW_INC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INC (const location_type& l)
      {
        return symbol_type (token::_KW_INC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INCO (location_type l)
      {
        return symbol_type (token::_KW_INCO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INCO (const location_type& l)
      {
        return symbol_type (token::_KW_INCO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INSB (location_type l)
      {
        return symbol_type (token::_KW_INSB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INSB (const location_type& l)
      {
        return symbol_type (token::_KW_INSB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INSD (location_type l)
      {
        return symbol_type (token::_KW_INSD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INSD (const location_type& l)
      {
        return symbol_type (token::_KW_INSD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INSTRSET (location_type l)
      {
        return symbol_type (token::_KW_INSTRSET, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INSTRSET (const location_type& l)
      {
        return symbol_type (token::_KW_INSTRSET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INSW (location_type l)
      {
        return symbol_type (token::_KW_INSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INSW (const location_type& l)
      {
        return symbol_type (token::_KW_INSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INT (location_type l)
      {
        return symbol_type (token::_KW_INT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INT (const location_type& l)
      {
        return symbol_type (token::_KW_INT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INT3 (location_type l)
      {
        return symbol_type (token::_KW_INT3, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INT3 (const location_type& l)
      {
        return symbol_type (token::_KW_INT3, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INTO (location_type l)
      {
        return symbol_type (token::_KW_INTO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INTO (const location_type& l)
      {
        return symbol_type (token::_KW_INTO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INVD (location_type l)
      {
        return symbol_type (token::_KW_INVD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INVD (const location_type& l)
      {
        return symbol_type (token::_KW_INVD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_INVLPG (location_type l)
      {
        return symbol_type (token::_KW_INVLPG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_INVLPG (const location_type& l)
      {
        return symbol_type (token::_KW_INVLPG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_IRET (location_type l)
      {
        return symbol_type (token::_KW_IRET, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_IRET (const location_type& l)
      {
        return symbol_type (token::_KW_IRET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_IRETD (location_type l)
      {
        return symbol_type (token::_KW_IRETD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_IRETD (const location_type& l)
      {
        return symbol_type (token::_KW_IRETD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_IRETW (location_type l)
      {
        return symbol_type (token::_KW_IRETW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_IRETW (const location_type& l)
      {
        return symbol_type (token::_KW_IRETW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JA (location_type l)
      {
        return symbol_type (token::_KW_JA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JA (const location_type& l)
      {
        return symbol_type (token::_KW_JA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JAE (location_type l)
      {
        return symbol_type (token::_KW_JAE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JAE (const location_type& l)
      {
        return symbol_type (token::_KW_JAE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JB (location_type l)
      {
        return symbol_type (token::_KW_JB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JB (const location_type& l)
      {
        return symbol_type (token::_KW_JB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JBE (location_type l)
      {
        return symbol_type (token::_KW_JBE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JBE (const location_type& l)
      {
        return symbol_type (token::_KW_JBE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JC (location_type l)
      {
        return symbol_type (token::_KW_JC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JC (const location_type& l)
      {
        return symbol_type (token::_KW_JC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JCXZ (location_type l)
      {
        return symbol_type (token::_KW_JCXZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JCXZ (const location_type& l)
      {
        return symbol_type (token::_KW_JCXZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JE (location_type l)
      {
        return symbol_type (token::_KW_JE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JE (const location_type& l)
      {
        return symbol_type (token::_KW_JE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JECXZ (location_type l)
      {
        return symbol_type (token::_KW_JECXZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JECXZ (const location_type& l)
      {
        return symbol_type (token::_KW_JECXZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JG (location_type l)
      {
        return symbol_type (token::_KW_JG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JG (const location_type& l)
      {
        return symbol_type (token::_KW_JG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JGE (location_type l)
      {
        return symbol_type (token::_KW_JGE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JGE (const location_type& l)
      {
        return symbol_type (token::_KW_JGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JL (location_type l)
      {
        return symbol_type (token::_KW_JL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JL (const location_type& l)
      {
        return symbol_type (token::_KW_JL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JLE (location_type l)
      {
        return symbol_type (token::_KW_JLE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JLE (const location_type& l)
      {
        return symbol_type (token::_KW_JLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JMP (location_type l)
      {
        return symbol_type (token::_KW_JMP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JMP (const location_type& l)
      {
        return symbol_type (token::_KW_JMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNA (location_type l)
      {
        return symbol_type (token::_KW_JNA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNA (const location_type& l)
      {
        return symbol_type (token::_KW_JNA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNAE (location_type l)
      {
        return symbol_type (token::_KW_JNAE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNAE (const location_type& l)
      {
        return symbol_type (token::_KW_JNAE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNB (location_type l)
      {
        return symbol_type (token::_KW_JNB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNB (const location_type& l)
      {
        return symbol_type (token::_KW_JNB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNBE (location_type l)
      {
        return symbol_type (token::_KW_JNBE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNBE (const location_type& l)
      {
        return symbol_type (token::_KW_JNBE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNC (location_type l)
      {
        return symbol_type (token::_KW_JNC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNC (const location_type& l)
      {
        return symbol_type (token::_KW_JNC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNE (location_type l)
      {
        return symbol_type (token::_KW_JNE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNE (const location_type& l)
      {
        return symbol_type (token::_KW_JNE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNG (location_type l)
      {
        return symbol_type (token::_KW_JNG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNG (const location_type& l)
      {
        return symbol_type (token::_KW_JNG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNGE (location_type l)
      {
        return symbol_type (token::_KW_JNGE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNGE (const location_type& l)
      {
        return symbol_type (token::_KW_JNGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNL (location_type l)
      {
        return symbol_type (token::_KW_JNL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNL (const location_type& l)
      {
        return symbol_type (token::_KW_JNL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNLE (location_type l)
      {
        return symbol_type (token::_KW_JNLE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNLE (const location_type& l)
      {
        return symbol_type (token::_KW_JNLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNO (location_type l)
      {
        return symbol_type (token::_KW_JNO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNO (const location_type& l)
      {
        return symbol_type (token::_KW_JNO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNP (location_type l)
      {
        return symbol_type (token::_KW_JNP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNP (const location_type& l)
      {
        return symbol_type (token::_KW_JNP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNS (location_type l)
      {
        return symbol_type (token::_KW_JNS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNS (const location_type& l)
      {
        return symbol_type (token::_KW_JNS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JNZ (location_type l)
      {
        return symbol_type (token::_KW_JNZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JNZ (const location_type& l)
      {
        return symbol_type (token::_KW_JNZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JO (location_type l)
      {
        return symbol_type (token::_KW_JO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JO (const location_type& l)
      {
        return symbol_type (token::_KW_JO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JP (location_type l)
      {
        return symbol_type (token::_KW_JP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JP (const location_type& l)
      {
        return symbol_type (token::_KW_JP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JPE (location_type l)
      {
        return symbol_type (token::_KW_JPE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JPE (const location_type& l)
      {
        return symbol_type (token::_KW_JPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JPO (location_type l)
      {
        return symbol_type (token::_KW_JPO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JPO (const location_type& l)
      {
        return symbol_type (token::_KW_JPO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JS (location_type l)
      {
        return symbol_type (token::_KW_JS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JS (const location_type& l)
      {
        return symbol_type (token::_KW_JS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_JZ (location_type l)
      {
        return symbol_type (token::_KW_JZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_JZ (const location_type& l)
      {
        return symbol_type (token::_KW_JZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LAHF (location_type l)
      {
        return symbol_type (token::_KW_LAHF, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LAHF (const location_type& l)
      {
        return symbol_type (token::_KW_LAHF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LAR (location_type l)
      {
        return symbol_type (token::_KW_LAR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LAR (const location_type& l)
      {
        return symbol_type (token::_KW_LAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LDS (location_type l)
      {
        return symbol_type (token::_KW_LDS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LDS (const location_type& l)
      {
        return symbol_type (token::_KW_LDS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LEA (location_type l)
      {
        return symbol_type (token::_KW_LEA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LEA (const location_type& l)
      {
        return symbol_type (token::_KW_LEA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LEAVE (location_type l)
      {
        return symbol_type (token::_KW_LEAVE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LEAVE (const location_type& l)
      {
        return symbol_type (token::_KW_LEAVE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LES (location_type l)
      {
        return symbol_type (token::_KW_LES, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LES (const location_type& l)
      {
        return symbol_type (token::_KW_LES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LFS (location_type l)
      {
        return symbol_type (token::_KW_LFS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LFS (const location_type& l)
      {
        return symbol_type (token::_KW_LFS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LGDT (location_type l)
      {
        return symbol_type (token::_KW_LGDT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LGDT (const location_type& l)
      {
        return symbol_type (token::_KW_LGDT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LGS (location_type l)
      {
        return symbol_type (token::_KW_LGS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LGS (const location_type& l)
      {
        return symbol_type (token::_KW_LGS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LIDT (location_type l)
      {
        return symbol_type (token::_KW_LIDT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LIDT (const location_type& l)
      {
        return symbol_type (token::_KW_LIDT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LLDT (location_type l)
      {
        return symbol_type (token::_KW_LLDT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LLDT (const location_type& l)
      {
        return symbol_type (token::_KW_LLDT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LMSW (location_type l)
      {
        return symbol_type (token::_KW_LMSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LMSW (const location_type& l)
      {
        return symbol_type (token::_KW_LMSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LOCK (location_type l)
      {
        return symbol_type (token::_KW_LOCK, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LOCK (const location_type& l)
      {
        return symbol_type (token::_KW_LOCK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LODSB (location_type l)
      {
        return symbol_type (token::_KW_LODSB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LODSB (const location_type& l)
      {
        return symbol_type (token::_KW_LODSB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LODSD (location_type l)
      {
        return symbol_type (token::_KW_LODSD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LODSD (const location_type& l)
      {
        return symbol_type (token::_KW_LODSD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LODSW (location_type l)
      {
        return symbol_type (token::_KW_LODSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LODSW (const location_type& l)
      {
        return symbol_type (token::_KW_LODSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LOOP (location_type l)
      {
        return symbol_type (token::_KW_LOOP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LOOP (const location_type& l)
      {
        return symbol_type (token::_KW_LOOP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LOOPE (location_type l)
      {
        return symbol_type (token::_KW_LOOPE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LOOPE (const location_type& l)
      {
        return symbol_type (token::_KW_LOOPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LOOPNE (location_type l)
      {
        return symbol_type (token::_KW_LOOPNE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LOOPNE (const location_type& l)
      {
        return symbol_type (token::_KW_LOOPNE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LOOPNZ (location_type l)
      {
        return symbol_type (token::_KW_LOOPNZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LOOPNZ (const location_type& l)
      {
        return symbol_type (token::_KW_LOOPNZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LOOPZ (location_type l)
      {
        return symbol_type (token::_KW_LOOPZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LOOPZ (const location_type& l)
      {
        return symbol_type (token::_KW_LOOPZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LSL (location_type l)
      {
        return symbol_type (token::_KW_LSL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LSL (const location_type& l)
      {
        return symbol_type (token::_KW_LSL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LSS (location_type l)
      {
        return symbol_type (token::_KW_LSS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LSS (const location_type& l)
      {
        return symbol_type (token::_KW_LSS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_LTR (location_type l)
      {
        return symbol_type (token::_KW_LTR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_LTR (const location_type& l)
      {
        return symbol_type (token::_KW_LTR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_MOV (location_type l)
      {
        return symbol_type (token::_KW_MOV, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_MOV (const location_type& l)
      {
        return symbol_type (token::_KW_MOV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_MOVSB (location_type l)
      {
        return symbol_type (token::_KW_MOVSB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_MOVSB (const location_type& l)
      {
        return symbol_type (token::_KW_MOVSB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_MOVSD (location_type l)
      {
        return symbol_type (token::_KW_MOVSD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_MOVSD (const location_type& l)
      {
        return symbol_type (token::_KW_MOVSD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_MOVSW (location_type l)
      {
        return symbol_type (token::_KW_MOVSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_MOVSW (const location_type& l)
      {
        return symbol_type (token::_KW_MOVSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_MOVSX (location_type l)
      {
        return symbol_type (token::_KW_MOVSX, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_MOVSX (const location_type& l)
      {
        return symbol_type (token::_KW_MOVSX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_MOVZX (location_type l)
      {
        return symbol_type (token::_KW_MOVZX, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_MOVZX (const location_type& l)
      {
        return symbol_type (token::_KW_MOVZX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_MUL (location_type l)
      {
        return symbol_type (token::_KW_MUL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_MUL (const location_type& l)
      {
        return symbol_type (token::_KW_MUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_NEAR (location_type l)
      {
        return symbol_type (token::_KW_NEAR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_NEAR (const location_type& l)
      {
        return symbol_type (token::_KW_NEAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_NEG (location_type l)
      {
        return symbol_type (token::_KW_NEG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_NEG (const location_type& l)
      {
        return symbol_type (token::_KW_NEG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_NOP (location_type l)
      {
        return symbol_type (token::_KW_NOP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_NOP (const location_type& l)
      {
        return symbol_type (token::_KW_NOP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_NOT (location_type l)
      {
        return symbol_type (token::_KW_NOT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_NOT (const location_type& l)
      {
        return symbol_type (token::_KW_NOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_OPTIMIZE (location_type l)
      {
        return symbol_type (token::_KW_OPTIMIZE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_OPTIMIZE (const location_type& l)
      {
        return symbol_type (token::_KW_OPTIMIZE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_OR (location_type l)
      {
        return symbol_type (token::_KW_OR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_OR (const location_type& l)
      {
        return symbol_type (token::_KW_OR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ORG (location_type l)
      {
        return symbol_type (token::_KW_ORG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ORG (const location_type& l)
      {
        return symbol_type (token::_KW_ORG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_OUT (location_type l)
      {
        return symbol_type (token::_KW_OUT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_OUT (const location_type& l)
      {
        return symbol_type (token::_KW_OUT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_OUTSB (location_type l)
      {
        return symbol_type (token::_KW_OUTSB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_OUTSB (const location_type& l)
      {
        return symbol_type (token::_KW_OUTSB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_OUTSD (location_type l)
      {
        return symbol_type (token::_KW_OUTSD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_OUTSD (const location_type& l)
      {
        return symbol_type (token::_KW_OUTSD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_OUTSW (location_type l)
      {
        return symbol_type (token::_KW_OUTSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_OUTSW (const location_type& l)
      {
        return symbol_type (token::_KW_OUTSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PADDING (location_type l)
      {
        return symbol_type (token::_KW_PADDING, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PADDING (const location_type& l)
      {
        return symbol_type (token::_KW_PADDING, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PADSET (location_type l)
      {
        return symbol_type (token::_KW_PADSET, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PADSET (const location_type& l)
      {
        return symbol_type (token::_KW_PADSET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_POP (location_type l)
      {
        return symbol_type (token::_KW_POP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_POP (const location_type& l)
      {
        return symbol_type (token::_KW_POP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_POPA (location_type l)
      {
        return symbol_type (token::_KW_POPA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_POPA (const location_type& l)
      {
        return symbol_type (token::_KW_POPA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_POPAD (location_type l)
      {
        return symbol_type (token::_KW_POPAD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_POPAD (const location_type& l)
      {
        return symbol_type (token::_KW_POPAD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_POPAW (location_type l)
      {
        return symbol_type (token::_KW_POPAW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_POPAW (const location_type& l)
      {
        return symbol_type (token::_KW_POPAW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_POPF (location_type l)
      {
        return symbol_type (token::_KW_POPF, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_POPF (const location_type& l)
      {
        return symbol_type (token::_KW_POPF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_POPFD (location_type l)
      {
        return symbol_type (token::_KW_POPFD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_POPFD (const location_type& l)
      {
        return symbol_type (token::_KW_POPFD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_POPFW (location_type l)
      {
        return symbol_type (token::_KW_POPFW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_POPFW (const location_type& l)
      {
        return symbol_type (token::_KW_POPFW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSH (location_type l)
      {
        return symbol_type (token::_KW_PUSH, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSH (const location_type& l)
      {
        return symbol_type (token::_KW_PUSH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSHA (location_type l)
      {
        return symbol_type (token::_KW_PUSHA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSHA (const location_type& l)
      {
        return symbol_type (token::_KW_PUSHA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSHAD (location_type l)
      {
        return symbol_type (token::_KW_PUSHAD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSHAD (const location_type& l)
      {
        return symbol_type (token::_KW_PUSHAD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSHAW (location_type l)
      {
        return symbol_type (token::_KW_PUSHAW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSHAW (const location_type& l)
      {
        return symbol_type (token::_KW_PUSHAW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSHD (location_type l)
      {
        return symbol_type (token::_KW_PUSHD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSHD (const location_type& l)
      {
        return symbol_type (token::_KW_PUSHD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSHF (location_type l)
      {
        return symbol_type (token::_KW_PUSHF, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSHF (const location_type& l)
      {
        return symbol_type (token::_KW_PUSHF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSHFD (location_type l)
      {
        return symbol_type (token::_KW_PUSHFD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSHFD (const location_type& l)
      {
        return symbol_type (token::_KW_PUSHFD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_PUSHFW (location_type l)
      {
        return symbol_type (token::_KW_PUSHFW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_PUSHFW (const location_type& l)
      {
        return symbol_type (token::_KW_PUSHFW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RCL (location_type l)
      {
        return symbol_type (token::_KW_RCL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RCL (const location_type& l)
      {
        return symbol_type (token::_KW_RCL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RCR (location_type l)
      {
        return symbol_type (token::_KW_RCR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RCR (const location_type& l)
      {
        return symbol_type (token::_KW_RCR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RDMSR (location_type l)
      {
        return symbol_type (token::_KW_RDMSR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RDMSR (const location_type& l)
      {
        return symbol_type (token::_KW_RDMSR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RDPMC (location_type l)
      {
        return symbol_type (token::_KW_RDPMC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RDPMC (const location_type& l)
      {
        return symbol_type (token::_KW_RDPMC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_REP (location_type l)
      {
        return symbol_type (token::_KW_REP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_REP (const location_type& l)
      {
        return symbol_type (token::_KW_REP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_REPE (location_type l)
      {
        return symbol_type (token::_KW_REPE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_REPE (const location_type& l)
      {
        return symbol_type (token::_KW_REPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_REPNE (location_type l)
      {
        return symbol_type (token::_KW_REPNE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_REPNE (const location_type& l)
      {
        return symbol_type (token::_KW_REPNE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_REPNZ (location_type l)
      {
        return symbol_type (token::_KW_REPNZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_REPNZ (const location_type& l)
      {
        return symbol_type (token::_KW_REPNZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_REPZ (location_type l)
      {
        return symbol_type (token::_KW_REPZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_REPZ (const location_type& l)
      {
        return symbol_type (token::_KW_REPZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RESB (location_type l)
      {
        return symbol_type (token::_KW_RESB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RESB (const location_type& l)
      {
        return symbol_type (token::_KW_RESB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RESD (location_type l)
      {
        return symbol_type (token::_KW_RESD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RESD (const location_type& l)
      {
        return symbol_type (token::_KW_RESD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RESQ (location_type l)
      {
        return symbol_type (token::_KW_RESQ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RESQ (const location_type& l)
      {
        return symbol_type (token::_KW_RESQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_REST (location_type l)
      {
        return symbol_type (token::_KW_REST, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_REST (const location_type& l)
      {
        return symbol_type (token::_KW_REST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RESW (location_type l)
      {
        return symbol_type (token::_KW_RESW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RESW (const location_type& l)
      {
        return symbol_type (token::_KW_RESW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RET (location_type l)
      {
        return symbol_type (token::_KW_RET, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RET (const location_type& l)
      {
        return symbol_type (token::_KW_RET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RETF (location_type l)
      {
        return symbol_type (token::_KW_RETF, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RETF (const location_type& l)
      {
        return symbol_type (token::_KW_RETF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RETN (location_type l)
      {
        return symbol_type (token::_KW_RETN, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RETN (const location_type& l)
      {
        return symbol_type (token::_KW_RETN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ROL (location_type l)
      {
        return symbol_type (token::_KW_ROL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ROL (const location_type& l)
      {
        return symbol_type (token::_KW_ROL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_ROR (location_type l)
      {
        return symbol_type (token::_KW_ROR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_ROR (const location_type& l)
      {
        return symbol_type (token::_KW_ROR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_RSM (location_type l)
      {
        return symbol_type (token::_KW_RSM, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_RSM (const location_type& l)
      {
        return symbol_type (token::_KW_RSM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SAHF (location_type l)
      {
        return symbol_type (token::_KW_SAHF, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SAHF (const location_type& l)
      {
        return symbol_type (token::_KW_SAHF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SAL (location_type l)
      {
        return symbol_type (token::_KW_SAL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SAL (const location_type& l)
      {
        return symbol_type (token::_KW_SAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SAR (location_type l)
      {
        return symbol_type (token::_KW_SAR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SAR (const location_type& l)
      {
        return symbol_type (token::_KW_SAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SBB (location_type l)
      {
        return symbol_type (token::_KW_SBB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SBB (const location_type& l)
      {
        return symbol_type (token::_KW_SBB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SCASB (location_type l)
      {
        return symbol_type (token::_KW_SCASB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SCASB (const location_type& l)
      {
        return symbol_type (token::_KW_SCASB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SCASD (location_type l)
      {
        return symbol_type (token::_KW_SCASD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SCASD (const location_type& l)
      {
        return symbol_type (token::_KW_SCASD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SCASW (location_type l)
      {
        return symbol_type (token::_KW_SCASW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SCASW (const location_type& l)
      {
        return symbol_type (token::_KW_SCASW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SECTION (location_type l)
      {
        return symbol_type (token::_KW_SECTION, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SECTION (const location_type& l)
      {
        return symbol_type (token::_KW_SECTION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETA (location_type l)
      {
        return symbol_type (token::_KW_SETA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETA (const location_type& l)
      {
        return symbol_type (token::_KW_SETA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETAE (location_type l)
      {
        return symbol_type (token::_KW_SETAE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETAE (const location_type& l)
      {
        return symbol_type (token::_KW_SETAE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETB (location_type l)
      {
        return symbol_type (token::_KW_SETB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETB (const location_type& l)
      {
        return symbol_type (token::_KW_SETB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETBE (location_type l)
      {
        return symbol_type (token::_KW_SETBE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETBE (const location_type& l)
      {
        return symbol_type (token::_KW_SETBE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETC (location_type l)
      {
        return symbol_type (token::_KW_SETC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETC (const location_type& l)
      {
        return symbol_type (token::_KW_SETC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETE (location_type l)
      {
        return symbol_type (token::_KW_SETE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETE (const location_type& l)
      {
        return symbol_type (token::_KW_SETE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETG (location_type l)
      {
        return symbol_type (token::_KW_SETG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETG (const location_type& l)
      {
        return symbol_type (token::_KW_SETG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETGE (location_type l)
      {
        return symbol_type (token::_KW_SETGE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETGE (const location_type& l)
      {
        return symbol_type (token::_KW_SETGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETL (location_type l)
      {
        return symbol_type (token::_KW_SETL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETL (const location_type& l)
      {
        return symbol_type (token::_KW_SETL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETLE (location_type l)
      {
        return symbol_type (token::_KW_SETLE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETLE (const location_type& l)
      {
        return symbol_type (token::_KW_SETLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNA (location_type l)
      {
        return symbol_type (token::_KW_SETNA, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNA (const location_type& l)
      {
        return symbol_type (token::_KW_SETNA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNAE (location_type l)
      {
        return symbol_type (token::_KW_SETNAE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNAE (const location_type& l)
      {
        return symbol_type (token::_KW_SETNAE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNB (location_type l)
      {
        return symbol_type (token::_KW_SETNB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNB (const location_type& l)
      {
        return symbol_type (token::_KW_SETNB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNBE (location_type l)
      {
        return symbol_type (token::_KW_SETNBE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNBE (const location_type& l)
      {
        return symbol_type (token::_KW_SETNBE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNC (location_type l)
      {
        return symbol_type (token::_KW_SETNC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNC (const location_type& l)
      {
        return symbol_type (token::_KW_SETNC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNE (location_type l)
      {
        return symbol_type (token::_KW_SETNE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNE (const location_type& l)
      {
        return symbol_type (token::_KW_SETNE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNG (location_type l)
      {
        return symbol_type (token::_KW_SETNG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNG (const location_type& l)
      {
        return symbol_type (token::_KW_SETNG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNGE (location_type l)
      {
        return symbol_type (token::_KW_SETNGE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNGE (const location_type& l)
      {
        return symbol_type (token::_KW_SETNGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNL (location_type l)
      {
        return symbol_type (token::_KW_SETNL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNL (const location_type& l)
      {
        return symbol_type (token::_KW_SETNL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNLE (location_type l)
      {
        return symbol_type (token::_KW_SETNLE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNLE (const location_type& l)
      {
        return symbol_type (token::_KW_SETNLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNO (location_type l)
      {
        return symbol_type (token::_KW_SETNO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNO (const location_type& l)
      {
        return symbol_type (token::_KW_SETNO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNP (location_type l)
      {
        return symbol_type (token::_KW_SETNP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNP (const location_type& l)
      {
        return symbol_type (token::_KW_SETNP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNS (location_type l)
      {
        return symbol_type (token::_KW_SETNS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNS (const location_type& l)
      {
        return symbol_type (token::_KW_SETNS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETNZ (location_type l)
      {
        return symbol_type (token::_KW_SETNZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETNZ (const location_type& l)
      {
        return symbol_type (token::_KW_SETNZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETO (location_type l)
      {
        return symbol_type (token::_KW_SETO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETO (const location_type& l)
      {
        return symbol_type (token::_KW_SETO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETP (location_type l)
      {
        return symbol_type (token::_KW_SETP, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETP (const location_type& l)
      {
        return symbol_type (token::_KW_SETP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETPE (location_type l)
      {
        return symbol_type (token::_KW_SETPE, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETPE (const location_type& l)
      {
        return symbol_type (token::_KW_SETPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETPO (location_type l)
      {
        return symbol_type (token::_KW_SETPO, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETPO (const location_type& l)
      {
        return symbol_type (token::_KW_SETPO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETS (location_type l)
      {
        return symbol_type (token::_KW_SETS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETS (const location_type& l)
      {
        return symbol_type (token::_KW_SETS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SETZ (location_type l)
      {
        return symbol_type (token::_KW_SETZ, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SETZ (const location_type& l)
      {
        return symbol_type (token::_KW_SETZ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SGDT (location_type l)
      {
        return symbol_type (token::_KW_SGDT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SGDT (const location_type& l)
      {
        return symbol_type (token::_KW_SGDT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SHL (location_type l)
      {
        return symbol_type (token::_KW_SHL, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SHL (const location_type& l)
      {
        return symbol_type (token::_KW_SHL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SHLD (location_type l)
      {
        return symbol_type (token::_KW_SHLD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SHLD (const location_type& l)
      {
        return symbol_type (token::_KW_SHLD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SHORT (location_type l)
      {
        return symbol_type (token::_KW_SHORT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SHORT (const location_type& l)
      {
        return symbol_type (token::_KW_SHORT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SHR (location_type l)
      {
        return symbol_type (token::_KW_SHR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SHR (const location_type& l)
      {
        return symbol_type (token::_KW_SHR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SHRD (location_type l)
      {
        return symbol_type (token::_KW_SHRD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SHRD (const location_type& l)
      {
        return symbol_type (token::_KW_SHRD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SIDT (location_type l)
      {
        return symbol_type (token::_KW_SIDT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SIDT (const location_type& l)
      {
        return symbol_type (token::_KW_SIDT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SLDT (location_type l)
      {
        return symbol_type (token::_KW_SLDT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SLDT (const location_type& l)
      {
        return symbol_type (token::_KW_SLDT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SMSW (location_type l)
      {
        return symbol_type (token::_KW_SMSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SMSW (const location_type& l)
      {
        return symbol_type (token::_KW_SMSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SS (location_type l)
      {
        return symbol_type (token::_KW_SS, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SS (const location_type& l)
      {
        return symbol_type (token::_KW_SS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_STC (location_type l)
      {
        return symbol_type (token::_KW_STC, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_STC (const location_type& l)
      {
        return symbol_type (token::_KW_STC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_STD (location_type l)
      {
        return symbol_type (token::_KW_STD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_STD (const location_type& l)
      {
        return symbol_type (token::_KW_STD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_STI (location_type l)
      {
        return symbol_type (token::_KW_STI, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_STI (const location_type& l)
      {
        return symbol_type (token::_KW_STI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_STOSB (location_type l)
      {
        return symbol_type (token::_KW_STOSB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_STOSB (const location_type& l)
      {
        return symbol_type (token::_KW_STOSB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_STOSD (location_type l)
      {
        return symbol_type (token::_KW_STOSD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_STOSD (const location_type& l)
      {
        return symbol_type (token::_KW_STOSD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_STOSW (location_type l)
      {
        return symbol_type (token::_KW_STOSW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_STOSW (const location_type& l)
      {
        return symbol_type (token::_KW_STOSW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_STR (location_type l)
      {
        return symbol_type (token::_KW_STR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_STR (const location_type& l)
      {
        return symbol_type (token::_KW_STR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_SUB (location_type l)
      {
        return symbol_type (token::_KW_SUB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_SUB (const location_type& l)
      {
        return symbol_type (token::_KW_SUB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_TEST (location_type l)
      {
        return symbol_type (token::_KW_TEST, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_TEST (const location_type& l)
      {
        return symbol_type (token::_KW_TEST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_TIMES (location_type l)
      {
        return symbol_type (token::_KW_TIMES, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_TIMES (const location_type& l)
      {
        return symbol_type (token::_KW_TIMES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_UD2 (location_type l)
      {
        return symbol_type (token::_KW_UD2, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_UD2 (const location_type& l)
      {
        return symbol_type (token::_KW_UD2, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_VERR (location_type l)
      {
        return symbol_type (token::_KW_VERR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_VERR (const location_type& l)
      {
        return symbol_type (token::_KW_VERR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_VERW (location_type l)
      {
        return symbol_type (token::_KW_VERW, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_VERW (const location_type& l)
      {
        return symbol_type (token::_KW_VERW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_WAIT (location_type l)
      {
        return symbol_type (token::_KW_WAIT, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_WAIT (const location_type& l)
      {
        return symbol_type (token::_KW_WAIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_WBINVD (location_type l)
      {
        return symbol_type (token::_KW_WBINVD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_WBINVD (const location_type& l)
      {
        return symbol_type (token::_KW_WBINVD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_WORD (location_type l)
      {
        return symbol_type (token::_KW_WORD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_WORD (const location_type& l)
      {
        return symbol_type (token::_KW_WORD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_WRMSR (location_type l)
      {
        return symbol_type (token::_KW_WRMSR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_WRMSR (const location_type& l)
      {
        return symbol_type (token::_KW_WRMSR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_XADD (location_type l)
      {
        return symbol_type (token::_KW_XADD, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_XADD (const location_type& l)
      {
        return symbol_type (token::_KW_XADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_XCHG (location_type l)
      {
        return symbol_type (token::_KW_XCHG, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_XCHG (const location_type& l)
      {
        return symbol_type (token::_KW_XCHG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_XLATB (location_type l)
      {
        return symbol_type (token::_KW_XLATB, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_XLATB (const location_type& l)
      {
        return symbol_type (token::_KW_XLATB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__KW_XOR (location_type l)
      {
        return symbol_type (token::_KW_XOR, std::move (l));
      }
#else
      static
      symbol_type
      make__KW_XOR (const location_type& l)
      {
        return symbol_type (token::_KW_XOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__LBRACK (location_type l)
      {
        return symbol_type (token::_LBRACK, std::move (l));
      }
#else
      static
      symbol_type
      make__LBRACK (const location_type& l)
      {
        return symbol_type (token::_LBRACK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__RBRACK (location_type l)
      {
        return symbol_type (token::_RBRACK, std::move (l));
      }
#else
      static
      symbol_type
      make__RBRACK (const location_type& l)
      {
        return symbol_type (token::_RBRACK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_Hex (std::string v, location_type l)
      {
        return symbol_type (token::T_Hex, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_Hex (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_Hex, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_Id (std::string v, location_type l)
      {
        return symbol_type (token::T_Id, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_Id (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_Id, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_NaskChar (std::string v, location_type l)
      {
        return symbol_type (token::T_NaskChar, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_NaskChar (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_NaskChar, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__STRING_ (std::string v, location_type l)
      {
        return symbol_type (token::_STRING_, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make__STRING_ (const std::string& v, const location_type& l)
      {
        return symbol_type (token::_STRING_, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make__INTEGER_ (int v, location_type l)
      {
        return symbol_type (token::_INTEGER_, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make__INTEGER_ (const int& v, const location_type& l)
      {
        return symbol_type (token::_INTEGER_, v, l);
      }
#endif


  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    NaskParser (const NaskParser&);
    /// Non copyable.
    NaskParser& operator= (const NaskParser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

#if YYDEBUG || 0
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if YYDEBUG || 0


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const short yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const short yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const short yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 1161,     ///< Last index in yytable_.
      yynnts_ = 24,  ///< Number of nonterminal symbols.
      yyfinal_ = 381 ///< Termination state number.
    };


    // User arguments.
    NaskScanner  &scanner;
    NaskDriver  &driver;

  };


} // nask_
#line 7738 "bison.hh"




#endif // !YY_NASK_BISON_HH_INCLUDED
