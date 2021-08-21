/* Bison宣言部 */
%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.5.1"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
# include <string>
# include "nask_ast.hh"
    class driver;
 }

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.hh"
}

%define api.token.prefix {TOK_}
%token
END  0    "end of file"
PLUS      "+"
MINUS     "-"
BANG      "!"
STAR      "*"
SLASH     "/"
COMMA     ","
COLON     ":"
SEMICOLON ";"
SHARP     "#"
LTE       "<="
GTE       ">="
LT        "<"
GT        ">"
LPAREN    "("
RPAREN    ")"
LBRACE    "{"
RBRACE    "}"
LBRACKET  "["
RBRACKET  "]"
STR_LIT   "\""
DOLLAR    "$"
;

//%printer { yyo << $$; } <*>;

%union {
    std::string* str;
    struct ast *a;
    struct symbol *sym;
}

/* 終端記号 */
%token <str> STRING
%token <str> ASSIGN
%token <str> CONFIG
%token <sym> IDENT
/* 非終端記号 */
%type <a> exp
%type <a> stmt stmts
%type <a> program

%%
/* 文法規則部 */
program
: stmts { $$ = newprogram($1); root = $$; }
;
stmts
: stmt
| stmt stmts { $$ = newast(NODE_STMTS, NULL,  $1, $2); }
;
stmt
: declare
| config
| label
| mnemonic
;
//declare
//: IDENT ASSIGN exp { $$ = newassign($1, $3); }
//;
//config
//: LBRACKET CONFIG STRING RBRACKET { $$ = newconfig($2, $3); }
//;
//label
//: IDENT COLON { $$ = newlabel($1); }
//;
//mnemonic
//:
//;
// unit: assignments exp  { drv.result = $2; };
//
// assignments:
// %empty                 {}
// | assignments assignment {};
//
// assignment:
// "identifier" ":=" exp { drv.variables[$1] = $3; };
//
// %left "+" "-";
// %left "*" "/";
exp:
"number"
// | "identifier"  { $$ = drv.variables[$1]; }
// | exp "+" exp   { $$ = $1 + $3; }
// | exp "-" exp   { $$ = $1 - $3; }
// | exp "*" exp   { $$ = $1 * $3; }
// | exp "/" exp   { $$ = $1 / $3; }
// | "("   exp ")"   { $$ = $2; }
%%

void yy::parser::error (const location_type& l, const std::string& m) {
    std::cerr << l << ": " << m << '\n';
}
