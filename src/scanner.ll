/** 定義、初期Cコード */

%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hh"
# include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

/** 定義、初期Cコード */
%%
 /* ルール */

%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+              loc.step ();
\n+                   loc.lines (yyleng); loc.step ();
"EQU"                 { return yylval.str = yytext; return ASSIGN; }
"BITS"                |
"INSTRSET"            |
"OPTIMIZE"            |
"FORMAT"              |
"PADDING"             |
"PADSET"              |
"OPTION"              |
"SECTION"             |
"ABSOLUTE"            |
"FILE"                { return yylval.str = yytext; return CONFIG; }
[a-zA-Z][a-zA-Z0-9_]* { yylval.sym = lookup(yytext); return IDENT; }
\"(\\\\.|[^\"])*\"    { yylval.str = yytext; return STRING; }
"+"                   return yy::parser::make_PLUS(loc);
"-"                   return yy::parser::make_MINUS(loc);
"!"                   return yy::parser::make_BANG(loc);
"*"                   return yy::parser::make_STAR(loc);
"/"                   return yy::parser::make_SLASH(loc);
","                   return yy::parser::make_COMMA(loc);
":"                   return yy::parser::make_COLON(loc);
";".*                 /* Ignore comments */ ;
"#".*                 /* Ignore comments */ ;
"<="                  return yy::parser::make_LTE(loc);
">="                  return yy::parser::make_GTE(loc);
"<"                   return yy::parser::make_LT(loc);
">"                   return yy::parser::make_GT(loc);
"("                   return yy::parser::make_LPAREN(loc);
")"                   return yy::parser::make_RPAREN(loc);
"{"                   return yy::parser::make_LBRACE(loc);
"}"                   return yy::parser::make_RBRACE(loc);
"["                   return yy::parser::make_LBRACKET(loc);
"]"                   return yy::parser::make_RBRACKET(loc);
"$"                   return yy::parser::make_DOLLAR(loc);
. {
    throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
}
<<EOF>>    return yy::parser::make_END (loc);

 /* ルール */
%%


void driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if (file.empty () || file == "-") {
        yyin = stdin;
    } else if (!(yyin = fopen (file.c_str (), "r"))) {
        std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
        exit (EXIT_FAILURE);
    }
}

void driver::scan_end() {
    fclose (yyin);
}
