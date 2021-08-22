/* Bison宣言部 */
%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.5.1"
%defines

// unionを使わない設定
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

/* 終端記号 */
%token NEWLINE
%token <std::string> IDENT  "ident"
%token <std::string> NUMBER "number"
%token <std::string> CMP    "cmp"
%token <std::string> CONST_STRING "const_string"
%token <std::string> ASSIGN
%token <std::string> CONFIG
/* 非終端記号 */
%type <ast::ExpNode> exp
%type <std::list<ast::Statement>> stmts
%type <ast::Statement> stmt
%type <ast::Program> program

%%
/* 文法規則部 */
program
: stmts { $$ = ast::Program($1); }
;
stmts
: stmts stmt NEWLINE { $$ = $1; $1.push_back($2); }
| { $$ = std::list<ast::Statement>(); }
;
stmt
: IDENT COLON { $$ = ast::LabelStmt($1); }
| IDENT ASSIGN exp { $$ = ast::DeclareStmt($<std::string>1, $3); }
| LBRACKET CONFIG CONST_STRING RBRACKET { $$ = ast::ConfigStmt($<std::string>2, $<std::string>3); }
;
exp: factor CMP factor { $$ = ast::CmpNode( $<std::string>1, $2,  $<std::string>3); }
| factor '+' factor { $$ = ast::CalcNode($<std::string>1, "+", $<std::string>3); }
| factor '-' factor { $$ = ast::CalcNode($<std::string>1, "-", $<std::string>3); }
| factor '*' factor { $$ = ast::CalcNode($<std::string>1, "*", $<std::string>3); }
| factor '/' factor { $$ = ast::CalcNode($<std::string>1, "/", $<std::string>3); }
| factor '%' factor { $$ = ast::CalcNode($<std::string>1, "%", $<std::string>3); }
;
factor:
| "number"
| "ident"
| "const_string"
;
%%

void yy::parser::error (const location_type& l, const std::string& m) {
    std::cerr << l << ": " << m << '\n';
}
