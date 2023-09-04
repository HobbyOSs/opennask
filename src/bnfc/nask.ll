/* -*- c++ -*- File generated by the BNF Converter (bnfc 2.9.5). */

/* Lexer definition for use with FLex */

%option nodefault noyywrap c++

/* Additional data for the lexer: a buffer for lexing string literals. */
%option extra-type="Buffer"

%top{
#include <memory>
}
%{
#include "scanner.hh"

/* using "token" to make the returns for the tokens shorter to type */
using token = nask_::NaskParser::token;


#include "absyn.hh"
#include "bison.hh"

#define initialize_lexer nask__initialize_lexer

/* BEGIN extensible string buffer */

#include "buffer.hh"

/* The initial size of the buffer to lex string literals. */
#define LITERAL_BUFFER_INITIAL_SIZE 1024

/* The pointer to the literal buffer. */
Buffer literal_buffer = nullptr;

/* Initialize the literal buffer. */
#define LITERAL_BUFFER_CREATE() literal_buffer = newBuffer(LITERAL_BUFFER_INITIAL_SIZE)

/* Append characters at the end of the buffer. */
#define LITERAL_BUFFER_APPEND(s) bufferAppendString(literal_buffer, s)

/* Append a character at the end of the buffer. */
#define LITERAL_BUFFER_APPEND_CHAR(c) bufferAppendChar(literal_buffer, c)

/* Release the buffer, returning a pointer to its content. */
#define LITERAL_BUFFER_HARVEST() releaseBuffer(literal_buffer)

/* In exceptional cases, e.g. when reaching EOF, we have to free the buffer. */
#define LITERAL_BUFFER_FREE() freeBuffer(literal_buffer)

/* END extensible string buffer */

/* update location on matching */
#define YY_USER_ACTION \
loc->begin.line   = loc->end.line; \
loc->begin.column = loc->end.column; \
for(int i = 0; yytext[i] != '\0'; i++) { \
    if(yytext[i] == '\n') { \
        loc->end.line++; \
        loc->end.column = 0; \
    } \
    else { \
        loc->end.column++; \
    } \
}
%}

