/** 定義、初期Cコード */
%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring> // strerror
#include <string>
#include "driver.hh"
#include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

opcode     AAA|AAD|AAS|AAM|ADC|ADD|AND|ALIGN|ALIGNB|ARPL|BOUND|BSF|BSR|BSWAP|BT|BTC|BTR|BTS|CALL|CBW|CDQ|CLC|CLD|CLI|CLTS|CMC|CMP|CMPSB|CMPSD|CMPSW|CMPXCHG|CPUID|CWD|CWDE|DAA|DAS|DB|DD|DEC|DIV|DQ|DT|DW|END|ENTER|EXTERN|F2XM1|FABS|FADD|FADDP|FBLD|FBSTP|FCHS|FCLEX|FCOM|FCOMP|FCOMPP|FCOS|FDECSTP|FDISI|FDIV|FDIVP|FDIVR|FDIVRP|FENI|FFREE|FIADD|FICOM|FICOMP|FIDIV|FIDIVR|FILD|FIMUL|FINCSTP|FINIT|FIST|FISTP|FISUB|FISUBR|FLD|FLD1|FLDCW|FLDENV|FLDL2E|FLDL2T|FLDLG2|FLDLN2|FLDPI|FLDZ|FMUL|FMULP|FNCLEX|FNDISI|FNENI|FNINIT|FNOP|FNSAVE|FNSTCW|FNSTENV|FNSTSW|FPATAN|FPTAN|FPREM|FPREM1|FRNDINT|FRSTOR|FSAVE|FSCALE|FSETPM|FSIN|FSINCOS|FSQRT|FST|FSTCW|FSTENV|FSTP|FSTSW|FSUB|FSUBP|FSUBR|FSUBRP|FTST|FUCOM|FUCOMP|FUCOMPP|FXAM|FXCH|FXTRACT|FYL2X|FYL2XP1|HLT|IDIV|IMUL|IN|INC|INCO|INSB|INSD|INSW|INT|INT3|INTO|INVD|INVLPG|IRET|IRETD|IRETW|JA|JAE|JB|JBE|JC|JCXZ|JE|JECXZ|JG|JGE|JL|JLE|JMP|JNA|JNAE|JNB|JNBE|JNC|JNE|JNG|JNGE|JNL|JNLE|JNO|JNP|JNS|JNZ|JO|JP|JPE|JPO|JS|JZ|LAHF|LAR|LDS|LEA|LEAVE|LES|LFS|LGDT|LGS|LIDT|LLDT|LMSW|LOCK|LODSB|LODSD|LODSW|LOOP|LOOPE|LOOPNE|LOOPNZ|LOOPZ|LSL|LSS|LTR|MOV|MOVSB|MOVSD|MOVSW|MOVSX|MOVZX|MUL|NEG|NOP|NOT|OR|ORG|OUT|OUTSB|OUTSD|OUTSW|POP|POPA|POPAD|POPAW|POPF|POPFD|POPFW|PUSH|PUSHA|PUSHD|PUSHAD|PUSHAW|PUSHF|PUSHFD|PUSHFW|RCL|RCR|RDMSR|RDPMC|REP|REPE|REPNE|REPNZ|REPZ|RESB|RESD|RESQ|REST|RESW|RET|RETF|RETN|ROL|ROR|RSM|SAHF|SAL|SAR|SBB|SCASB|SCASD|SCASW|SETA|SETAE|SETB|SETBE|SETC|SETE|SETG|SETGE|SETL|SETLE|SETNA|SETNAE|SETNB|SETNBE|SETNC|SETNE|SETNG|SETNGE|SETNL|SETNLE|SETNO|SETNP|SETNS|SETNZ|SETO|SETP|SETPE|SETPO|SETS|SETZ|SGDT|SHL|SHLD|SHR|SHRD|SIDT|SLDT|SMSW|STC|STD|STI|STOSB|STOSD|STOSW|STR|SUB|TEST|TIMES|UD2|VERR|VERW|WAIT|WBINVD|WRMSR|XADD|XCHG|XLATB|XOR
id         [a-zA-Z][a-zA-Z_0-9$]*
hex        0[xX][0-9a-fA-F]+-{0,1}${0,1}
int        [0-9]+
blank      [ \t]
cmp        [==|!=|>|<|>=|<=]
str        [\"(([^\"]|\\\")*[^\\])?\"]
config     BITS|INSTRSET|OPTIMIZE|FORMAT|PADDING|PADSET|SECTION|ABSOLUTE|FILE

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

{blank}*                  loc.step();
(\n|\r\n)                 { loc.lines(yyleng); loc.step(); }
";"[^\n]*?\n              { loc.lines(yyleng); loc.step(); } /* ignore one line comment */
"#"[^\n]*?\n              { loc.lines(yyleng); loc.step(); } /* ignore one line comment */
\"(([^\"]|\\\")*[^\\])?\" return yy::parser::make_CONST_STRING(yytext, loc);
{config}                  return yy::parser::make_CONFIG(yytext, loc);
{opcode}                  return yy::parser::make_OPCODE(yytext, loc);
{id}                      return yy::parser::make_IDENT(yytext, loc);
{hex}                     return yy::parser::make_NUMBER(yytext, loc);
{int}                     return yy::parser::make_NUMBER(yytext, loc);
{cmp}                     return yy::parser::make_CMP(yytext, loc);
"+"                       return yy::parser::make_PLUS(loc);
"-"                       return yy::parser::make_MINUS(loc);
"!"                       return yy::parser::make_BANG(loc);
"*"                       return yy::parser::make_STAR(loc);
"/"                       return yy::parser::make_SLASH(loc);
","                       return yy::parser::make_COMMA(loc);
":"                       return yy::parser::make_COLON(loc);
"("                       return yy::parser::make_LPAREN(loc);
")"                       return yy::parser::make_RPAREN(loc);
"{"                       return yy::parser::make_LBRACE(loc);
"}"                       return yy::parser::make_RBRACE(loc);
"["                       return yy::parser::make_LBRACKET(loc);
"]"                       return yy::parser::make_RBRACKET(loc);
"$"                       return yy::parser::make_DOLLAR(loc);
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
