// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

// "%code top" blocks.
#line 12 "nask.yy"

#include <memory>

#line 43 "parser.cc"

// Take the name prefix into account.
#define yylex   nask_lex



#include "bison.hh"


// Unqualified %code blocks.
#line 34 "nask.yy"

/* Begin C++ preamble code */
#include <algorithm> /* for std::reverse */
#include <iostream>
#include <cstdlib>
#include <fstream>

/* include for all driver functions */
#include "driver.hh"

#undef yylex
#define yylex scanner.lex

#line 68 "parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace nask_ {
#line 160 "parser.cc"

  /// Build a parser object.
  NaskParser::NaskParser (NaskScanner  &scanner_yyarg, NaskDriver  &driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  NaskParser::~NaskParser ()
  {}

  NaskParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  NaskParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.copy< std::shared_ptr<ConfigType> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DataType: // DataType
        value.copy< std::shared_ptr<DataType> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
        value.copy< std::shared_ptr<Exp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Factor: // Factor
        value.copy< std::shared_ptr<Factor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.copy< std::shared_ptr<IndexExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.copy< std::shared_ptr<JumpDir> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.copy< std::shared_ptr<ListFactor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.copy< std::shared_ptr<ListMnemonicArgs> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.copy< std::shared_ptr<ListStatement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.copy< std::shared_ptr<MemoryAddr> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.copy< std::shared_ptr<MnemonicArgs> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.copy< std::shared_ptr<Opcode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Program: // Program
        value.copy< std::shared_ptr<Program> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.copy< std::shared_ptr<Statement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_Label: // T_Label
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  NaskParser::symbol_kind_type
  NaskParser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  NaskParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  NaskParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.move< std::shared_ptr<ConfigType> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DataType: // DataType
        value.move< std::shared_ptr<DataType> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
        value.move< std::shared_ptr<Exp> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Factor: // Factor
        value.move< std::shared_ptr<Factor> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.move< std::shared_ptr<IndexExp> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.move< std::shared_ptr<JumpDir> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.move< std::shared_ptr<ListFactor> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.move< std::shared_ptr<ListMnemonicArgs> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.move< std::shared_ptr<ListStatement> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.move< std::shared_ptr<MemoryAddr> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.move< std::shared_ptr<MnemonicArgs> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.move< std::shared_ptr<Opcode> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Program: // Program
        value.move< std::shared_ptr<Program> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< std::shared_ptr<Statement> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_Label: // T_Label
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  NaskParser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  NaskParser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  NaskParser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  NaskParser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  NaskParser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  NaskParser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  NaskParser::symbol_kind_type
  NaskParser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  NaskParser::symbol_kind_type
  NaskParser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  NaskParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  NaskParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  NaskParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  NaskParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  NaskParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  NaskParser::symbol_kind_type
  NaskParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  NaskParser::stack_symbol_type::stack_symbol_type ()
  {}

  NaskParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.YY_MOVE_OR_COPY< std::shared_ptr<ConfigType> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DataType: // DataType
        value.YY_MOVE_OR_COPY< std::shared_ptr<DataType> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
        value.YY_MOVE_OR_COPY< std::shared_ptr<Exp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Factor: // Factor
        value.YY_MOVE_OR_COPY< std::shared_ptr<Factor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.YY_MOVE_OR_COPY< std::shared_ptr<IndexExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.YY_MOVE_OR_COPY< std::shared_ptr<JumpDir> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.YY_MOVE_OR_COPY< std::shared_ptr<ListFactor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.YY_MOVE_OR_COPY< std::shared_ptr<ListMnemonicArgs> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.YY_MOVE_OR_COPY< std::shared_ptr<ListStatement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.YY_MOVE_OR_COPY< std::shared_ptr<MemoryAddr> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.YY_MOVE_OR_COPY< std::shared_ptr<MnemonicArgs> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.YY_MOVE_OR_COPY< std::shared_ptr<Opcode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Program: // Program
        value.YY_MOVE_OR_COPY< std::shared_ptr<Program> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.YY_MOVE_OR_COPY< std::shared_ptr<Statement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_Label: // T_Label
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  NaskParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.move< std::shared_ptr<ConfigType> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DataType: // DataType
        value.move< std::shared_ptr<DataType> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
        value.move< std::shared_ptr<Exp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Factor: // Factor
        value.move< std::shared_ptr<Factor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.move< std::shared_ptr<IndexExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.move< std::shared_ptr<JumpDir> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.move< std::shared_ptr<ListFactor> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.move< std::shared_ptr<ListMnemonicArgs> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.move< std::shared_ptr<ListStatement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.move< std::shared_ptr<MemoryAddr> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.move< std::shared_ptr<MnemonicArgs> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.move< std::shared_ptr<Opcode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Program: // Program
        value.move< std::shared_ptr<Program> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< std::shared_ptr<Statement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_Label: // T_Label
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  NaskParser::stack_symbol_type&
  NaskParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.copy< std::shared_ptr<ConfigType> > (that.value);
        break;

      case symbol_kind::S_DataType: // DataType
        value.copy< std::shared_ptr<DataType> > (that.value);
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
        value.copy< std::shared_ptr<Exp> > (that.value);
        break;

      case symbol_kind::S_Factor: // Factor
        value.copy< std::shared_ptr<Factor> > (that.value);
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.copy< std::shared_ptr<IndexExp> > (that.value);
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.copy< std::shared_ptr<JumpDir> > (that.value);
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.copy< std::shared_ptr<ListFactor> > (that.value);
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.copy< std::shared_ptr<ListMnemonicArgs> > (that.value);
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.copy< std::shared_ptr<ListStatement> > (that.value);
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.copy< std::shared_ptr<MemoryAddr> > (that.value);
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.copy< std::shared_ptr<MnemonicArgs> > (that.value);
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.copy< std::shared_ptr<Opcode> > (that.value);
        break;

      case symbol_kind::S_Program: // Program
        value.copy< std::shared_ptr<Program> > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.copy< std::shared_ptr<Statement> > (that.value);
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_Label: // T_Label
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  NaskParser::stack_symbol_type&
  NaskParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        value.move< int > (that.value);
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        value.move< std::shared_ptr<ConfigType> > (that.value);
        break;

      case symbol_kind::S_DataType: // DataType
        value.move< std::shared_ptr<DataType> > (that.value);
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
        value.move< std::shared_ptr<Exp> > (that.value);
        break;

      case symbol_kind::S_Factor: // Factor
        value.move< std::shared_ptr<Factor> > (that.value);
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        value.move< std::shared_ptr<IndexExp> > (that.value);
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        value.move< std::shared_ptr<JumpDir> > (that.value);
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        value.move< std::shared_ptr<ListFactor> > (that.value);
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        value.move< std::shared_ptr<ListMnemonicArgs> > (that.value);
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        value.move< std::shared_ptr<ListStatement> > (that.value);
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        value.move< std::shared_ptr<MemoryAddr> > (that.value);
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        value.move< std::shared_ptr<MnemonicArgs> > (that.value);
        break;

      case symbol_kind::S_Opcode: // Opcode
        value.move< std::shared_ptr<Opcode> > (that.value);
        break;

      case symbol_kind::S_Program: // Program
        value.move< std::shared_ptr<Program> > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< std::shared_ptr<Statement> > (that.value);
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_Label: // T_Label
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  NaskParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  NaskParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  NaskParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  NaskParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  NaskParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  NaskParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  NaskParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  NaskParser::debug_level_type
  NaskParser::debug_level () const
  {
    return yydebug_;
  }

  void
  NaskParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  NaskParser::state_type
  NaskParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  NaskParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  NaskParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  NaskParser::operator() ()
  {
    return parse ();
  }

  int
  NaskParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S__INTEGER_: // _INTEGER_
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_ConfigType: // ConfigType
        yylhs.value.emplace< std::shared_ptr<ConfigType> > ();
        break;

      case symbol_kind::S_DataType: // DataType
        yylhs.value.emplace< std::shared_ptr<DataType> > ();
        break;

      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_Exp1: // Exp1
      case symbol_kind::S_Exp2: // Exp2
        yylhs.value.emplace< std::shared_ptr<Exp> > ();
        break;

      case symbol_kind::S_Factor: // Factor
        yylhs.value.emplace< std::shared_ptr<Factor> > ();
        break;

      case symbol_kind::S_IndexExp: // IndexExp
        yylhs.value.emplace< std::shared_ptr<IndexExp> > ();
        break;

      case symbol_kind::S_JumpDir: // JumpDir
        yylhs.value.emplace< std::shared_ptr<JumpDir> > ();
        break;

      case symbol_kind::S_ListFactor: // ListFactor
        yylhs.value.emplace< std::shared_ptr<ListFactor> > ();
        break;

      case symbol_kind::S_ListMnemonicArgs: // ListMnemonicArgs
        yylhs.value.emplace< std::shared_ptr<ListMnemonicArgs> > ();
        break;

      case symbol_kind::S_ListStatement: // ListStatement
        yylhs.value.emplace< std::shared_ptr<ListStatement> > ();
        break;

      case symbol_kind::S_MemoryAddr: // MemoryAddr
        yylhs.value.emplace< std::shared_ptr<MemoryAddr> > ();
        break;

      case symbol_kind::S_MnemonicArgs: // MnemonicArgs
        yylhs.value.emplace< std::shared_ptr<MnemonicArgs> > ();
        break;

      case symbol_kind::S_Opcode: // Opcode
        yylhs.value.emplace< std::shared_ptr<Opcode> > ();
        break;

      case symbol_kind::S_Program: // Program
        yylhs.value.emplace< std::shared_ptr<Program> > ();
        break;

      case symbol_kind::S_Statement: // Statement
        yylhs.value.emplace< std::shared_ptr<Statement> > ();
        break;

      case symbol_kind::S_T_Hex: // T_Hex
      case symbol_kind::S_T_Id: // T_Id
      case symbol_kind::S_T_Label: // T_Label
      case symbol_kind::S_T_NaskChar: // T_NaskChar
      case symbol_kind::S__STRING_: // _STRING_
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // Program: ListStatement
#line 427 "nask.yy"
                        { yystack_[0].value.as < std::shared_ptr<ListStatement> > ()->reverse();yylhs.value.as < std::shared_ptr<Program> > () = std::make_shared<Prog>(yystack_[0].value.as < std::shared_ptr<ListStatement> > ()); yylhs.value.as < std::shared_ptr<Program> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Program> > ()->char_number = yylhs.location.begin.column; driver.program_ = yylhs.value.as < std::shared_ptr<Program> > (); }
#line 1140 "parser.cc"
    break;

  case 3: // ListStatement: Statement
#line 429 "nask.yy"
                          { yylhs.value.as < std::shared_ptr<ListStatement> > () = std::make_shared<ListStatement>(); yylhs.value.as < std::shared_ptr<ListStatement> > ()->cons(yystack_[0].value.as < std::shared_ptr<Statement> > ()); driver.liststatement_ = yylhs.value.as < std::shared_ptr<ListStatement> > (); }
#line 1146 "parser.cc"
    break;

  case 4: // ListStatement: Statement ListStatement
#line 430 "nask.yy"
                            { yystack_[0].value.as < std::shared_ptr<ListStatement> > ()->cons(yystack_[1].value.as < std::shared_ptr<Statement> > ()); yylhs.value.as < std::shared_ptr<ListStatement> > () = yystack_[0].value.as < std::shared_ptr<ListStatement> > (); driver.liststatement_ = yylhs.value.as < std::shared_ptr<ListStatement> > (); }
#line 1152 "parser.cc"
    break;

  case 5: // Statement: T_Label
#line 432 "nask.yy"
                    { yylhs.value.as < std::shared_ptr<Statement> > () = std::make_shared<LabelStmt>(yystack_[0].value.as < std::string > ()); yylhs.value.as < std::shared_ptr<Statement> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Statement> > ()->char_number = yylhs.location.begin.column; driver.statement_ = yylhs.value.as < std::shared_ptr<Statement> > (); }
#line 1158 "parser.cc"
    break;

  case 6: // Statement: T_Id _KW_EQU Exp
#line 433 "nask.yy"
                     { yylhs.value.as < std::shared_ptr<Statement> > () = std::make_shared<DeclareStmt>(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<Statement> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Statement> > ()->char_number = yylhs.location.begin.column; driver.statement_ = yylhs.value.as < std::shared_ptr<Statement> > (); }
#line 1164 "parser.cc"
    break;

  case 7: // Statement: _KW_GLOBAL ListFactor
#line 434 "nask.yy"
                          { yystack_[0].value.as < std::shared_ptr<ListFactor> > ()->reverse();yylhs.value.as < std::shared_ptr<Statement> > () = std::make_shared<ExportSymStmt>(yystack_[0].value.as < std::shared_ptr<ListFactor> > ()); yylhs.value.as < std::shared_ptr<Statement> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Statement> > ()->char_number = yylhs.location.begin.column; driver.statement_ = yylhs.value.as < std::shared_ptr<Statement> > (); }
#line 1170 "parser.cc"
    break;

  case 8: // Statement: _KW_EXTERN ListFactor
#line 435 "nask.yy"
                          { yystack_[0].value.as < std::shared_ptr<ListFactor> > ()->reverse();yylhs.value.as < std::shared_ptr<Statement> > () = std::make_shared<ExternSymStmt>(yystack_[0].value.as < std::shared_ptr<ListFactor> > ()); yylhs.value.as < std::shared_ptr<Statement> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Statement> > ()->char_number = yylhs.location.begin.column; driver.statement_ = yylhs.value.as < std::shared_ptr<Statement> > (); }
#line 1176 "parser.cc"
    break;

  case 9: // Statement: _LBRACK ConfigType Factor _RBRACK
#line 436 "nask.yy"
                                      { yylhs.value.as < std::shared_ptr<Statement> > () = std::make_shared<ConfigStmt>(yystack_[2].value.as < std::shared_ptr<ConfigType> > (), yystack_[1].value.as < std::shared_ptr<Factor> > ()); yylhs.value.as < std::shared_ptr<Statement> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Statement> > ()->char_number = yylhs.location.begin.column; driver.statement_ = yylhs.value.as < std::shared_ptr<Statement> > (); }
#line 1182 "parser.cc"
    break;

  case 10: // Statement: Opcode ListMnemonicArgs
#line 437 "nask.yy"
                            { yystack_[0].value.as < std::shared_ptr<ListMnemonicArgs> > ()->reverse();yylhs.value.as < std::shared_ptr<Statement> > () = std::make_shared<MnemonicStmt>(yystack_[1].value.as < std::shared_ptr<Opcode> > (), yystack_[0].value.as < std::shared_ptr<ListMnemonicArgs> > ()); yylhs.value.as < std::shared_ptr<Statement> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Statement> > ()->char_number = yylhs.location.begin.column; driver.statement_ = yylhs.value.as < std::shared_ptr<Statement> > (); }
#line 1188 "parser.cc"
    break;

  case 11: // ListFactor: %empty
#line 439 "nask.yy"
                         { yylhs.value.as < std::shared_ptr<ListFactor> > () = std::make_shared<ListFactor>(); driver.listfactor_ = yylhs.value.as < std::shared_ptr<ListFactor> > (); }
#line 1194 "parser.cc"
    break;

  case 12: // ListFactor: Factor
#line 440 "nask.yy"
           { yylhs.value.as < std::shared_ptr<ListFactor> > () = std::make_shared<ListFactor>(); yylhs.value.as < std::shared_ptr<ListFactor> > ()->cons(yystack_[0].value.as < std::shared_ptr<Factor> > ()); driver.listfactor_ = yylhs.value.as < std::shared_ptr<ListFactor> > (); }
#line 1200 "parser.cc"
    break;

  case 13: // ListFactor: Factor _COMMA ListFactor
#line 441 "nask.yy"
                             { yystack_[0].value.as < std::shared_ptr<ListFactor> > ()->cons(yystack_[2].value.as < std::shared_ptr<Factor> > ()); yylhs.value.as < std::shared_ptr<ListFactor> > () = yystack_[0].value.as < std::shared_ptr<ListFactor> > (); driver.listfactor_ = yylhs.value.as < std::shared_ptr<ListFactor> > (); }
#line 1206 "parser.cc"
    break;

  case 14: // ListMnemonicArgs: %empty
#line 443 "nask.yy"
                               { yylhs.value.as < std::shared_ptr<ListMnemonicArgs> > () = std::make_shared<ListMnemonicArgs>(); driver.listmnemonicargs_ = yylhs.value.as < std::shared_ptr<ListMnemonicArgs> > (); }
#line 1212 "parser.cc"
    break;

  case 15: // ListMnemonicArgs: MnemonicArgs
#line 444 "nask.yy"
                 { yylhs.value.as < std::shared_ptr<ListMnemonicArgs> > () = std::make_shared<ListMnemonicArgs>(); yylhs.value.as < std::shared_ptr<ListMnemonicArgs> > ()->cons(yystack_[0].value.as < std::shared_ptr<MnemonicArgs> > ()); driver.listmnemonicargs_ = yylhs.value.as < std::shared_ptr<ListMnemonicArgs> > (); }
#line 1218 "parser.cc"
    break;

  case 16: // ListMnemonicArgs: MnemonicArgs _COMMA ListMnemonicArgs
#line 445 "nask.yy"
                                         { yystack_[0].value.as < std::shared_ptr<ListMnemonicArgs> > ()->cons(yystack_[2].value.as < std::shared_ptr<MnemonicArgs> > ()); yylhs.value.as < std::shared_ptr<ListMnemonicArgs> > () = yystack_[0].value.as < std::shared_ptr<ListMnemonicArgs> > (); driver.listmnemonicargs_ = yylhs.value.as < std::shared_ptr<ListMnemonicArgs> > (); }
#line 1224 "parser.cc"
    break;

  case 17: // MnemonicArgs: Exp
#line 447 "nask.yy"
                   { yylhs.value.as < std::shared_ptr<MnemonicArgs> > () = std::make_shared<MnemoArg>(yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<MnemonicArgs> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<MnemonicArgs> > ()->char_number = yylhs.location.begin.column; driver.mnemonicargs_ = yylhs.value.as < std::shared_ptr<MnemonicArgs> > (); }
#line 1230 "parser.cc"
    break;

  case 18: // Exp: Exp _PLUS Exp1
#line 449 "nask.yy"
                     { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<PlusExp>(yystack_[2].value.as < std::shared_ptr<Exp> > (), yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1236 "parser.cc"
    break;

  case 19: // Exp: Exp _MINUS Exp1
#line 450 "nask.yy"
                    { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<MinusExp>(yystack_[2].value.as < std::shared_ptr<Exp> > (), yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1242 "parser.cc"
    break;

  case 20: // Exp: Exp1
#line 451 "nask.yy"
         { yylhs.value.as < std::shared_ptr<Exp> > () = yystack_[0].value.as < std::shared_ptr<Exp> > (); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1248 "parser.cc"
    break;

  case 21: // Exp: DataType MemoryAddr
#line 452 "nask.yy"
                        { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<DatatypeExp>(yystack_[1].value.as < std::shared_ptr<DataType> > (), yystack_[0].value.as < std::shared_ptr<MemoryAddr> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1254 "parser.cc"
    break;

  case 22: // Exp: DataType Exp _COLON Exp
#line 453 "nask.yy"
                            { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<SegmentOffsetExp>(yystack_[3].value.as < std::shared_ptr<DataType> > (), yystack_[2].value.as < std::shared_ptr<Exp> > (), yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1260 "parser.cc"
    break;

  case 23: // Exp: MemoryAddr
#line 454 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<MemoryAddrExp>(yystack_[0].value.as < std::shared_ptr<MemoryAddr> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1266 "parser.cc"
    break;

  case 24: // Exp: JumpDir MemoryAddr
#line 455 "nask.yy"
                       { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<JmpMemoryAddrExp>(yystack_[1].value.as < std::shared_ptr<JumpDir> > (), yystack_[0].value.as < std::shared_ptr<MemoryAddr> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1272 "parser.cc"
    break;

  case 25: // Exp1: Exp1 _STAR Exp2
#line 457 "nask.yy"
                       { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<MulExp>(yystack_[2].value.as < std::shared_ptr<Exp> > (), yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1278 "parser.cc"
    break;

  case 26: // Exp1: Exp1 _SLASH Exp2
#line 458 "nask.yy"
                     { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<DivExp>(yystack_[2].value.as < std::shared_ptr<Exp> > (), yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1284 "parser.cc"
    break;

  case 27: // Exp1: Exp1 _PERCENT Exp2
#line 459 "nask.yy"
                       { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<ModExp>(yystack_[2].value.as < std::shared_ptr<Exp> > (), yystack_[0].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1290 "parser.cc"
    break;

  case 28: // Exp1: Exp2
#line 460 "nask.yy"
         { yylhs.value.as < std::shared_ptr<Exp> > () = yystack_[0].value.as < std::shared_ptr<Exp> > (); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1296 "parser.cc"
    break;

  case 29: // Exp2: Factor
#line 462 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Exp> > () = std::make_shared<ImmExp>(yystack_[0].value.as < std::shared_ptr<Factor> > ()); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1302 "parser.cc"
    break;

  case 30: // Exp2: _LPAREN Exp _RPAREN
#line 463 "nask.yy"
                        { yylhs.value.as < std::shared_ptr<Exp> > () = yystack_[1].value.as < std::shared_ptr<Exp> > (); yylhs.value.as < std::shared_ptr<Exp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Exp> > ()->char_number = yylhs.location.begin.column; driver.exp_ = yylhs.value.as < std::shared_ptr<Exp> > (); }
#line 1308 "parser.cc"
    break;

  case 31: // MemoryAddr: _LBRACK Exp _RBRACK
#line 465 "nask.yy"
                                 { yylhs.value.as < std::shared_ptr<MemoryAddr> > () = std::make_shared<Direct>(yystack_[1].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->char_number = yylhs.location.begin.column; driver.memoryaddr_ = yylhs.value.as < std::shared_ptr<MemoryAddr> > (); }
#line 1314 "parser.cc"
    break;

  case 32: // MemoryAddr: _LBRACK T_Id _PLUS Exp _RBRACK
#line 466 "nask.yy"
                                   { yylhs.value.as < std::shared_ptr<MemoryAddr> > () = std::make_shared<BasedOrIndexed>(yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::shared_ptr<Exp> > ()); yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->char_number = yylhs.location.begin.column; driver.memoryaddr_ = yylhs.value.as < std::shared_ptr<MemoryAddr> > (); }
#line 1320 "parser.cc"
    break;

  case 33: // MemoryAddr: _LBRACK IndexExp _PLUS _INTEGER_ _RBRACK
#line 467 "nask.yy"
                                             { yylhs.value.as < std::shared_ptr<MemoryAddr> > () = std::make_shared<Indexed>(yystack_[3].value.as < std::shared_ptr<IndexExp> > (), yystack_[1].value.as < int > ()); yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->char_number = yylhs.location.begin.column; driver.memoryaddr_ = yylhs.value.as < std::shared_ptr<MemoryAddr> > (); }
#line 1326 "parser.cc"
    break;

  case 34: // MemoryAddr: _LBRACK T_Id _PLUS T_Id _RBRACK
#line 468 "nask.yy"
                                    { yylhs.value.as < std::shared_ptr<MemoryAddr> > () = std::make_shared<BasedIndexed>(yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::string > ()); yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->char_number = yylhs.location.begin.column; driver.memoryaddr_ = yylhs.value.as < std::shared_ptr<MemoryAddr> > (); }
#line 1332 "parser.cc"
    break;

  case 35: // MemoryAddr: _LBRACK T_Id _PLUS T_Id _PLUS _INTEGER_ _RBRACK
#line 469 "nask.yy"
                                                    { yylhs.value.as < std::shared_ptr<MemoryAddr> > () = std::make_shared<BasedIndexedDisp>(yystack_[5].value.as < std::string > (), yystack_[3].value.as < std::string > (), yystack_[1].value.as < int > ()); yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->char_number = yylhs.location.begin.column; driver.memoryaddr_ = yylhs.value.as < std::shared_ptr<MemoryAddr> > (); }
#line 1338 "parser.cc"
    break;

  case 36: // MemoryAddr: _LBRACK T_Id _PLUS IndexExp _PLUS _INTEGER_ _RBRACK
#line 470 "nask.yy"
                                                        { yylhs.value.as < std::shared_ptr<MemoryAddr> > () = std::make_shared<BasedIndexedDispScale>(yystack_[5].value.as < std::string > (), yystack_[3].value.as < std::shared_ptr<IndexExp> > (), yystack_[1].value.as < int > ()); yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<MemoryAddr> > ()->char_number = yylhs.location.begin.column; driver.memoryaddr_ = yylhs.value.as < std::shared_ptr<MemoryAddr> > (); }
#line 1344 "parser.cc"
    break;

  case 37: // IndexExp: T_Id _STAR _INTEGER_
#line 472 "nask.yy"
                                { yylhs.value.as < std::shared_ptr<IndexExp> > () = std::make_shared<IndexScaleExp>(yystack_[2].value.as < std::string > (), yystack_[0].value.as < int > ()); yylhs.value.as < std::shared_ptr<IndexExp> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<IndexExp> > ()->char_number = yylhs.location.begin.column; driver.indexexp_ = yylhs.value.as < std::shared_ptr<IndexExp> > (); }
#line 1350 "parser.cc"
    break;

  case 38: // Factor: _INTEGER_
#line 474 "nask.yy"
                   { yylhs.value.as < std::shared_ptr<Factor> > () = std::make_shared<NumberFactor>(yystack_[0].value.as < int > ()); yylhs.value.as < std::shared_ptr<Factor> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Factor> > ()->char_number = yylhs.location.begin.column; driver.factor_ = yylhs.value.as < std::shared_ptr<Factor> > (); }
#line 1356 "parser.cc"
    break;

  case 39: // Factor: T_Hex
#line 475 "nask.yy"
          { yylhs.value.as < std::shared_ptr<Factor> > () = std::make_shared<HexFactor>(yystack_[0].value.as < std::string > ()); yylhs.value.as < std::shared_ptr<Factor> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Factor> > ()->char_number = yylhs.location.begin.column; driver.factor_ = yylhs.value.as < std::shared_ptr<Factor> > (); }
#line 1362 "parser.cc"
    break;

  case 40: // Factor: T_Id
#line 476 "nask.yy"
         { yylhs.value.as < std::shared_ptr<Factor> > () = std::make_shared<IdentFactor>(yystack_[0].value.as < std::string > ()); yylhs.value.as < std::shared_ptr<Factor> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Factor> > ()->char_number = yylhs.location.begin.column; driver.factor_ = yylhs.value.as < std::shared_ptr<Factor> > (); }
#line 1368 "parser.cc"
    break;

  case 41: // Factor: _STRING_
#line 477 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Factor> > () = std::make_shared<StringFactor>(yystack_[0].value.as < std::string > ()); yylhs.value.as < std::shared_ptr<Factor> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Factor> > ()->char_number = yylhs.location.begin.column; driver.factor_ = yylhs.value.as < std::shared_ptr<Factor> > (); }
#line 1374 "parser.cc"
    break;

  case 42: // Factor: T_NaskChar
#line 478 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Factor> > () = std::make_shared<CharFactor>(yystack_[0].value.as < std::string > ()); yylhs.value.as < std::shared_ptr<Factor> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Factor> > ()->char_number = yylhs.location.begin.column; driver.factor_ = yylhs.value.as < std::shared_ptr<Factor> > (); }
#line 1380 "parser.cc"
    break;

  case 43: // JumpDir: _KW_SHORT
#line 480 "nask.yy"
                    { yylhs.value.as < std::shared_ptr<JumpDir> > () = std::make_shared<ShortJumpDir>(); yylhs.value.as < std::shared_ptr<JumpDir> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<JumpDir> > ()->char_number = yylhs.location.begin.column; driver.jumpdir_ = yylhs.value.as < std::shared_ptr<JumpDir> > (); }
#line 1386 "parser.cc"
    break;

  case 44: // JumpDir: _KW_NEAR
#line 481 "nask.yy"
             { yylhs.value.as < std::shared_ptr<JumpDir> > () = std::make_shared<NearJumpDir>(); yylhs.value.as < std::shared_ptr<JumpDir> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<JumpDir> > ()->char_number = yylhs.location.begin.column; driver.jumpdir_ = yylhs.value.as < std::shared_ptr<JumpDir> > (); }
#line 1392 "parser.cc"
    break;

  case 45: // JumpDir: _KW_FAR
#line 482 "nask.yy"
            { yylhs.value.as < std::shared_ptr<JumpDir> > () = std::make_shared<FarJumpDir>(); yylhs.value.as < std::shared_ptr<JumpDir> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<JumpDir> > ()->char_number = yylhs.location.begin.column; driver.jumpdir_ = yylhs.value.as < std::shared_ptr<JumpDir> > (); }
#line 1398 "parser.cc"
    break;

  case 46: // ConfigType: _KW_BITS
#line 484 "nask.yy"
                      { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<BitsConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1404 "parser.cc"
    break;

  case 47: // ConfigType: _KW_INSTRSET
#line 485 "nask.yy"
                 { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<InstConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1410 "parser.cc"
    break;

  case 48: // ConfigType: _KW_OPTIMIZE
#line 486 "nask.yy"
                 { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<OptiConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1416 "parser.cc"
    break;

  case 49: // ConfigType: _KW_FORMAT
#line 487 "nask.yy"
               { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<FormConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1422 "parser.cc"
    break;

  case 50: // ConfigType: _KW_PADDING
#line 488 "nask.yy"
                { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<PaddConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1428 "parser.cc"
    break;

  case 51: // ConfigType: _KW_PADSET
#line 489 "nask.yy"
               { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<PadsConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1434 "parser.cc"
    break;

  case 52: // ConfigType: _KW_SECTION
#line 490 "nask.yy"
                { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<SectConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1440 "parser.cc"
    break;

  case 53: // ConfigType: _KW_ABSOLUTE
#line 491 "nask.yy"
                 { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<AbsoConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1446 "parser.cc"
    break;

  case 54: // ConfigType: _KW_FILE
#line 492 "nask.yy"
             { yylhs.value.as < std::shared_ptr<ConfigType> > () = std::make_shared<FileConfig>(); yylhs.value.as < std::shared_ptr<ConfigType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<ConfigType> > ()->char_number = yylhs.location.begin.column; driver.configtype_ = yylhs.value.as < std::shared_ptr<ConfigType> > (); }
#line 1452 "parser.cc"
    break;

  case 55: // DataType: _KW_BYTE
#line 494 "nask.yy"
                    { yylhs.value.as < std::shared_ptr<DataType> > () = std::make_shared<ByteDataType>(); yylhs.value.as < std::shared_ptr<DataType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<DataType> > ()->char_number = yylhs.location.begin.column; driver.datatype_ = yylhs.value.as < std::shared_ptr<DataType> > (); }
#line 1458 "parser.cc"
    break;

  case 56: // DataType: _KW_WORD
#line 495 "nask.yy"
             { yylhs.value.as < std::shared_ptr<DataType> > () = std::make_shared<WordDataType>(); yylhs.value.as < std::shared_ptr<DataType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<DataType> > ()->char_number = yylhs.location.begin.column; driver.datatype_ = yylhs.value.as < std::shared_ptr<DataType> > (); }
#line 1464 "parser.cc"
    break;

  case 57: // DataType: _KW_DWORD
#line 496 "nask.yy"
              { yylhs.value.as < std::shared_ptr<DataType> > () = std::make_shared<DwordDataType>(); yylhs.value.as < std::shared_ptr<DataType> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<DataType> > ()->char_number = yylhs.location.begin.column; driver.datatype_ = yylhs.value.as < std::shared_ptr<DataType> > (); }
#line 1470 "parser.cc"
    break;

  case 58: // Opcode: _KW_AAA
#line 498 "nask.yy"
                 { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesAAA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1476 "parser.cc"
    break;

  case 59: // Opcode: _KW_AAD
#line 499 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesAAD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1482 "parser.cc"
    break;

  case 60: // Opcode: _KW_AAS
#line 500 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesAAS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1488 "parser.cc"
    break;

  case 61: // Opcode: _KW_AAM
#line 501 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesAAM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1494 "parser.cc"
    break;

  case 62: // Opcode: _KW_ADC
#line 502 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesADC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1500 "parser.cc"
    break;

  case 63: // Opcode: _KW_ADD
#line 503 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesADD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1506 "parser.cc"
    break;

  case 64: // Opcode: _KW_AND
#line 504 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesAND>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1512 "parser.cc"
    break;

  case 65: // Opcode: _KW_ALIGN
#line 505 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesALIGN>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1518 "parser.cc"
    break;

  case 66: // Opcode: _KW_ALIGNB
#line 506 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesALIGNB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1524 "parser.cc"
    break;

  case 67: // Opcode: _KW_ARPL
#line 507 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesARPL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1530 "parser.cc"
    break;

  case 68: // Opcode: _KW_BOUND
#line 508 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBOUND>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1536 "parser.cc"
    break;

  case 69: // Opcode: _KW_BSF
#line 509 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBSF>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1542 "parser.cc"
    break;

  case 70: // Opcode: _KW_BSR
#line 510 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBSR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1548 "parser.cc"
    break;

  case 71: // Opcode: _KW_BSWAP
#line 511 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBSWAP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1554 "parser.cc"
    break;

  case 72: // Opcode: _KW_BT
#line 512 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1560 "parser.cc"
    break;

  case 73: // Opcode: _KW_BTC
#line 513 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBTC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1566 "parser.cc"
    break;

  case 74: // Opcode: _KW_BTR
#line 514 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBTR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1572 "parser.cc"
    break;

  case 75: // Opcode: _KW_BTS
#line 515 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesBTS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1578 "parser.cc"
    break;

  case 76: // Opcode: _KW_CALL
#line 516 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCALL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1584 "parser.cc"
    break;

  case 77: // Opcode: _KW_CBW
#line 517 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCBW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1590 "parser.cc"
    break;

  case 78: // Opcode: _KW_CDQ
#line 518 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCDQ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1596 "parser.cc"
    break;

  case 79: // Opcode: _KW_CLC
#line 519 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCLC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1602 "parser.cc"
    break;

  case 80: // Opcode: _KW_CLD
#line 520 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCLD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1608 "parser.cc"
    break;

  case 81: // Opcode: _KW_CLI
#line 521 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCLI>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1614 "parser.cc"
    break;

  case 82: // Opcode: _KW_CLTS
#line 522 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCLTS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1620 "parser.cc"
    break;

  case 83: // Opcode: _KW_CMC
#line 523 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCMC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1626 "parser.cc"
    break;

  case 84: // Opcode: _KW_CMP
#line 524 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCMP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1632 "parser.cc"
    break;

  case 85: // Opcode: _KW_CMPSB
#line 525 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCMPSB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1638 "parser.cc"
    break;

  case 86: // Opcode: _KW_CMPSD
#line 526 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCMPSD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1644 "parser.cc"
    break;

  case 87: // Opcode: _KW_CMPSW
#line 527 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCMPSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1650 "parser.cc"
    break;

  case 88: // Opcode: _KW_CMPXCHG
#line 528 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCMPXCHG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1656 "parser.cc"
    break;

  case 89: // Opcode: _KW_CPUID
#line 529 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCPUID>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1662 "parser.cc"
    break;

  case 90: // Opcode: _KW_CWD
#line 530 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCWD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1668 "parser.cc"
    break;

  case 91: // Opcode: _KW_CWDE
#line 531 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesCWDE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1674 "parser.cc"
    break;

  case 92: // Opcode: _KW_DAA
#line 532 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDAA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1680 "parser.cc"
    break;

  case 93: // Opcode: _KW_DAS
#line 533 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDAS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1686 "parser.cc"
    break;

  case 94: // Opcode: _KW_DB
#line 534 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1692 "parser.cc"
    break;

  case 95: // Opcode: _KW_DD
#line 535 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1698 "parser.cc"
    break;

  case 96: // Opcode: _KW_DEC
#line 536 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDEC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1704 "parser.cc"
    break;

  case 97: // Opcode: _KW_DIV
#line 537 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDIV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1710 "parser.cc"
    break;

  case 98: // Opcode: _KW_DQ
#line 538 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDQ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1716 "parser.cc"
    break;

  case 99: // Opcode: _KW_DT
#line 539 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1722 "parser.cc"
    break;

  case 100: // Opcode: _KW_DW
#line 540 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesDW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1728 "parser.cc"
    break;

  case 101: // Opcode: _KW_END
#line 541 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesEND>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1734 "parser.cc"
    break;

  case 102: // Opcode: _KW_ENTER
#line 542 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesENTER>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1740 "parser.cc"
    break;

  case 103: // Opcode: _KW_F2XM1
#line 543 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesF2XM1>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1746 "parser.cc"
    break;

  case 104: // Opcode: _KW_FABS
#line 544 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFABS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1752 "parser.cc"
    break;

  case 105: // Opcode: _KW_FADD
#line 545 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFADD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1758 "parser.cc"
    break;

  case 106: // Opcode: _KW_FADDP
#line 546 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFADDP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1764 "parser.cc"
    break;

  case 107: // Opcode: _KW_FBLD
#line 547 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFBLD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1770 "parser.cc"
    break;

  case 108: // Opcode: _KW_FBSTP
#line 548 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFBSTP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1776 "parser.cc"
    break;

  case 109: // Opcode: _KW_FCHS
#line 549 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFCHS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1782 "parser.cc"
    break;

  case 110: // Opcode: _KW_FCLEX
#line 550 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFCLEX>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1788 "parser.cc"
    break;

  case 111: // Opcode: _KW_FCOM
#line 551 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFCOM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1794 "parser.cc"
    break;

  case 112: // Opcode: _KW_FCOMP
#line 552 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFCOMP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1800 "parser.cc"
    break;

  case 113: // Opcode: _KW_FCOMPP
#line 553 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFCOMPP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1806 "parser.cc"
    break;

  case 114: // Opcode: _KW_FCOS
#line 554 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFCOS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1812 "parser.cc"
    break;

  case 115: // Opcode: _KW_FDECSTP
#line 555 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFDECSTP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1818 "parser.cc"
    break;

  case 116: // Opcode: _KW_FDISI
#line 556 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFDISI>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1824 "parser.cc"
    break;

  case 117: // Opcode: _KW_FDIV
#line 557 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFDIV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1830 "parser.cc"
    break;

  case 118: // Opcode: _KW_FDIVP
#line 558 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFDIVP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1836 "parser.cc"
    break;

  case 119: // Opcode: _KW_FDIVR
#line 559 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFDIVR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1842 "parser.cc"
    break;

  case 120: // Opcode: _KW_FDIVRP
#line 560 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFDIVRP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1848 "parser.cc"
    break;

  case 121: // Opcode: _KW_FENI
#line 561 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFENI>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1854 "parser.cc"
    break;

  case 122: // Opcode: _KW_FFREE
#line 562 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFFREE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1860 "parser.cc"
    break;

  case 123: // Opcode: _KW_FIADD
#line 563 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFIADD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1866 "parser.cc"
    break;

  case 124: // Opcode: _KW_FICOM
#line 564 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFICOM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1872 "parser.cc"
    break;

  case 125: // Opcode: _KW_FICOMP
#line 565 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFICOMP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1878 "parser.cc"
    break;

  case 126: // Opcode: _KW_FIDIV
#line 566 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFIDIV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1884 "parser.cc"
    break;

  case 127: // Opcode: _KW_FIDIVR
#line 567 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFIDIVR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1890 "parser.cc"
    break;

  case 128: // Opcode: _KW_FILD
#line 568 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFILD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1896 "parser.cc"
    break;

  case 129: // Opcode: _KW_FIMUL
#line 569 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFIMUL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1902 "parser.cc"
    break;

  case 130: // Opcode: _KW_FINCSTP
#line 570 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFINCSTP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1908 "parser.cc"
    break;

  case 131: // Opcode: _KW_FINIT
#line 571 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFINIT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1914 "parser.cc"
    break;

  case 132: // Opcode: _KW_FIST
#line 572 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFIST>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1920 "parser.cc"
    break;

  case 133: // Opcode: _KW_FISTP
#line 573 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFISTP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1926 "parser.cc"
    break;

  case 134: // Opcode: _KW_FISUB
#line 574 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFISUB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1932 "parser.cc"
    break;

  case 135: // Opcode: _KW_FISUBR
#line 575 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFISUBR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1938 "parser.cc"
    break;

  case 136: // Opcode: _KW_FLD
#line 576 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1944 "parser.cc"
    break;

  case 137: // Opcode: _KW_FLD1
#line 577 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLD1>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1950 "parser.cc"
    break;

  case 138: // Opcode: _KW_FLDCW
#line 578 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDCW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1956 "parser.cc"
    break;

  case 139: // Opcode: _KW_FLDENV
#line 579 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDENV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1962 "parser.cc"
    break;

  case 140: // Opcode: _KW_FLDL2E
#line 580 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDL2E>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1968 "parser.cc"
    break;

  case 141: // Opcode: _KW_FLDL2T
#line 581 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDL2T>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1974 "parser.cc"
    break;

  case 142: // Opcode: _KW_FLDLG2
#line 582 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDLG2>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1980 "parser.cc"
    break;

  case 143: // Opcode: _KW_FLDLN2
#line 583 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDLN2>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1986 "parser.cc"
    break;

  case 144: // Opcode: _KW_FLDPI
#line 584 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDPI>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1992 "parser.cc"
    break;

  case 145: // Opcode: _KW_FLDZ
#line 585 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFLDZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 1998 "parser.cc"
    break;

  case 146: // Opcode: _KW_FMUL
#line 586 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFMUL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2004 "parser.cc"
    break;

  case 147: // Opcode: _KW_FMULP
#line 587 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFMULP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2010 "parser.cc"
    break;

  case 148: // Opcode: _KW_FNCLEX
#line 588 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNCLEX>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2016 "parser.cc"
    break;

  case 149: // Opcode: _KW_FNDISI
#line 589 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNDISI>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2022 "parser.cc"
    break;

  case 150: // Opcode: _KW_FNENI
#line 590 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNENI>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2028 "parser.cc"
    break;

  case 151: // Opcode: _KW_FNINIT
#line 591 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNINIT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2034 "parser.cc"
    break;

  case 152: // Opcode: _KW_FNOP
#line 592 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNOP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2040 "parser.cc"
    break;

  case 153: // Opcode: _KW_FNSAVE
#line 593 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNSAVE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2046 "parser.cc"
    break;

  case 154: // Opcode: _KW_FNSTCW
#line 594 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNSTCW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2052 "parser.cc"
    break;

  case 155: // Opcode: _KW_FNSTENV
#line 595 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNSTENV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2058 "parser.cc"
    break;

  case 156: // Opcode: _KW_FNSTSW
#line 596 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFNSTSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2064 "parser.cc"
    break;

  case 157: // Opcode: _KW_FPATAN
#line 597 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFPATAN>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2070 "parser.cc"
    break;

  case 158: // Opcode: _KW_FPTAN
#line 598 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFPTAN>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2076 "parser.cc"
    break;

  case 159: // Opcode: _KW_FPREM
#line 599 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFPREM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2082 "parser.cc"
    break;

  case 160: // Opcode: _KW_FPREM1
#line 600 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFPREM1>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2088 "parser.cc"
    break;

  case 161: // Opcode: _KW_FRNDINT
#line 601 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFRNDINT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2094 "parser.cc"
    break;

  case 162: // Opcode: _KW_FRSTOR
#line 602 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFRSTOR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2100 "parser.cc"
    break;

  case 163: // Opcode: _KW_FSAVE
#line 603 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSAVE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2106 "parser.cc"
    break;

  case 164: // Opcode: _KW_FSCALE
#line 604 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSCALE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2112 "parser.cc"
    break;

  case 165: // Opcode: _KW_FSETPM
#line 605 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSETPM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2118 "parser.cc"
    break;

  case 166: // Opcode: _KW_FSIN
#line 606 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSIN>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2124 "parser.cc"
    break;

  case 167: // Opcode: _KW_FSINCOS
#line 607 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSINCOS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2130 "parser.cc"
    break;

  case 168: // Opcode: _KW_FSQRT
#line 608 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSQRT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2136 "parser.cc"
    break;

  case 169: // Opcode: _KW_FST
#line 609 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFST>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2142 "parser.cc"
    break;

  case 170: // Opcode: _KW_FSTCW
#line 610 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSTCW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2148 "parser.cc"
    break;

  case 171: // Opcode: _KW_FSTENV
#line 611 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSTENV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2154 "parser.cc"
    break;

  case 172: // Opcode: _KW_FSTP
#line 612 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSTP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2160 "parser.cc"
    break;

  case 173: // Opcode: _KW_FSTSW
#line 613 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSTSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2166 "parser.cc"
    break;

  case 174: // Opcode: _KW_FSUB
#line 614 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSUB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2172 "parser.cc"
    break;

  case 175: // Opcode: _KW_FSUBP
#line 615 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSUBP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2178 "parser.cc"
    break;

  case 176: // Opcode: _KW_FSUBR
#line 616 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSUBR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2184 "parser.cc"
    break;

  case 177: // Opcode: _KW_FSUBRP
#line 617 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFSUBRP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2190 "parser.cc"
    break;

  case 178: // Opcode: _KW_FTST
#line 618 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFTST>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2196 "parser.cc"
    break;

  case 179: // Opcode: _KW_FUCOM
#line 619 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFUCOM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2202 "parser.cc"
    break;

  case 180: // Opcode: _KW_FUCOMP
#line 620 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFUCOMP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2208 "parser.cc"
    break;

  case 181: // Opcode: _KW_FUCOMPP
#line 621 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFUCOMPP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2214 "parser.cc"
    break;

  case 182: // Opcode: _KW_FXAM
#line 622 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFXAM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2220 "parser.cc"
    break;

  case 183: // Opcode: _KW_FXCH
#line 623 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFXCH>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2226 "parser.cc"
    break;

  case 184: // Opcode: _KW_FXTRACT
#line 624 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFXTRACT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2232 "parser.cc"
    break;

  case 185: // Opcode: _KW_FYL2X
#line 625 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFYL2X>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2238 "parser.cc"
    break;

  case 186: // Opcode: _KW_FYL2XP1
#line 626 "nask.yy"
                { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesFYL2XP1>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2244 "parser.cc"
    break;

  case 187: // Opcode: _KW_HLT
#line 627 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesHLT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2250 "parser.cc"
    break;

  case 188: // Opcode: _KW_IDIV
#line 628 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesIDIV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2256 "parser.cc"
    break;

  case 189: // Opcode: _KW_IMUL
#line 629 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesIMUL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2262 "parser.cc"
    break;

  case 190: // Opcode: _KW_IN
#line 630 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesIN>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2268 "parser.cc"
    break;

  case 191: // Opcode: _KW_INC
#line 631 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2274 "parser.cc"
    break;

  case 192: // Opcode: _KW_INCO
#line 632 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINCO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2280 "parser.cc"
    break;

  case 193: // Opcode: _KW_INSB
#line 633 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINSB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2286 "parser.cc"
    break;

  case 194: // Opcode: _KW_INSD
#line 634 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINSD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2292 "parser.cc"
    break;

  case 195: // Opcode: _KW_INSW
#line 635 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2298 "parser.cc"
    break;

  case 196: // Opcode: _KW_INT
#line 636 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2304 "parser.cc"
    break;

  case 197: // Opcode: _KW_INT3
#line 637 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINT3>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2310 "parser.cc"
    break;

  case 198: // Opcode: _KW_INTO
#line 638 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINTO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2316 "parser.cc"
    break;

  case 199: // Opcode: _KW_INVD
#line 639 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINVD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2322 "parser.cc"
    break;

  case 200: // Opcode: _KW_INVLPG
#line 640 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesINVLPG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2328 "parser.cc"
    break;

  case 201: // Opcode: _KW_IRET
#line 641 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesIRET>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2334 "parser.cc"
    break;

  case 202: // Opcode: _KW_IRETD
#line 642 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesIRETD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2340 "parser.cc"
    break;

  case 203: // Opcode: _KW_IRETW
#line 643 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesIRETW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2346 "parser.cc"
    break;

  case 204: // Opcode: _KW_JA
#line 644 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2352 "parser.cc"
    break;

  case 205: // Opcode: _KW_JAE
#line 645 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJAE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2358 "parser.cc"
    break;

  case 206: // Opcode: _KW_JB
#line 646 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2364 "parser.cc"
    break;

  case 207: // Opcode: _KW_JBE
#line 647 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJBE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2370 "parser.cc"
    break;

  case 208: // Opcode: _KW_JC
#line 648 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2376 "parser.cc"
    break;

  case 209: // Opcode: _KW_JCXZ
#line 649 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJCXZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2382 "parser.cc"
    break;

  case 210: // Opcode: _KW_JE
#line 650 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2388 "parser.cc"
    break;

  case 211: // Opcode: _KW_JECXZ
#line 651 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJECXZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2394 "parser.cc"
    break;

  case 212: // Opcode: _KW_JG
#line 652 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2400 "parser.cc"
    break;

  case 213: // Opcode: _KW_JGE
#line 653 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJGE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2406 "parser.cc"
    break;

  case 214: // Opcode: _KW_JL
#line 654 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2412 "parser.cc"
    break;

  case 215: // Opcode: _KW_JLE
#line 655 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJLE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2418 "parser.cc"
    break;

  case 216: // Opcode: _KW_JMP
#line 656 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJMP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2424 "parser.cc"
    break;

  case 217: // Opcode: _KW_JNA
#line 657 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2430 "parser.cc"
    break;

  case 218: // Opcode: _KW_JNAE
#line 658 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNAE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2436 "parser.cc"
    break;

  case 219: // Opcode: _KW_JNB
#line 659 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2442 "parser.cc"
    break;

  case 220: // Opcode: _KW_JNBE
#line 660 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNBE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2448 "parser.cc"
    break;

  case 221: // Opcode: _KW_JNC
#line 661 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2454 "parser.cc"
    break;

  case 222: // Opcode: _KW_JNE
#line 662 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2460 "parser.cc"
    break;

  case 223: // Opcode: _KW_JNG
#line 663 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2466 "parser.cc"
    break;

  case 224: // Opcode: _KW_JNGE
#line 664 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNGE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2472 "parser.cc"
    break;

  case 225: // Opcode: _KW_JNL
#line 665 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2478 "parser.cc"
    break;

  case 226: // Opcode: _KW_JNLE
#line 666 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNLE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2484 "parser.cc"
    break;

  case 227: // Opcode: _KW_JNO
#line 667 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2490 "parser.cc"
    break;

  case 228: // Opcode: _KW_JNP
#line 668 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2496 "parser.cc"
    break;

  case 229: // Opcode: _KW_JNS
#line 669 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2502 "parser.cc"
    break;

  case 230: // Opcode: _KW_JNZ
#line 670 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJNZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2508 "parser.cc"
    break;

  case 231: // Opcode: _KW_JO
#line 671 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2514 "parser.cc"
    break;

  case 232: // Opcode: _KW_JP
#line 672 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2520 "parser.cc"
    break;

  case 233: // Opcode: _KW_JPE
#line 673 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJPE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2526 "parser.cc"
    break;

  case 234: // Opcode: _KW_JPO
#line 674 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJPO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2532 "parser.cc"
    break;

  case 235: // Opcode: _KW_JS
#line 675 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2538 "parser.cc"
    break;

  case 236: // Opcode: _KW_JZ
#line 676 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesJZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2544 "parser.cc"
    break;

  case 237: // Opcode: _KW_LAHF
#line 677 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLAHF>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2550 "parser.cc"
    break;

  case 238: // Opcode: _KW_LAR
#line 678 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLAR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2556 "parser.cc"
    break;

  case 239: // Opcode: _KW_LDS
#line 679 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLDS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2562 "parser.cc"
    break;

  case 240: // Opcode: _KW_LEA
#line 680 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLEA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2568 "parser.cc"
    break;

  case 241: // Opcode: _KW_LEAVE
#line 681 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLEAVE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2574 "parser.cc"
    break;

  case 242: // Opcode: _KW_LES
#line 682 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLES>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2580 "parser.cc"
    break;

  case 243: // Opcode: _KW_LFS
#line 683 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLFS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2586 "parser.cc"
    break;

  case 244: // Opcode: _KW_LGDT
#line 684 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLGDT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2592 "parser.cc"
    break;

  case 245: // Opcode: _KW_LGS
#line 685 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLGS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2598 "parser.cc"
    break;

  case 246: // Opcode: _KW_LIDT
#line 686 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLIDT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2604 "parser.cc"
    break;

  case 247: // Opcode: _KW_LLDT
#line 687 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLLDT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2610 "parser.cc"
    break;

  case 248: // Opcode: _KW_LMSW
#line 688 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLMSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2616 "parser.cc"
    break;

  case 249: // Opcode: _KW_LOCK
#line 689 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLOCK>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2622 "parser.cc"
    break;

  case 250: // Opcode: _KW_LODSB
#line 690 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLODSB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2628 "parser.cc"
    break;

  case 251: // Opcode: _KW_LODSD
#line 691 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLODSD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2634 "parser.cc"
    break;

  case 252: // Opcode: _KW_LODSW
#line 692 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLODSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2640 "parser.cc"
    break;

  case 253: // Opcode: _KW_LOOP
#line 693 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLOOP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2646 "parser.cc"
    break;

  case 254: // Opcode: _KW_LOOPE
#line 694 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLOOPE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2652 "parser.cc"
    break;

  case 255: // Opcode: _KW_LOOPNE
#line 695 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLOOPNE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2658 "parser.cc"
    break;

  case 256: // Opcode: _KW_LOOPNZ
#line 696 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLOOPNZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2664 "parser.cc"
    break;

  case 257: // Opcode: _KW_LOOPZ
#line 697 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLOOPZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2670 "parser.cc"
    break;

  case 258: // Opcode: _KW_LSL
#line 698 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLSL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2676 "parser.cc"
    break;

  case 259: // Opcode: _KW_LSS
#line 699 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLSS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2682 "parser.cc"
    break;

  case 260: // Opcode: _KW_LTR
#line 700 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesLTR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2688 "parser.cc"
    break;

  case 261: // Opcode: _KW_MOV
#line 701 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesMOV>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2694 "parser.cc"
    break;

  case 262: // Opcode: _KW_MOVSB
#line 702 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesMOVSB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2700 "parser.cc"
    break;

  case 263: // Opcode: _KW_MOVSD
#line 703 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesMOVSD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2706 "parser.cc"
    break;

  case 264: // Opcode: _KW_MOVSW
#line 704 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesMOVSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2712 "parser.cc"
    break;

  case 265: // Opcode: _KW_MOVSX
#line 705 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesMOVSX>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2718 "parser.cc"
    break;

  case 266: // Opcode: _KW_MOVZX
#line 706 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesMOVZX>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2724 "parser.cc"
    break;

  case 267: // Opcode: _KW_MUL
#line 707 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesMUL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2730 "parser.cc"
    break;

  case 268: // Opcode: _KW_NEG
#line 708 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesNEG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2736 "parser.cc"
    break;

  case 269: // Opcode: _KW_NOP
#line 709 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesNOP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2742 "parser.cc"
    break;

  case 270: // Opcode: _KW_NOT
#line 710 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesNOT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2748 "parser.cc"
    break;

  case 271: // Opcode: _KW_OR
#line 711 "nask.yy"
           { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesOR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2754 "parser.cc"
    break;

  case 272: // Opcode: _KW_ORG
#line 712 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesORG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2760 "parser.cc"
    break;

  case 273: // Opcode: _KW_OUT
#line 713 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesOUT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2766 "parser.cc"
    break;

  case 274: // Opcode: _KW_OUTSB
#line 714 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesOUTSB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2772 "parser.cc"
    break;

  case 275: // Opcode: _KW_OUTSD
#line 715 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesOUTSD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2778 "parser.cc"
    break;

  case 276: // Opcode: _KW_OUTSW
#line 716 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesOUTSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2784 "parser.cc"
    break;

  case 277: // Opcode: _KW_POP
#line 717 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPOP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2790 "parser.cc"
    break;

  case 278: // Opcode: _KW_POPA
#line 718 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPOPA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2796 "parser.cc"
    break;

  case 279: // Opcode: _KW_POPAD
#line 719 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPOPAD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2802 "parser.cc"
    break;

  case 280: // Opcode: _KW_POPAW
#line 720 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPOPAW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2808 "parser.cc"
    break;

  case 281: // Opcode: _KW_POPF
#line 721 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPOPF>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2814 "parser.cc"
    break;

  case 282: // Opcode: _KW_POPFD
#line 722 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPOPFD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2820 "parser.cc"
    break;

  case 283: // Opcode: _KW_POPFW
#line 723 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPOPFW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2826 "parser.cc"
    break;

  case 284: // Opcode: _KW_PUSH
#line 724 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSH>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2832 "parser.cc"
    break;

  case 285: // Opcode: _KW_PUSHA
#line 725 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSHA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2838 "parser.cc"
    break;

  case 286: // Opcode: _KW_PUSHD
#line 726 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSHD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2844 "parser.cc"
    break;

  case 287: // Opcode: _KW_PUSHAD
#line 727 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSHAD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2850 "parser.cc"
    break;

  case 288: // Opcode: _KW_PUSHAW
#line 728 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSHAW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2856 "parser.cc"
    break;

  case 289: // Opcode: _KW_PUSHF
#line 729 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSHF>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2862 "parser.cc"
    break;

  case 290: // Opcode: _KW_PUSHFD
#line 730 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSHFD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2868 "parser.cc"
    break;

  case 291: // Opcode: _KW_PUSHFW
#line 731 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesPUSHFW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2874 "parser.cc"
    break;

  case 292: // Opcode: _KW_RCL
#line 732 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRCL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2880 "parser.cc"
    break;

  case 293: // Opcode: _KW_RCR
#line 733 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRCR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2886 "parser.cc"
    break;

  case 294: // Opcode: _KW_RDMSR
#line 734 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRDMSR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2892 "parser.cc"
    break;

  case 295: // Opcode: _KW_RDPMC
#line 735 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRDPMC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2898 "parser.cc"
    break;

  case 296: // Opcode: _KW_REP
#line 736 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesREP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2904 "parser.cc"
    break;

  case 297: // Opcode: _KW_REPE
#line 737 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesREPE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2910 "parser.cc"
    break;

  case 298: // Opcode: _KW_REPNE
#line 738 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesREPNE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2916 "parser.cc"
    break;

  case 299: // Opcode: _KW_REPNZ
#line 739 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesREPNZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2922 "parser.cc"
    break;

  case 300: // Opcode: _KW_REPZ
#line 740 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesREPZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2928 "parser.cc"
    break;

  case 301: // Opcode: _KW_RESB
#line 741 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRESB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2934 "parser.cc"
    break;

  case 302: // Opcode: _KW_RESD
#line 742 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRESD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2940 "parser.cc"
    break;

  case 303: // Opcode: _KW_RESQ
#line 743 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRESQ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2946 "parser.cc"
    break;

  case 304: // Opcode: _KW_REST
#line 744 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesREST>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2952 "parser.cc"
    break;

  case 305: // Opcode: _KW_RESW
#line 745 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRESW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2958 "parser.cc"
    break;

  case 306: // Opcode: _KW_RET
#line 746 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRET>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2964 "parser.cc"
    break;

  case 307: // Opcode: _KW_RETF
#line 747 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRETF>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2970 "parser.cc"
    break;

  case 308: // Opcode: _KW_RETN
#line 748 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRETN>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2976 "parser.cc"
    break;

  case 309: // Opcode: _KW_ROL
#line 749 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesROL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2982 "parser.cc"
    break;

  case 310: // Opcode: _KW_ROR
#line 750 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesROR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2988 "parser.cc"
    break;

  case 311: // Opcode: _KW_RSM
#line 751 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesRSM>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 2994 "parser.cc"
    break;

  case 312: // Opcode: _KW_SAHF
#line 752 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSAHF>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3000 "parser.cc"
    break;

  case 313: // Opcode: _KW_SAL
#line 753 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSAL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3006 "parser.cc"
    break;

  case 314: // Opcode: _KW_SAR
#line 754 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSAR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3012 "parser.cc"
    break;

  case 315: // Opcode: _KW_SBB
#line 755 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSBB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3018 "parser.cc"
    break;

  case 316: // Opcode: _KW_SCASB
#line 756 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSCASB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3024 "parser.cc"
    break;

  case 317: // Opcode: _KW_SCASD
#line 757 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSCASD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3030 "parser.cc"
    break;

  case 318: // Opcode: _KW_SCASW
#line 758 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSCASW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3036 "parser.cc"
    break;

  case 319: // Opcode: _KW_SETA
#line 759 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3042 "parser.cc"
    break;

  case 320: // Opcode: _KW_SETAE
#line 760 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETAE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3048 "parser.cc"
    break;

  case 321: // Opcode: _KW_SETB
#line 761 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3054 "parser.cc"
    break;

  case 322: // Opcode: _KW_SETBE
#line 762 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETBE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3060 "parser.cc"
    break;

  case 323: // Opcode: _KW_SETC
#line 763 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3066 "parser.cc"
    break;

  case 324: // Opcode: _KW_SETE
#line 764 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3072 "parser.cc"
    break;

  case 325: // Opcode: _KW_SETG
#line 765 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3078 "parser.cc"
    break;

  case 326: // Opcode: _KW_SETGE
#line 766 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETGE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3084 "parser.cc"
    break;

  case 327: // Opcode: _KW_SETL
#line 767 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3090 "parser.cc"
    break;

  case 328: // Opcode: _KW_SETLE
#line 768 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETLE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3096 "parser.cc"
    break;

  case 329: // Opcode: _KW_SETNA
#line 769 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNA>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3102 "parser.cc"
    break;

  case 330: // Opcode: _KW_SETNAE
#line 770 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNAE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3108 "parser.cc"
    break;

  case 331: // Opcode: _KW_SETNB
#line 771 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3114 "parser.cc"
    break;

  case 332: // Opcode: _KW_SETNBE
#line 772 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNBE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3120 "parser.cc"
    break;

  case 333: // Opcode: _KW_SETNC
#line 773 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3126 "parser.cc"
    break;

  case 334: // Opcode: _KW_SETNE
#line 774 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3132 "parser.cc"
    break;

  case 335: // Opcode: _KW_SETNG
#line 775 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3138 "parser.cc"
    break;

  case 336: // Opcode: _KW_SETNGE
#line 776 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNGE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3144 "parser.cc"
    break;

  case 337: // Opcode: _KW_SETNL
#line 777 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3150 "parser.cc"
    break;

  case 338: // Opcode: _KW_SETNLE
#line 778 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNLE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3156 "parser.cc"
    break;

  case 339: // Opcode: _KW_SETNO
#line 779 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3162 "parser.cc"
    break;

  case 340: // Opcode: _KW_SETNP
#line 780 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3168 "parser.cc"
    break;

  case 341: // Opcode: _KW_SETNS
#line 781 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3174 "parser.cc"
    break;

  case 342: // Opcode: _KW_SETNZ
#line 782 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETNZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3180 "parser.cc"
    break;

  case 343: // Opcode: _KW_SETO
#line 783 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3186 "parser.cc"
    break;

  case 344: // Opcode: _KW_SETP
#line 784 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETP>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3192 "parser.cc"
    break;

  case 345: // Opcode: _KW_SETPE
#line 785 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETPE>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3198 "parser.cc"
    break;

  case 346: // Opcode: _KW_SETPO
#line 786 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETPO>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3204 "parser.cc"
    break;

  case 347: // Opcode: _KW_SETS
#line 787 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETS>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3210 "parser.cc"
    break;

  case 348: // Opcode: _KW_SETZ
#line 788 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSETZ>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3216 "parser.cc"
    break;

  case 349: // Opcode: _KW_SGDT
#line 789 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSGDT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3222 "parser.cc"
    break;

  case 350: // Opcode: _KW_SHL
#line 790 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSHL>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3228 "parser.cc"
    break;

  case 351: // Opcode: _KW_SHLD
#line 791 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSHLD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3234 "parser.cc"
    break;

  case 352: // Opcode: _KW_SHR
#line 792 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSHR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3240 "parser.cc"
    break;

  case 353: // Opcode: _KW_SHRD
#line 793 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSHRD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3246 "parser.cc"
    break;

  case 354: // Opcode: _KW_SIDT
#line 794 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSIDT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3252 "parser.cc"
    break;

  case 355: // Opcode: _KW_SLDT
#line 795 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSLDT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3258 "parser.cc"
    break;

  case 356: // Opcode: _KW_SMSW
#line 796 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSMSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3264 "parser.cc"
    break;

  case 357: // Opcode: _KW_STC
#line 797 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSTC>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3270 "parser.cc"
    break;

  case 358: // Opcode: _KW_STD
#line 798 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSTD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3276 "parser.cc"
    break;

  case 359: // Opcode: _KW_STI
#line 799 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSTI>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3282 "parser.cc"
    break;

  case 360: // Opcode: _KW_STOSB
#line 800 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSTOSB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3288 "parser.cc"
    break;

  case 361: // Opcode: _KW_STOSD
#line 801 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSTOSD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3294 "parser.cc"
    break;

  case 362: // Opcode: _KW_STOSW
#line 802 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSTOSW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3300 "parser.cc"
    break;

  case 363: // Opcode: _KW_STR
#line 803 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSTR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3306 "parser.cc"
    break;

  case 364: // Opcode: _KW_SUB
#line 804 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesSUB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3312 "parser.cc"
    break;

  case 365: // Opcode: _KW_TEST
#line 805 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesTEST>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3318 "parser.cc"
    break;

  case 366: // Opcode: _KW_TIMES
#line 806 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesTIMES>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3324 "parser.cc"
    break;

  case 367: // Opcode: _KW_UD2
#line 807 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesUD2>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3330 "parser.cc"
    break;

  case 368: // Opcode: _KW_VERR
#line 808 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesVERR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3336 "parser.cc"
    break;

  case 369: // Opcode: _KW_VERW
#line 809 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesVERW>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3342 "parser.cc"
    break;

  case 370: // Opcode: _KW_WAIT
#line 810 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesWAIT>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3348 "parser.cc"
    break;

  case 371: // Opcode: _KW_WBINVD
#line 811 "nask.yy"
               { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesWBINVD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3354 "parser.cc"
    break;

  case 372: // Opcode: _KW_WRMSR
#line 812 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesWRMSR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3360 "parser.cc"
    break;

  case 373: // Opcode: _KW_XADD
#line 813 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesXADD>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3366 "parser.cc"
    break;

  case 374: // Opcode: _KW_XCHG
#line 814 "nask.yy"
             { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesXCHG>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3372 "parser.cc"
    break;

  case 375: // Opcode: _KW_XLATB
#line 815 "nask.yy"
              { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesXLATB>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3378 "parser.cc"
    break;

  case 376: // Opcode: _KW_XOR
#line 816 "nask.yy"
            { yylhs.value.as < std::shared_ptr<Opcode> > () = std::make_shared<OpcodesXOR>(); yylhs.value.as < std::shared_ptr<Opcode> > ()->line_number = yylhs.location.begin.line; yylhs.value.as < std::shared_ptr<Opcode> > ()->char_number = yylhs.location.begin.column; driver.opcode_ = yylhs.value.as < std::shared_ptr<Opcode> > (); }
#line 3384 "parser.cc"
    break;


#line 3388 "parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  NaskParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  NaskParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const short NaskParser::yypact_ninf_ = -358;

  const signed char NaskParser::yytable_ninf_ = -1;

  const short
  NaskParser::yypact_[] =
  {
     366,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -316,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -316,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,  -358,
    -358,  -358,   -12,   -47,  -358,    30,  -358,   366,    -5,  -358,
    -358,  -358,  -358,  -358,  -358,    26,  -358,  -358,  -358,  -358,
    -358,  -358,  -358,  -358,  -358,  -358,  -316,    -5,  -358,  -358,
      -5,  -358,  -358,  -358,  -358,  -358,  -358,     5,  -358,    33,
      19,    10,  -358,  -358,  -358,  -306,    -5,  -316,  -301,    19,
      37,     0,    -7,    44,    -5,    21,    21,    21,    21,    21,
    -358,    41,  -358,  -358,  -358,  -358,  -300,    15,  -358,  -299,
    -358,    10,    10,  -358,  -358,  -358,    -5,  -358,    24,     3,
      48,  -292,    19,  -297,  -358,  -358,  -296,  -358,  -289,  -286,
    -358,  -358
  };

  const short
  NaskParser::yydefact_[] =
  {
       0,    58,    59,    61,    60,    62,    63,    65,    66,    64,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    11,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   159,   160,   158,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    11,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   287,   288,   286,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,     0,     0,     5,     0,     2,     3,    14,    39,
      40,    42,    41,    38,     8,    12,     7,    53,    46,    54,
      49,    47,    48,    50,    51,    52,     0,     0,     1,     4,
       0,    55,    57,    45,    44,    43,    56,     0,    10,    15,
      17,    20,    28,    23,    29,     0,     0,    11,     0,     6,
       0,    40,     0,     0,    14,     0,     0,     0,     0,     0,
      24,     0,    21,    13,     9,    30,     0,     0,    31,     0,
      16,    18,    19,    27,    25,    26,     0,    37,    40,     0,
       0,     0,    22,     0,    34,    32,     0,    33,     0,     0,
      35,    36
  };

  const short
  NaskParser::yypgoto_[] =
  {
    -358,  -358,  -261,  -358,  -127,  -307,  -358,  -341,  -357,  -355,
    -332,  -319,   -44,  -358,  -358,  -358,  -358
  };

  const short
  NaskParser::yydefgoto_[] =
  {
       0,   325,   326,   327,   334,   358,   359,   360,   361,   362,
     363,   373,   364,   365,   346,   366,   328
  };

  const short
  NaskParser::yytable_[] =
  {
     350,   375,   335,   376,   336,   337,   369,   386,   387,   370,
     350,   375,   338,   376,   377,   347,   372,   378,   391,   392,
     350,   379,   393,   394,   395,   381,   350,   375,   351,   376,
     348,   386,   403,   380,   382,   367,   329,   330,   351,   331,
     332,   333,   374,   385,   357,   375,   399,   376,   351,   375,
     384,   376,   389,   396,   352,   402,   406,   397,   401,   407,
     408,   409,   410,   353,   352,   411,   349,   390,   400,     0,
       0,     0,     0,   353,   352,     0,     0,     0,     0,   339,
       0,     0,     0,   353,     0,     0,     0,   335,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   342,     0,     0,   354,
       0,     0,     0,   343,   344,     0,     0,     0,     0,   354,
     383,     0,     0,     0,     0,     0,     0,     0,     0,   354,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   355,     0,
       0,     0,     0,   335,     0,     0,     0,     0,   355,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   355,   356,
       0,     0,     0,     0,   388,   357,     0,   329,   330,   356,
     331,   332,   333,     0,   405,   357,     0,   329,   371,   356,
     331,   332,   333,     0,     0,   357,     0,   329,   398,     0,
     331,   332,   333,   329,   330,   404,   331,   332,   333,     1,
       2,     3,     4,     0,     5,     6,     7,     8,     9,    10,
       0,    11,    12,    13,    14,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,     0,    44,    45,     0,    46,
      47,    48,    49,    50,     0,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,     0,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,     0,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
       0,   213,   214,   215,     0,   216,   217,   218,   219,   220,
     221,     0,     0,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,     0,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
       0,   317,   318,   319,   320,   321,   322,     0,     0,   323,
     324
  };

  const short
  NaskParser::yycheck_[] =
  {
       5,     8,    46,    10,   131,    17,   347,     7,     8,   350,
       5,     8,    24,    10,     4,    62,   357,     7,   375,   376,
       5,    11,   377,   378,   379,   366,     5,     8,    33,    10,
       0,     7,     8,   365,   366,     9,   352,   353,    33,   355,
     356,   357,     9,     6,   350,     8,   387,    10,    33,     8,
     351,    10,     8,    12,    59,   396,     8,   357,   357,   351,
     357,   357,   351,    68,    59,   351,   327,   374,   387,    -1,
      -1,    -1,    -1,    68,    59,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    68,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   238,    -1,    -1,   234,
      -1,    -1,    -1,   245,   246,    -1,    -1,    -1,    -1,   234,
     367,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   234,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   289,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,
      -1,    -1,    -1,   367,    -1,    -1,    -1,    -1,   323,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   323,   344,
      -1,    -1,    -1,    -1,   351,   350,    -1,   352,   353,   344,
     355,   356,   357,    -1,   351,   350,    -1,   352,   353,   344,
     355,   356,   357,    -1,    -1,   350,    -1,   352,   353,    -1,
     355,   356,   357,   352,   353,   351,   355,   356,   357,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      -1,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    -1,    60,    61,    -1,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,    -1,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
      -1,   235,   236,   237,    -1,   239,   240,   241,   242,   243,
     244,    -1,    -1,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,    -1,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    -1,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
      -1,   345,   346,   347,   348,   349,   350,    -1,    -1,   353,
     354
  };

  const short
  NaskParser::yystos_[] =
  {
       0,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    25,    26,    27,    28,    29,    30,    31,    32,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    60,    61,    63,    64,    65,    66,
      67,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   235,   236,   237,   239,   240,   241,   242,
     243,   244,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   345,   346,   347,
     348,   349,   350,   353,   354,   359,   360,   361,   374,   352,
     353,   355,   356,   357,   362,   370,   362,    17,    24,    91,
     120,   160,   238,   245,   246,   289,   372,    62,     0,   360,
       5,    33,    59,    68,   234,   323,   344,   350,   363,   364,
     365,   366,   367,   368,   370,   371,   373,     9,   370,   365,
     365,   353,   365,   369,     9,     8,    10,     4,     7,    11,
     368,   365,   368,   362,   351,     6,     7,     8,   351,     8,
     363,   366,   366,   367,   367,   367,    12,   357,   353,   365,
     369,   357,   365,     8,   351,   351,     8,   351,   357,   357,
     351,   351
  };

  const short
  NaskParser::yyr1_[] =
  {
       0,   358,   359,   360,   360,   361,   361,   361,   361,   361,
     361,   362,   362,   362,   363,   363,   363,   364,   365,   365,
     365,   365,   365,   365,   365,   366,   366,   366,   366,   367,
     367,   368,   368,   368,   368,   368,   368,   369,   370,   370,
     370,   370,   370,   371,   371,   371,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   373,   373,   373,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374
  };

  const signed char
  NaskParser::yyr2_[] =
  {
       0,     2,     1,     1,     2,     1,     3,     2,     2,     4,
       2,     0,     1,     3,     0,     1,     3,     1,     3,     3,
       1,     2,     4,     1,     2,     3,     3,     3,     1,     1,
       3,     3,     5,     5,     5,     7,     7,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const NaskParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "_ERROR_", "_PERCENT",
  "_LPAREN", "_RPAREN", "_STAR", "_PLUS", "_COMMA", "_MINUS", "_SLASH",
  "_COLON", "_KW_AAA", "_KW_AAD", "_KW_AAM", "_KW_AAS", "_KW_ABSOLUTE",
  "_KW_ADC", "_KW_ADD", "_KW_ALIGN", "_KW_ALIGNB", "_KW_AND", "_KW_ARPL",
  "_KW_BITS", "_KW_BOUND", "_KW_BSF", "_KW_BSR", "_KW_BSWAP", "_KW_BT",
  "_KW_BTC", "_KW_BTR", "_KW_BTS", "_KW_BYTE", "_KW_CALL", "_KW_CBW",
  "_KW_CDQ", "_KW_CLC", "_KW_CLD", "_KW_CLI", "_KW_CLTS", "_KW_CMC",
  "_KW_CMP", "_KW_CMPSB", "_KW_CMPSD", "_KW_CMPSW", "_KW_CMPXCHG",
  "_KW_CPUID", "_KW_CWD", "_KW_CWDE", "_KW_DAA", "_KW_DAS", "_KW_DB",
  "_KW_DD", "_KW_DEC", "_KW_DIV", "_KW_DQ", "_KW_DT", "_KW_DW",
  "_KW_DWORD", "_KW_END", "_KW_ENTER", "_KW_EQU", "_KW_EXTERN",
  "_KW_F2XM1", "_KW_FABS", "_KW_FADD", "_KW_FADDP", "_KW_FAR", "_KW_FBLD",
  "_KW_FBSTP", "_KW_FCHS", "_KW_FCLEX", "_KW_FCOM", "_KW_FCOMP",
  "_KW_FCOMPP", "_KW_FCOS", "_KW_FDECSTP", "_KW_FDISI", "_KW_FDIV",
  "_KW_FDIVP", "_KW_FDIVR", "_KW_FDIVRP", "_KW_FENI", "_KW_FFREE",
  "_KW_FIADD", "_KW_FICOM", "_KW_FICOMP", "_KW_FIDIV", "_KW_FIDIVR",
  "_KW_FILD", "_KW_FILE", "_KW_FIMUL", "_KW_FINCSTP", "_KW_FINIT",
  "_KW_FIST", "_KW_FISTP", "_KW_FISUB", "_KW_FISUBR", "_KW_FLD",
  "_KW_FLD1", "_KW_FLDCW", "_KW_FLDENV", "_KW_FLDL2E", "_KW_FLDL2T",
  "_KW_FLDLG2", "_KW_FLDLN2", "_KW_FLDPI", "_KW_FLDZ", "_KW_FMUL",
  "_KW_FMULP", "_KW_FNCLEX", "_KW_FNDISI", "_KW_FNENI", "_KW_FNINIT",
  "_KW_FNOP", "_KW_FNSAVE", "_KW_FNSTCW", "_KW_FNSTENV", "_KW_FNSTSW",
  "_KW_FORMAT", "_KW_FPATAN", "_KW_FPREM", "_KW_FPREM1", "_KW_FPTAN",
  "_KW_FRNDINT", "_KW_FRSTOR", "_KW_FSAVE", "_KW_FSCALE", "_KW_FSETPM",
  "_KW_FSIN", "_KW_FSINCOS", "_KW_FSQRT", "_KW_FST", "_KW_FSTCW",
  "_KW_FSTENV", "_KW_FSTP", "_KW_FSTSW", "_KW_FSUB", "_KW_FSUBP",
  "_KW_FSUBR", "_KW_FSUBRP", "_KW_FTST", "_KW_FUCOM", "_KW_FUCOMP",
  "_KW_FUCOMPP", "_KW_FXAM", "_KW_FXCH", "_KW_FXTRACT", "_KW_FYL2X",
  "_KW_FYL2XP1", "_KW_GLOBAL", "_KW_HLT", "_KW_IDIV", "_KW_IMUL", "_KW_IN",
  "_KW_INC", "_KW_INCO", "_KW_INSB", "_KW_INSD", "_KW_INSTRSET",
  "_KW_INSW", "_KW_INT", "_KW_INT3", "_KW_INTO", "_KW_INVD", "_KW_INVLPG",
  "_KW_IRET", "_KW_IRETD", "_KW_IRETW", "_KW_JA", "_KW_JAE", "_KW_JB",
  "_KW_JBE", "_KW_JC", "_KW_JCXZ", "_KW_JE", "_KW_JECXZ", "_KW_JG",
  "_KW_JGE", "_KW_JL", "_KW_JLE", "_KW_JMP", "_KW_JNA", "_KW_JNAE",
  "_KW_JNB", "_KW_JNBE", "_KW_JNC", "_KW_JNE", "_KW_JNG", "_KW_JNGE",
  "_KW_JNL", "_KW_JNLE", "_KW_JNO", "_KW_JNP", "_KW_JNS", "_KW_JNZ",
  "_KW_JO", "_KW_JP", "_KW_JPE", "_KW_JPO", "_KW_JS", "_KW_JZ", "_KW_LAHF",
  "_KW_LAR", "_KW_LDS", "_KW_LEA", "_KW_LEAVE", "_KW_LES", "_KW_LFS",
  "_KW_LGDT", "_KW_LGS", "_KW_LIDT", "_KW_LLDT", "_KW_LMSW", "_KW_LOCK",
  "_KW_LODSB", "_KW_LODSD", "_KW_LODSW", "_KW_LOOP", "_KW_LOOPE",
  "_KW_LOOPNE", "_KW_LOOPNZ", "_KW_LOOPZ", "_KW_LSL", "_KW_LSS", "_KW_LTR",
  "_KW_MOV", "_KW_MOVSB", "_KW_MOVSD", "_KW_MOVSW", "_KW_MOVSX",
  "_KW_MOVZX", "_KW_MUL", "_KW_NEAR", "_KW_NEG", "_KW_NOP", "_KW_NOT",
  "_KW_OPTIMIZE", "_KW_OR", "_KW_ORG", "_KW_OUT", "_KW_OUTSB", "_KW_OUTSD",
  "_KW_OUTSW", "_KW_PADDING", "_KW_PADSET", "_KW_POP", "_KW_POPA",
  "_KW_POPAD", "_KW_POPAW", "_KW_POPF", "_KW_POPFD", "_KW_POPFW",
  "_KW_PUSH", "_KW_PUSHA", "_KW_PUSHAD", "_KW_PUSHAW", "_KW_PUSHD",
  "_KW_PUSHF", "_KW_PUSHFD", "_KW_PUSHFW", "_KW_RCL", "_KW_RCR",
  "_KW_RDMSR", "_KW_RDPMC", "_KW_REP", "_KW_REPE", "_KW_REPNE",
  "_KW_REPNZ", "_KW_REPZ", "_KW_RESB", "_KW_RESD", "_KW_RESQ", "_KW_REST",
  "_KW_RESW", "_KW_RET", "_KW_RETF", "_KW_RETN", "_KW_ROL", "_KW_ROR",
  "_KW_RSM", "_KW_SAHF", "_KW_SAL", "_KW_SAR", "_KW_SBB", "_KW_SCASB",
  "_KW_SCASD", "_KW_SCASW", "_KW_SECTION", "_KW_SETA", "_KW_SETAE",
  "_KW_SETB", "_KW_SETBE", "_KW_SETC", "_KW_SETE", "_KW_SETG", "_KW_SETGE",
  "_KW_SETL", "_KW_SETLE", "_KW_SETNA", "_KW_SETNAE", "_KW_SETNB",
  "_KW_SETNBE", "_KW_SETNC", "_KW_SETNE", "_KW_SETNG", "_KW_SETNGE",
  "_KW_SETNL", "_KW_SETNLE", "_KW_SETNO", "_KW_SETNP", "_KW_SETNS",
  "_KW_SETNZ", "_KW_SETO", "_KW_SETP", "_KW_SETPE", "_KW_SETPO",
  "_KW_SETS", "_KW_SETZ", "_KW_SGDT", "_KW_SHL", "_KW_SHLD", "_KW_SHORT",
  "_KW_SHR", "_KW_SHRD", "_KW_SIDT", "_KW_SLDT", "_KW_SMSW", "_KW_STC",
  "_KW_STD", "_KW_STI", "_KW_STOSB", "_KW_STOSD", "_KW_STOSW", "_KW_STR",
  "_KW_SUB", "_KW_TEST", "_KW_TIMES", "_KW_UD2", "_KW_VERR", "_KW_VERW",
  "_KW_WAIT", "_KW_WBINVD", "_KW_WORD", "_KW_WRMSR", "_KW_XADD",
  "_KW_XCHG", "_KW_XLATB", "_KW_XOR", "_LBRACK", "_RBRACK", "T_Hex",
  "T_Id", "T_Label", "T_NaskChar", "_STRING_", "_INTEGER_", "$accept",
  "Program", "ListStatement", "Statement", "ListFactor",
  "ListMnemonicArgs", "MnemonicArgs", "Exp", "Exp1", "Exp2", "MemoryAddr",
  "IndexExp", "Factor", "JumpDir", "ConfigType", "DataType", "Opcode", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  NaskParser::yyrline_[] =
  {
       0,   427,   427,   429,   430,   432,   433,   434,   435,   436,
     437,   439,   440,   441,   443,   444,   445,   447,   449,   450,
     451,   452,   453,   454,   455,   457,   458,   459,   460,   462,
     463,   465,   466,   467,   468,   469,   470,   472,   474,   475,
     476,   477,   478,   480,   481,   482,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   494,   495,   496,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   543,   544,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   614,   615,   616,   617,   618,   619,
     620,   621,   622,   623,   624,   625,   626,   627,   628,   629,
     630,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,   644,   645,   646,   647,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,   691,   692,   693,   694,   695,   696,   697,   698,   699,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,   723,   724,   725,   726,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816
  };

  void
  NaskParser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  NaskParser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  NaskParser::symbol_kind_type
  NaskParser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const short
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357
    };
    // Last valid token kind.
    const int code_max = 612;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // nask_
#line 4216 "parser.cc"

#line 819 "nask.yy"



void nask_::NaskParser::error(const nask_::NaskParser::location_type& l, const std::string& m)
{
    driver.error(*scanner.loc, m);
}