LETTER [a-zA-Z]
CAPITAL [A-Z]
SMALL [a-z]
DIGIT [0-9]
IDENT [a-zA-Z0-9'_]
%START CHAR CHARESC CHAREND STRING ESCAPED COMMENT COMMENT1

%%  /* Rules. */

<INITIAL>"["      	 return token::_LBRACK;
<INITIAL>"]"      	 return token::_RBRACK;
<INITIAL>","      	 return token::_COMMA;
<INITIAL>"+"      	 return token::_PLUS;
<INITIAL>"-"      	 return token::_MINUS;
<INITIAL>"*"      	 return token::_STAR;
<INITIAL>"/"      	 return token::_SLASH;
<INITIAL>"%"      	 return token::_PERCENT;
<INITIAL>"("      	 return token::_LPAREN;
<INITIAL>")"      	 return token::_RPAREN;
<INITIAL>":"      	 return token::_COLON;
<INITIAL>"AAA"      	 return token::_KW_AAA;
<INITIAL>"AAD"      	 return token::_KW_AAD;
<INITIAL>"AAM"      	 return token::_KW_AAM;
<INITIAL>"AAS"      	 return token::_KW_AAS;
<INITIAL>"ABSOLUTE"      	 return token::_KW_ABSOLUTE;
<INITIAL>"ADC"      	 return token::_KW_ADC;
<INITIAL>"ADD"      	 return token::_KW_ADD;
<INITIAL>"ALIGN"      	 return token::_KW_ALIGN;
<INITIAL>"ALIGNB"      	 return token::_KW_ALIGNB;
<INITIAL>"AND"      	 return token::_KW_AND;
<INITIAL>"ARPL"      	 return token::_KW_ARPL;
<INITIAL>"BITS"      	 return token::_KW_BITS;
<INITIAL>"BOUND"      	 return token::_KW_BOUND;
<INITIAL>"BSF"      	 return token::_KW_BSF;
<INITIAL>"BSR"      	 return token::_KW_BSR;
<INITIAL>"BSWAP"      	 return token::_KW_BSWAP;
<INITIAL>"BT"      	 return token::_KW_BT;
<INITIAL>"BTC"      	 return token::_KW_BTC;
<INITIAL>"BTR"      	 return token::_KW_BTR;
<INITIAL>"BTS"      	 return token::_KW_BTS;
<INITIAL>"BYTE"      	 return token::_KW_BYTE;
<INITIAL>"CALL"      	 return token::_KW_CALL;
<INITIAL>"CBW"      	 return token::_KW_CBW;
<INITIAL>"CDQ"      	 return token::_KW_CDQ;
<INITIAL>"CLC"      	 return token::_KW_CLC;
<INITIAL>"CLD"      	 return token::_KW_CLD;
<INITIAL>"CLI"      	 return token::_KW_CLI;
<INITIAL>"CLTS"      	 return token::_KW_CLTS;
<INITIAL>"CMC"      	 return token::_KW_CMC;
<INITIAL>"CMP"      	 return token::_KW_CMP;
<INITIAL>"CMPSB"      	 return token::_KW_CMPSB;
<INITIAL>"CMPSD"      	 return token::_KW_CMPSD;
<INITIAL>"CMPSW"      	 return token::_KW_CMPSW;
<INITIAL>"CMPXCHG"      	 return token::_KW_CMPXCHG;
<INITIAL>"CPUID"      	 return token::_KW_CPUID;
<INITIAL>"CWD"      	 return token::_KW_CWD;
<INITIAL>"CWDE"      	 return token::_KW_CWDE;
<INITIAL>"DAA"      	 return token::_KW_DAA;
<INITIAL>"DAS"      	 return token::_KW_DAS;
<INITIAL>"DB"      	 return token::_KW_DB;
<INITIAL>"DD"      	 return token::_KW_DD;
<INITIAL>"DEC"      	 return token::_KW_DEC;
<INITIAL>"DIV"      	 return token::_KW_DIV;
<INITIAL>"DQ"      	 return token::_KW_DQ;
<INITIAL>"DT"      	 return token::_KW_DT;
<INITIAL>"DW"      	 return token::_KW_DW;
<INITIAL>"DWORD"      	 return token::_KW_DWORD;
<INITIAL>"END"      	 return token::_KW_END;
<INITIAL>"ENTER"      	 return token::_KW_ENTER;
<INITIAL>"EQU"      	 return token::_KW_EQU;
<INITIAL>"EXTERN"      	 return token::_KW_EXTERN;
<INITIAL>"F2XM1"      	 return token::_KW_F2XM1;
<INITIAL>"FABS"      	 return token::_KW_FABS;
<INITIAL>"FADD"      	 return token::_KW_FADD;
<INITIAL>"FADDP"      	 return token::_KW_FADDP;
<INITIAL>"FBLD"      	 return token::_KW_FBLD;
<INITIAL>"FBSTP"      	 return token::_KW_FBSTP;
<INITIAL>"FCHS"      	 return token::_KW_FCHS;
<INITIAL>"FCLEX"      	 return token::_KW_FCLEX;
<INITIAL>"FCOM"      	 return token::_KW_FCOM;
<INITIAL>"FCOMP"      	 return token::_KW_FCOMP;
<INITIAL>"FCOMPP"      	 return token::_KW_FCOMPP;
<INITIAL>"FCOS"      	 return token::_KW_FCOS;
<INITIAL>"FDECSTP"      	 return token::_KW_FDECSTP;
<INITIAL>"FDISI"      	 return token::_KW_FDISI;
<INITIAL>"FDIV"      	 return token::_KW_FDIV;
<INITIAL>"FDIVP"      	 return token::_KW_FDIVP;
<INITIAL>"FDIVR"      	 return token::_KW_FDIVR;
<INITIAL>"FDIVRP"      	 return token::_KW_FDIVRP;
<INITIAL>"FENI"      	 return token::_KW_FENI;
<INITIAL>"FFREE"      	 return token::_KW_FFREE;
<INITIAL>"FIADD"      	 return token::_KW_FIADD;
<INITIAL>"FICOM"      	 return token::_KW_FICOM;
<INITIAL>"FICOMP"      	 return token::_KW_FICOMP;
<INITIAL>"FIDIV"      	 return token::_KW_FIDIV;
<INITIAL>"FIDIVR"      	 return token::_KW_FIDIVR;
<INITIAL>"FILD"      	 return token::_KW_FILD;
<INITIAL>"FILE"      	 return token::_KW_FILE;
<INITIAL>"FIMUL"      	 return token::_KW_FIMUL;
<INITIAL>"FINCSTP"      	 return token::_KW_FINCSTP;
<INITIAL>"FINIT"      	 return token::_KW_FINIT;
<INITIAL>"FIST"      	 return token::_KW_FIST;
<INITIAL>"FISTP"      	 return token::_KW_FISTP;
<INITIAL>"FISUB"      	 return token::_KW_FISUB;
<INITIAL>"FISUBR"      	 return token::_KW_FISUBR;
<INITIAL>"FLD"      	 return token::_KW_FLD;
<INITIAL>"FLD1"      	 return token::_KW_FLD1;
<INITIAL>"FLDCW"      	 return token::_KW_FLDCW;
<INITIAL>"FLDENV"      	 return token::_KW_FLDENV;
<INITIAL>"FLDL2E"      	 return token::_KW_FLDL2E;
<INITIAL>"FLDL2T"      	 return token::_KW_FLDL2T;
<INITIAL>"FLDLG2"      	 return token::_KW_FLDLG2;
<INITIAL>"FLDLN2"      	 return token::_KW_FLDLN2;
<INITIAL>"FLDPI"      	 return token::_KW_FLDPI;
<INITIAL>"FLDZ"      	 return token::_KW_FLDZ;
<INITIAL>"FMUL"      	 return token::_KW_FMUL;
<INITIAL>"FMULP"      	 return token::_KW_FMULP;
<INITIAL>"FNCLEX"      	 return token::_KW_FNCLEX;
<INITIAL>"FNDISI"      	 return token::_KW_FNDISI;
<INITIAL>"FNENI"      	 return token::_KW_FNENI;
<INITIAL>"FNINIT"      	 return token::_KW_FNINIT;
<INITIAL>"FNOP"      	 return token::_KW_FNOP;
<INITIAL>"FNSAVE"      	 return token::_KW_FNSAVE;
<INITIAL>"FNSTCW"      	 return token::_KW_FNSTCW;
<INITIAL>"FNSTENV"      	 return token::_KW_FNSTENV;
<INITIAL>"FNSTSW"      	 return token::_KW_FNSTSW;
<INITIAL>"FORMAT"      	 return token::_KW_FORMAT;
<INITIAL>"FPATAN"      	 return token::_KW_FPATAN;
<INITIAL>"FPREM"      	 return token::_KW_FPREM;
<INITIAL>"FPREM1"      	 return token::_KW_FPREM1;
<INITIAL>"FPTAN"      	 return token::_KW_FPTAN;
<INITIAL>"FRNDINT"      	 return token::_KW_FRNDINT;
<INITIAL>"FRSTOR"      	 return token::_KW_FRSTOR;
<INITIAL>"FSAVE"      	 return token::_KW_FSAVE;
<INITIAL>"FSCALE"      	 return token::_KW_FSCALE;
<INITIAL>"FSETPM"      	 return token::_KW_FSETPM;
<INITIAL>"FSIN"      	 return token::_KW_FSIN;
<INITIAL>"FSINCOS"      	 return token::_KW_FSINCOS;
<INITIAL>"FSQRT"      	 return token::_KW_FSQRT;
<INITIAL>"FST"      	 return token::_KW_FST;
<INITIAL>"FSTCW"      	 return token::_KW_FSTCW;
<INITIAL>"FSTENV"      	 return token::_KW_FSTENV;
<INITIAL>"FSTP"      	 return token::_KW_FSTP;
<INITIAL>"FSTSW"      	 return token::_KW_FSTSW;
<INITIAL>"FSUB"      	 return token::_KW_FSUB;
<INITIAL>"FSUBP"      	 return token::_KW_FSUBP;
<INITIAL>"FSUBR"      	 return token::_KW_FSUBR;
<INITIAL>"FSUBRP"      	 return token::_KW_FSUBRP;
<INITIAL>"FTST"      	 return token::_KW_FTST;
<INITIAL>"FUCOM"      	 return token::_KW_FUCOM;
<INITIAL>"FUCOMP"      	 return token::_KW_FUCOMP;
<INITIAL>"FUCOMPP"      	 return token::_KW_FUCOMPP;
<INITIAL>"FXAM"      	 return token::_KW_FXAM;
<INITIAL>"FXCH"      	 return token::_KW_FXCH;
<INITIAL>"FXTRACT"      	 return token::_KW_FXTRACT;
<INITIAL>"FYL2X"      	 return token::_KW_FYL2X;
<INITIAL>"FYL2XP1"      	 return token::_KW_FYL2XP1;
<INITIAL>"GLOBAL"      	 return token::_KW_GLOBAL;
<INITIAL>"HLT"      	 return token::_KW_HLT;
<INITIAL>"IDIV"      	 return token::_KW_IDIV;
<INITIAL>"IMUL"      	 return token::_KW_IMUL;
<INITIAL>"IN"      	 return token::_KW_IN;
<INITIAL>"INC"      	 return token::_KW_INC;
<INITIAL>"INCO"      	 return token::_KW_INCO;
<INITIAL>"INSB"      	 return token::_KW_INSB;
<INITIAL>"INSD"      	 return token::_KW_INSD;
<INITIAL>"INSTRSET"      	 return token::_KW_INSTRSET;
<INITIAL>"INSW"      	 return token::_KW_INSW;
<INITIAL>"INT"      	 return token::_KW_INT;
<INITIAL>"INT3"      	 return token::_KW_INT3;
<INITIAL>"INTO"      	 return token::_KW_INTO;
<INITIAL>"INVD"      	 return token::_KW_INVD;
<INITIAL>"INVLPG"      	 return token::_KW_INVLPG;
<INITIAL>"IRET"      	 return token::_KW_IRET;
<INITIAL>"IRETD"      	 return token::_KW_IRETD;
<INITIAL>"IRETW"      	 return token::_KW_IRETW;
<INITIAL>"JA"      	 return token::_KW_JA;
<INITIAL>"JAE"      	 return token::_KW_JAE;
<INITIAL>"JB"      	 return token::_KW_JB;
<INITIAL>"JBE"      	 return token::_KW_JBE;
<INITIAL>"JC"      	 return token::_KW_JC;
<INITIAL>"JCXZ"      	 return token::_KW_JCXZ;
<INITIAL>"JE"      	 return token::_KW_JE;
<INITIAL>"JECXZ"      	 return token::_KW_JECXZ;
<INITIAL>"JG"      	 return token::_KW_JG;
<INITIAL>"JGE"      	 return token::_KW_JGE;
<INITIAL>"JL"      	 return token::_KW_JL;
<INITIAL>"JLE"      	 return token::_KW_JLE;
<INITIAL>"JMP"      	 return token::_KW_JMP;
<INITIAL>"JNA"      	 return token::_KW_JNA;
<INITIAL>"JNAE"      	 return token::_KW_JNAE;
<INITIAL>"JNB"      	 return token::_KW_JNB;
<INITIAL>"JNBE"      	 return token::_KW_JNBE;
<INITIAL>"JNC"      	 return token::_KW_JNC;
<INITIAL>"JNE"      	 return token::_KW_JNE;
<INITIAL>"JNG"      	 return token::_KW_JNG;
<INITIAL>"JNGE"      	 return token::_KW_JNGE;
<INITIAL>"JNL"      	 return token::_KW_JNL;
<INITIAL>"JNLE"      	 return token::_KW_JNLE;
<INITIAL>"JNO"      	 return token::_KW_JNO;
<INITIAL>"JNP"      	 return token::_KW_JNP;
<INITIAL>"JNS"      	 return token::_KW_JNS;
<INITIAL>"JNZ"      	 return token::_KW_JNZ;
<INITIAL>"JO"      	 return token::_KW_JO;
<INITIAL>"JP"      	 return token::_KW_JP;
<INITIAL>"JPE"      	 return token::_KW_JPE;
<INITIAL>"JPO"      	 return token::_KW_JPO;
<INITIAL>"JS"      	 return token::_KW_JS;
<INITIAL>"JZ"      	 return token::_KW_JZ;
<INITIAL>"LAHF"      	 return token::_KW_LAHF;
<INITIAL>"LAR"      	 return token::_KW_LAR;
<INITIAL>"LDS"      	 return token::_KW_LDS;
<INITIAL>"LEA"      	 return token::_KW_LEA;
<INITIAL>"LEAVE"      	 return token::_KW_LEAVE;
<INITIAL>"LES"      	 return token::_KW_LES;
<INITIAL>"LFS"      	 return token::_KW_LFS;
<INITIAL>"LGDT"      	 return token::_KW_LGDT;
<INITIAL>"LGS"      	 return token::_KW_LGS;
<INITIAL>"LIDT"      	 return token::_KW_LIDT;
<INITIAL>"LLDT"      	 return token::_KW_LLDT;
<INITIAL>"LMSW"      	 return token::_KW_LMSW;
<INITIAL>"LOCK"      	 return token::_KW_LOCK;
<INITIAL>"LODSB"      	 return token::_KW_LODSB;
<INITIAL>"LODSD"      	 return token::_KW_LODSD;
<INITIAL>"LODSW"      	 return token::_KW_LODSW;
<INITIAL>"LOOP"      	 return token::_KW_LOOP;
<INITIAL>"LOOPE"      	 return token::_KW_LOOPE;
<INITIAL>"LOOPNE"      	 return token::_KW_LOOPNE;
<INITIAL>"LOOPNZ"      	 return token::_KW_LOOPNZ;
<INITIAL>"LOOPZ"      	 return token::_KW_LOOPZ;
<INITIAL>"LSL"      	 return token::_KW_LSL;
<INITIAL>"LSS"      	 return token::_KW_LSS;
<INITIAL>"LTR"      	 return token::_KW_LTR;
<INITIAL>"MOV"      	 return token::_KW_MOV;
<INITIAL>"MOVSB"      	 return token::_KW_MOVSB;
<INITIAL>"MOVSD"      	 return token::_KW_MOVSD;
<INITIAL>"MOVSW"      	 return token::_KW_MOVSW;
<INITIAL>"MOVSX"      	 return token::_KW_MOVSX;
<INITIAL>"MOVZX"      	 return token::_KW_MOVZX;
<INITIAL>"MUL"      	 return token::_KW_MUL;
<INITIAL>"NEG"      	 return token::_KW_NEG;
<INITIAL>"NOP"      	 return token::_KW_NOP;
<INITIAL>"NOT"      	 return token::_KW_NOT;
<INITIAL>"OPTIMIZE"      	 return token::_KW_OPTIMIZE;
<INITIAL>"OR"      	 return token::_KW_OR;
<INITIAL>"ORG"      	 return token::_KW_ORG;
<INITIAL>"OUT"      	 return token::_KW_OUT;
<INITIAL>"OUTSB"      	 return token::_KW_OUTSB;
<INITIAL>"OUTSD"      	 return token::_KW_OUTSD;
<INITIAL>"OUTSW"      	 return token::_KW_OUTSW;
<INITIAL>"PADDING"      	 return token::_KW_PADDING;
<INITIAL>"PADSET"      	 return token::_KW_PADSET;
<INITIAL>"POP"      	 return token::_KW_POP;
<INITIAL>"POPA"      	 return token::_KW_POPA;
<INITIAL>"POPAD"      	 return token::_KW_POPAD;
<INITIAL>"POPAW"      	 return token::_KW_POPAW;
<INITIAL>"POPF"      	 return token::_KW_POPF;
<INITIAL>"POPFD"      	 return token::_KW_POPFD;
<INITIAL>"POPFW"      	 return token::_KW_POPFW;
<INITIAL>"PUSH"      	 return token::_KW_PUSH;
<INITIAL>"PUSHA"      	 return token::_KW_PUSHA;
<INITIAL>"PUSHAD"      	 return token::_KW_PUSHAD;
<INITIAL>"PUSHAW"      	 return token::_KW_PUSHAW;
<INITIAL>"PUSHD"      	 return token::_KW_PUSHD;
<INITIAL>"PUSHF"      	 return token::_KW_PUSHF;
<INITIAL>"PUSHFD"      	 return token::_KW_PUSHFD;
<INITIAL>"PUSHFW"      	 return token::_KW_PUSHFW;
<INITIAL>"RCL"      	 return token::_KW_RCL;
<INITIAL>"RCR"      	 return token::_KW_RCR;
<INITIAL>"RDMSR"      	 return token::_KW_RDMSR;
<INITIAL>"RDPMC"      	 return token::_KW_RDPMC;
<INITIAL>"REP"      	 return token::_KW_REP;
<INITIAL>"REPE"      	 return token::_KW_REPE;
<INITIAL>"REPNE"      	 return token::_KW_REPNE;
<INITIAL>"REPNZ"      	 return token::_KW_REPNZ;
<INITIAL>"REPZ"      	 return token::_KW_REPZ;
<INITIAL>"RESB"      	 return token::_KW_RESB;
<INITIAL>"RESD"      	 return token::_KW_RESD;
<INITIAL>"RESQ"      	 return token::_KW_RESQ;
<INITIAL>"REST"      	 return token::_KW_REST;
<INITIAL>"RESW"      	 return token::_KW_RESW;
<INITIAL>"RET"      	 return token::_KW_RET;
<INITIAL>"RETF"      	 return token::_KW_RETF;
<INITIAL>"RETN"      	 return token::_KW_RETN;
<INITIAL>"ROL"      	 return token::_KW_ROL;
<INITIAL>"ROR"      	 return token::_KW_ROR;
<INITIAL>"RSM"      	 return token::_KW_RSM;
<INITIAL>"SAHF"      	 return token::_KW_SAHF;
<INITIAL>"SAL"      	 return token::_KW_SAL;
<INITIAL>"SAR"      	 return token::_KW_SAR;
<INITIAL>"SBB"      	 return token::_KW_SBB;
<INITIAL>"SCASB"      	 return token::_KW_SCASB;
<INITIAL>"SCASD"      	 return token::_KW_SCASD;
<INITIAL>"SCASW"      	 return token::_KW_SCASW;
<INITIAL>"SECTION"      	 return token::_KW_SECTION;
<INITIAL>"SETA"      	 return token::_KW_SETA;
<INITIAL>"SETAE"      	 return token::_KW_SETAE;
<INITIAL>"SETB"      	 return token::_KW_SETB;
<INITIAL>"SETBE"      	 return token::_KW_SETBE;
<INITIAL>"SETC"      	 return token::_KW_SETC;
<INITIAL>"SETE"      	 return token::_KW_SETE;
<INITIAL>"SETG"      	 return token::_KW_SETG;
<INITIAL>"SETGE"      	 return token::_KW_SETGE;
<INITIAL>"SETL"      	 return token::_KW_SETL;
<INITIAL>"SETLE"      	 return token::_KW_SETLE;
<INITIAL>"SETNA"      	 return token::_KW_SETNA;
<INITIAL>"SETNAE"      	 return token::_KW_SETNAE;
<INITIAL>"SETNB"      	 return token::_KW_SETNB;
<INITIAL>"SETNBE"      	 return token::_KW_SETNBE;
<INITIAL>"SETNC"      	 return token::_KW_SETNC;
<INITIAL>"SETNE"      	 return token::_KW_SETNE;
<INITIAL>"SETNG"      	 return token::_KW_SETNG;
<INITIAL>"SETNGE"      	 return token::_KW_SETNGE;
<INITIAL>"SETNL"      	 return token::_KW_SETNL;
<INITIAL>"SETNLE"      	 return token::_KW_SETNLE;
<INITIAL>"SETNO"      	 return token::_KW_SETNO;
<INITIAL>"SETNP"      	 return token::_KW_SETNP;
<INITIAL>"SETNS"      	 return token::_KW_SETNS;
<INITIAL>"SETNZ"      	 return token::_KW_SETNZ;
<INITIAL>"SETO"      	 return token::_KW_SETO;
<INITIAL>"SETP"      	 return token::_KW_SETP;
<INITIAL>"SETPE"      	 return token::_KW_SETPE;
<INITIAL>"SETPO"      	 return token::_KW_SETPO;
<INITIAL>"SETS"      	 return token::_KW_SETS;
<INITIAL>"SETZ"      	 return token::_KW_SETZ;
<INITIAL>"SGDT"      	 return token::_KW_SGDT;
<INITIAL>"SHL"      	 return token::_KW_SHL;
<INITIAL>"SHLD"      	 return token::_KW_SHLD;
<INITIAL>"SHR"      	 return token::_KW_SHR;
<INITIAL>"SHRD"      	 return token::_KW_SHRD;
<INITIAL>"SIDT"      	 return token::_KW_SIDT;
<INITIAL>"SLDT"      	 return token::_KW_SLDT;
<INITIAL>"SMSW"      	 return token::_KW_SMSW;
<INITIAL>"STC"      	 return token::_KW_STC;
<INITIAL>"STD"      	 return token::_KW_STD;
<INITIAL>"STI"      	 return token::_KW_STI;
<INITIAL>"STOSB"      	 return token::_KW_STOSB;
<INITIAL>"STOSD"      	 return token::_KW_STOSD;
<INITIAL>"STOSW"      	 return token::_KW_STOSW;
<INITIAL>"STR"      	 return token::_KW_STR;
<INITIAL>"SUB"      	 return token::_KW_SUB;
<INITIAL>"TEST"      	 return token::_KW_TEST;
<INITIAL>"TIMES"      	 return token::_KW_TIMES;
<INITIAL>"UD2"      	 return token::_KW_UD2;
<INITIAL>"VERR"      	 return token::_KW_VERR;
<INITIAL>"VERW"      	 return token::_KW_VERW;
<INITIAL>"WAIT"      	 return token::_KW_WAIT;
<INITIAL>"WBINVD"      	 return token::_KW_WBINVD;
<INITIAL>"WORD"      	 return token::_KW_WORD;
<INITIAL>"WRMSR"      	 return token::_KW_WRMSR;
<INITIAL>"XADD"      	 return token::_KW_XADD;
<INITIAL>"XCHG"      	 return token::_KW_XCHG;
<INITIAL>"XLATB"      	 return token::_KW_XLATB;
<INITIAL>"XOR"      	 return token::_KW_XOR;

<INITIAL>"#" BEGIN COMMENT; /* BNFC: block comment "#" "\\n" */
<COMMENT>\n BEGIN INITIAL;
<COMMENT>[^\n] /* skip */;
<INITIAL>";" BEGIN COMMENT1; /* BNFC: block comment ";" "\\n" */
<COMMENT1>\n BEGIN INITIAL;
<COMMENT1>[^\n] /* skip */;

<INITIAL>0(X|x)(A|B|C|D|E|F|a|b|c|d|e|f|{DIGIT})+\-?\$?    	 yylval->emplace<std::string>(yytext); return token::T_Hex;
<INITIAL>(\$|\.|\_|{LETTER})(\$|\.|\_|({DIGIT}|{LETTER}))*\:(\\|n|r|t)*    	 yylval->emplace<std::string>(yytext); return token::T_Label;
<INITIAL>(\$|\.|\_|{LETTER})(\$|\.|\_|({DIGIT}|{LETTER}))*    	 yylval->emplace<std::string>(yytext); return token::T_Id;
<INITIAL>"\""        	 LITERAL_BUFFER_CREATE(); BEGIN STRING;
<STRING>\\             	 BEGIN ESCAPED;
<STRING>\"             	 yylval->emplace<std::string>(LITERAL_BUFFER_HARVEST()); BEGIN INITIAL; return token::_STRING_;
<STRING>.              	 LITERAL_BUFFER_APPEND_CHAR(yytext[0]);
<ESCAPED>f             	 LITERAL_BUFFER_APPEND_CHAR('\f'); BEGIN STRING;
<ESCAPED>n             	 LITERAL_BUFFER_APPEND_CHAR('\n'); BEGIN STRING;
<ESCAPED>r             	 LITERAL_BUFFER_APPEND_CHAR('\r'); BEGIN STRING;
<ESCAPED>t             	 LITERAL_BUFFER_APPEND_CHAR('\t'); BEGIN STRING;
<ESCAPED>\"            	 LITERAL_BUFFER_APPEND_CHAR('"');  BEGIN STRING;
<ESCAPED>\\            	 LITERAL_BUFFER_APPEND_CHAR('\\'); BEGIN STRING;
<ESCAPED>.             	 LITERAL_BUFFER_APPEND(yytext);    BEGIN STRING;
<STRING,ESCAPED><<EOF>>	 LITERAL_BUFFER_FREE(); return token::_ERROR_;
<INITIAL>{DIGIT}+      	 yylval->emplace<int>(atoi(yytext)); return token::_INTEGER_;
<INITIAL>[ \t\r\n\f]      	 /* ignore white space. */;
<INITIAL>.      	 return token::_ERROR_;

%%  /* Initialization code. */


NaskScanner::NaskScanner(std::istream *in)
    : yyFlexLexer(in)
{
    loc = new nask_::NaskParser::location_type();
}

NaskScanner::~NaskScanner()
{
    delete loc;
    delete yylval;
}

/* This implementation of NaskFlexLexer::yylex() is required to fill the
 * vtable of the class NaskFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */



#ifdef yylex
#undef yylex
#endif

int yyFlexLexer::yylex()
{
    std::cerr << "in yyFlexLexer::yylex() !" << std::endl;
    return 0;
}


