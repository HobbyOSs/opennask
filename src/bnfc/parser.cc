/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         nask_parse
#define yylex           nask_lex
#define yyerror         nask_error
#define yydebug         nask_debug
#define yynerrs         nask_nerrs

/* First part of user prologue.  */
#line 18 "nask.y"

/* Begin C preamble code */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "absyn.hh"

#define YYMAXDEPTH 10000000

/* The type yyscan_t is defined by flex, but we need it in the parser already. */
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE nask__scan_string(const char *str, yyscan_t scanner);
extern void nask__delete_buffer(YY_BUFFER_STATE buf, yyscan_t scanner);

extern void nask_lex_destroy(yyscan_t scanner);
extern char* nask_get_text(yyscan_t scanner);

extern yyscan_t nask__initialize_lexer(FILE * inp);

/* End C preamble code */

#line 104 "parser.cc"

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

#include "bison.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL__ERROR_ = 3,                    /* _ERROR_  */
  YYSYMBOL__BANGEQ = 4,                    /* _BANGEQ  */
  YYSYMBOL__PERCENT = 5,                   /* _PERCENT  */
  YYSYMBOL__STAR = 6,                      /* _STAR  */
  YYSYMBOL__PLUS = 7,                      /* _PLUS  */
  YYSYMBOL__COMMA = 8,                     /* _COMMA  */
  YYSYMBOL__MINUS = 9,                     /* _MINUS  */
  YYSYMBOL__SLASH = 10,                    /* _SLASH  */
  YYSYMBOL__COLON = 11,                    /* _COLON  */
  YYSYMBOL__LT = 12,                       /* _LT  */
  YYSYMBOL__LDARROW = 13,                  /* _LDARROW  */
  YYSYMBOL__DEQ = 14,                      /* _DEQ  */
  YYSYMBOL__GT = 15,                       /* _GT  */
  YYSYMBOL__GTEQ = 16,                     /* _GTEQ  */
  YYSYMBOL__KW_AAA = 17,                   /* _KW_AAA  */
  YYSYMBOL__KW_AAD = 18,                   /* _KW_AAD  */
  YYSYMBOL__KW_AAM = 19,                   /* _KW_AAM  */
  YYSYMBOL__KW_AAS = 20,                   /* _KW_AAS  */
  YYSYMBOL__KW_ABSOLUTE = 21,              /* _KW_ABSOLUTE  */
  YYSYMBOL__KW_ADC = 22,                   /* _KW_ADC  */
  YYSYMBOL__KW_ADD = 23,                   /* _KW_ADD  */
  YYSYMBOL__KW_ALIGN = 24,                 /* _KW_ALIGN  */
  YYSYMBOL__KW_ALIGNB = 25,                /* _KW_ALIGNB  */
  YYSYMBOL__KW_AND = 26,                   /* _KW_AND  */
  YYSYMBOL__KW_ARPL = 27,                  /* _KW_ARPL  */
  YYSYMBOL__KW_BITS = 28,                  /* _KW_BITS  */
  YYSYMBOL__KW_BOUND = 29,                 /* _KW_BOUND  */
  YYSYMBOL__KW_BSF = 30,                   /* _KW_BSF  */
  YYSYMBOL__KW_BSR = 31,                   /* _KW_BSR  */
  YYSYMBOL__KW_BSWAP = 32,                 /* _KW_BSWAP  */
  YYSYMBOL__KW_BT = 33,                    /* _KW_BT  */
  YYSYMBOL__KW_BTC = 34,                   /* _KW_BTC  */
  YYSYMBOL__KW_BTR = 35,                   /* _KW_BTR  */
  YYSYMBOL__KW_BTS = 36,                   /* _KW_BTS  */
  YYSYMBOL__KW_CALL = 37,                  /* _KW_CALL  */
  YYSYMBOL__KW_CBW = 38,                   /* _KW_CBW  */
  YYSYMBOL__KW_CDQ = 39,                   /* _KW_CDQ  */
  YYSYMBOL__KW_CLC = 40,                   /* _KW_CLC  */
  YYSYMBOL__KW_CLD = 41,                   /* _KW_CLD  */
  YYSYMBOL__KW_CLI = 42,                   /* _KW_CLI  */
  YYSYMBOL__KW_CLTS = 43,                  /* _KW_CLTS  */
  YYSYMBOL__KW_CMC = 44,                   /* _KW_CMC  */
  YYSYMBOL__KW_CMP = 45,                   /* _KW_CMP  */
  YYSYMBOL__KW_CMPSB = 46,                 /* _KW_CMPSB  */
  YYSYMBOL__KW_CMPSD = 47,                 /* _KW_CMPSD  */
  YYSYMBOL__KW_CMPSW = 48,                 /* _KW_CMPSW  */
  YYSYMBOL__KW_CMPXCHG = 49,               /* _KW_CMPXCHG  */
  YYSYMBOL__KW_CPUID = 50,                 /* _KW_CPUID  */
  YYSYMBOL__KW_CWD = 51,                   /* _KW_CWD  */
  YYSYMBOL__KW_CWDE = 52,                  /* _KW_CWDE  */
  YYSYMBOL__KW_DAA = 53,                   /* _KW_DAA  */
  YYSYMBOL__KW_DAS = 54,                   /* _KW_DAS  */
  YYSYMBOL__KW_DB = 55,                    /* _KW_DB  */
  YYSYMBOL__KW_DD = 56,                    /* _KW_DD  */
  YYSYMBOL__KW_DEC = 57,                   /* _KW_DEC  */
  YYSYMBOL__KW_DIV = 58,                   /* _KW_DIV  */
  YYSYMBOL__KW_DQ = 59,                    /* _KW_DQ  */
  YYSYMBOL__KW_DT = 60,                    /* _KW_DT  */
  YYSYMBOL__KW_DW = 61,                    /* _KW_DW  */
  YYSYMBOL__KW_END = 62,                   /* _KW_END  */
  YYSYMBOL__KW_ENTER = 63,                 /* _KW_ENTER  */
  YYSYMBOL__KW_EQU = 64,                   /* _KW_EQU  */
  YYSYMBOL__KW_EXTERN = 65,                /* _KW_EXTERN  */
  YYSYMBOL__KW_F2XM1 = 66,                 /* _KW_F2XM1  */
  YYSYMBOL__KW_FABS = 67,                  /* _KW_FABS  */
  YYSYMBOL__KW_FADD = 68,                  /* _KW_FADD  */
  YYSYMBOL__KW_FADDP = 69,                 /* _KW_FADDP  */
  YYSYMBOL__KW_FBLD = 70,                  /* _KW_FBLD  */
  YYSYMBOL__KW_FBSTP = 71,                 /* _KW_FBSTP  */
  YYSYMBOL__KW_FCHS = 72,                  /* _KW_FCHS  */
  YYSYMBOL__KW_FCLEX = 73,                 /* _KW_FCLEX  */
  YYSYMBOL__KW_FCOM = 74,                  /* _KW_FCOM  */
  YYSYMBOL__KW_FCOMP = 75,                 /* _KW_FCOMP  */
  YYSYMBOL__KW_FCOMPP = 76,                /* _KW_FCOMPP  */
  YYSYMBOL__KW_FCOS = 77,                  /* _KW_FCOS  */
  YYSYMBOL__KW_FDECSTP = 78,               /* _KW_FDECSTP  */
  YYSYMBOL__KW_FDISI = 79,                 /* _KW_FDISI  */
  YYSYMBOL__KW_FDIV = 80,                  /* _KW_FDIV  */
  YYSYMBOL__KW_FDIVP = 81,                 /* _KW_FDIVP  */
  YYSYMBOL__KW_FDIVR = 82,                 /* _KW_FDIVR  */
  YYSYMBOL__KW_FDIVRP = 83,                /* _KW_FDIVRP  */
  YYSYMBOL__KW_FENI = 84,                  /* _KW_FENI  */
  YYSYMBOL__KW_FFREE = 85,                 /* _KW_FFREE  */
  YYSYMBOL__KW_FIADD = 86,                 /* _KW_FIADD  */
  YYSYMBOL__KW_FICOM = 87,                 /* _KW_FICOM  */
  YYSYMBOL__KW_FICOMP = 88,                /* _KW_FICOMP  */
  YYSYMBOL__KW_FIDIV = 89,                 /* _KW_FIDIV  */
  YYSYMBOL__KW_FIDIVR = 90,                /* _KW_FIDIVR  */
  YYSYMBOL__KW_FILD = 91,                  /* _KW_FILD  */
  YYSYMBOL__KW_FILE = 92,                  /* _KW_FILE  */
  YYSYMBOL__KW_FIMUL = 93,                 /* _KW_FIMUL  */
  YYSYMBOL__KW_FINCSTP = 94,               /* _KW_FINCSTP  */
  YYSYMBOL__KW_FINIT = 95,                 /* _KW_FINIT  */
  YYSYMBOL__KW_FIST = 96,                  /* _KW_FIST  */
  YYSYMBOL__KW_FISTP = 97,                 /* _KW_FISTP  */
  YYSYMBOL__KW_FISUB = 98,                 /* _KW_FISUB  */
  YYSYMBOL__KW_FISUBR = 99,                /* _KW_FISUBR  */
  YYSYMBOL__KW_FLD = 100,                  /* _KW_FLD  */
  YYSYMBOL__KW_FLD1 = 101,                 /* _KW_FLD1  */
  YYSYMBOL__KW_FLDCW = 102,                /* _KW_FLDCW  */
  YYSYMBOL__KW_FLDENV = 103,               /* _KW_FLDENV  */
  YYSYMBOL__KW_FLDL2E = 104,               /* _KW_FLDL2E  */
  YYSYMBOL__KW_FLDL2T = 105,               /* _KW_FLDL2T  */
  YYSYMBOL__KW_FLDLG2 = 106,               /* _KW_FLDLG2  */
  YYSYMBOL__KW_FLDLN2 = 107,               /* _KW_FLDLN2  */
  YYSYMBOL__KW_FLDPI = 108,                /* _KW_FLDPI  */
  YYSYMBOL__KW_FLDZ = 109,                 /* _KW_FLDZ  */
  YYSYMBOL__KW_FMUL = 110,                 /* _KW_FMUL  */
  YYSYMBOL__KW_FMULP = 111,                /* _KW_FMULP  */
  YYSYMBOL__KW_FNCLEX = 112,               /* _KW_FNCLEX  */
  YYSYMBOL__KW_FNDISI = 113,               /* _KW_FNDISI  */
  YYSYMBOL__KW_FNENI = 114,                /* _KW_FNENI  */
  YYSYMBOL__KW_FNINIT = 115,               /* _KW_FNINIT  */
  YYSYMBOL__KW_FNOP = 116,                 /* _KW_FNOP  */
  YYSYMBOL__KW_FNSAVE = 117,               /* _KW_FNSAVE  */
  YYSYMBOL__KW_FNSTCW = 118,               /* _KW_FNSTCW  */
  YYSYMBOL__KW_FNSTENV = 119,              /* _KW_FNSTENV  */
  YYSYMBOL__KW_FNSTSW = 120,               /* _KW_FNSTSW  */
  YYSYMBOL__KW_FORMAT = 121,               /* _KW_FORMAT  */
  YYSYMBOL__KW_FPATAN = 122,               /* _KW_FPATAN  */
  YYSYMBOL__KW_FPREM = 123,                /* _KW_FPREM  */
  YYSYMBOL__KW_FPREM1 = 124,               /* _KW_FPREM1  */
  YYSYMBOL__KW_FPTAN = 125,                /* _KW_FPTAN  */
  YYSYMBOL__KW_FRNDINT = 126,              /* _KW_FRNDINT  */
  YYSYMBOL__KW_FRSTOR = 127,               /* _KW_FRSTOR  */
  YYSYMBOL__KW_FSAVE = 128,                /* _KW_FSAVE  */
  YYSYMBOL__KW_FSCALE = 129,               /* _KW_FSCALE  */
  YYSYMBOL__KW_FSETPM = 130,               /* _KW_FSETPM  */
  YYSYMBOL__KW_FSIN = 131,                 /* _KW_FSIN  */
  YYSYMBOL__KW_FSINCOS = 132,              /* _KW_FSINCOS  */
  YYSYMBOL__KW_FSQRT = 133,                /* _KW_FSQRT  */
  YYSYMBOL__KW_FST = 134,                  /* _KW_FST  */
  YYSYMBOL__KW_FSTCW = 135,                /* _KW_FSTCW  */
  YYSYMBOL__KW_FSTENV = 136,               /* _KW_FSTENV  */
  YYSYMBOL__KW_FSTP = 137,                 /* _KW_FSTP  */
  YYSYMBOL__KW_FSTSW = 138,                /* _KW_FSTSW  */
  YYSYMBOL__KW_FSUB = 139,                 /* _KW_FSUB  */
  YYSYMBOL__KW_FSUBP = 140,                /* _KW_FSUBP  */
  YYSYMBOL__KW_FSUBR = 141,                /* _KW_FSUBR  */
  YYSYMBOL__KW_FSUBRP = 142,               /* _KW_FSUBRP  */
  YYSYMBOL__KW_FTST = 143,                 /* _KW_FTST  */
  YYSYMBOL__KW_FUCOM = 144,                /* _KW_FUCOM  */
  YYSYMBOL__KW_FUCOMP = 145,               /* _KW_FUCOMP  */
  YYSYMBOL__KW_FUCOMPP = 146,              /* _KW_FUCOMPP  */
  YYSYMBOL__KW_FXAM = 147,                 /* _KW_FXAM  */
  YYSYMBOL__KW_FXCH = 148,                 /* _KW_FXCH  */
  YYSYMBOL__KW_FXTRACT = 149,              /* _KW_FXTRACT  */
  YYSYMBOL__KW_FYL2X = 150,                /* _KW_FYL2X  */
  YYSYMBOL__KW_FYL2XP1 = 151,              /* _KW_FYL2XP1  */
  YYSYMBOL__KW_HLT = 152,                  /* _KW_HLT  */
  YYSYMBOL__KW_IDIV = 153,                 /* _KW_IDIV  */
  YYSYMBOL__KW_IMUL = 154,                 /* _KW_IMUL  */
  YYSYMBOL__KW_IN = 155,                   /* _KW_IN  */
  YYSYMBOL__KW_INC = 156,                  /* _KW_INC  */
  YYSYMBOL__KW_INCO = 157,                 /* _KW_INCO  */
  YYSYMBOL__KW_INSB = 158,                 /* _KW_INSB  */
  YYSYMBOL__KW_INSD = 159,                 /* _KW_INSD  */
  YYSYMBOL__KW_INSTRSET = 160,             /* _KW_INSTRSET  */
  YYSYMBOL__KW_INSW = 161,                 /* _KW_INSW  */
  YYSYMBOL__KW_INT = 162,                  /* _KW_INT  */
  YYSYMBOL__KW_INT3 = 163,                 /* _KW_INT3  */
  YYSYMBOL__KW_INTO = 164,                 /* _KW_INTO  */
  YYSYMBOL__KW_INVD = 165,                 /* _KW_INVD  */
  YYSYMBOL__KW_INVLPG = 166,               /* _KW_INVLPG  */
  YYSYMBOL__KW_IRET = 167,                 /* _KW_IRET  */
  YYSYMBOL__KW_IRETD = 168,                /* _KW_IRETD  */
  YYSYMBOL__KW_IRETW = 169,                /* _KW_IRETW  */
  YYSYMBOL__KW_JA = 170,                   /* _KW_JA  */
  YYSYMBOL__KW_JAE = 171,                  /* _KW_JAE  */
  YYSYMBOL__KW_JB = 172,                   /* _KW_JB  */
  YYSYMBOL__KW_JBE = 173,                  /* _KW_JBE  */
  YYSYMBOL__KW_JC = 174,                   /* _KW_JC  */
  YYSYMBOL__KW_JCXZ = 175,                 /* _KW_JCXZ  */
  YYSYMBOL__KW_JE = 176,                   /* _KW_JE  */
  YYSYMBOL__KW_JECXZ = 177,                /* _KW_JECXZ  */
  YYSYMBOL__KW_JG = 178,                   /* _KW_JG  */
  YYSYMBOL__KW_JGE = 179,                  /* _KW_JGE  */
  YYSYMBOL__KW_JL = 180,                   /* _KW_JL  */
  YYSYMBOL__KW_JLE = 181,                  /* _KW_JLE  */
  YYSYMBOL__KW_JMP = 182,                  /* _KW_JMP  */
  YYSYMBOL__KW_JNA = 183,                  /* _KW_JNA  */
  YYSYMBOL__KW_JNAE = 184,                 /* _KW_JNAE  */
  YYSYMBOL__KW_JNB = 185,                  /* _KW_JNB  */
  YYSYMBOL__KW_JNBE = 186,                 /* _KW_JNBE  */
  YYSYMBOL__KW_JNC = 187,                  /* _KW_JNC  */
  YYSYMBOL__KW_JNE = 188,                  /* _KW_JNE  */
  YYSYMBOL__KW_JNG = 189,                  /* _KW_JNG  */
  YYSYMBOL__KW_JNGE = 190,                 /* _KW_JNGE  */
  YYSYMBOL__KW_JNL = 191,                  /* _KW_JNL  */
  YYSYMBOL__KW_JNLE = 192,                 /* _KW_JNLE  */
  YYSYMBOL__KW_JNO = 193,                  /* _KW_JNO  */
  YYSYMBOL__KW_JNP = 194,                  /* _KW_JNP  */
  YYSYMBOL__KW_JNS = 195,                  /* _KW_JNS  */
  YYSYMBOL__KW_JNZ = 196,                  /* _KW_JNZ  */
  YYSYMBOL__KW_JO = 197,                   /* _KW_JO  */
  YYSYMBOL__KW_JP = 198,                   /* _KW_JP  */
  YYSYMBOL__KW_JPE = 199,                  /* _KW_JPE  */
  YYSYMBOL__KW_JPO = 200,                  /* _KW_JPO  */
  YYSYMBOL__KW_JS = 201,                   /* _KW_JS  */
  YYSYMBOL__KW_JZ = 202,                   /* _KW_JZ  */
  YYSYMBOL__KW_LAHF = 203,                 /* _KW_LAHF  */
  YYSYMBOL__KW_LAR = 204,                  /* _KW_LAR  */
  YYSYMBOL__KW_LDS = 205,                  /* _KW_LDS  */
  YYSYMBOL__KW_LEA = 206,                  /* _KW_LEA  */
  YYSYMBOL__KW_LEAVE = 207,                /* _KW_LEAVE  */
  YYSYMBOL__KW_LES = 208,                  /* _KW_LES  */
  YYSYMBOL__KW_LFS = 209,                  /* _KW_LFS  */
  YYSYMBOL__KW_LGDT = 210,                 /* _KW_LGDT  */
  YYSYMBOL__KW_LGS = 211,                  /* _KW_LGS  */
  YYSYMBOL__KW_LIDT = 212,                 /* _KW_LIDT  */
  YYSYMBOL__KW_LLDT = 213,                 /* _KW_LLDT  */
  YYSYMBOL__KW_LMSW = 214,                 /* _KW_LMSW  */
  YYSYMBOL__KW_LOCK = 215,                 /* _KW_LOCK  */
  YYSYMBOL__KW_LODSB = 216,                /* _KW_LODSB  */
  YYSYMBOL__KW_LODSD = 217,                /* _KW_LODSD  */
  YYSYMBOL__KW_LODSW = 218,                /* _KW_LODSW  */
  YYSYMBOL__KW_LOOP = 219,                 /* _KW_LOOP  */
  YYSYMBOL__KW_LOOPE = 220,                /* _KW_LOOPE  */
  YYSYMBOL__KW_LOOPNE = 221,               /* _KW_LOOPNE  */
  YYSYMBOL__KW_LOOPNZ = 222,               /* _KW_LOOPNZ  */
  YYSYMBOL__KW_LOOPZ = 223,                /* _KW_LOOPZ  */
  YYSYMBOL__KW_LSL = 224,                  /* _KW_LSL  */
  YYSYMBOL__KW_LSS = 225,                  /* _KW_LSS  */
  YYSYMBOL__KW_LTR = 226,                  /* _KW_LTR  */
  YYSYMBOL__KW_MOV = 227,                  /* _KW_MOV  */
  YYSYMBOL__KW_MOVSB = 228,                /* _KW_MOVSB  */
  YYSYMBOL__KW_MOVSD = 229,                /* _KW_MOVSD  */
  YYSYMBOL__KW_MOVSW = 230,                /* _KW_MOVSW  */
  YYSYMBOL__KW_MOVSX = 231,                /* _KW_MOVSX  */
  YYSYMBOL__KW_MOVZX = 232,                /* _KW_MOVZX  */
  YYSYMBOL__KW_MUL = 233,                  /* _KW_MUL  */
  YYSYMBOL__KW_NEG = 234,                  /* _KW_NEG  */
  YYSYMBOL__KW_NOP = 235,                  /* _KW_NOP  */
  YYSYMBOL__KW_NOT = 236,                  /* _KW_NOT  */
  YYSYMBOL__KW_OPTIMIZE = 237,             /* _KW_OPTIMIZE  */
  YYSYMBOL__KW_OR = 238,                   /* _KW_OR  */
  YYSYMBOL__KW_ORG = 239,                  /* _KW_ORG  */
  YYSYMBOL__KW_OUT = 240,                  /* _KW_OUT  */
  YYSYMBOL__KW_OUTSB = 241,                /* _KW_OUTSB  */
  YYSYMBOL__KW_OUTSD = 242,                /* _KW_OUTSD  */
  YYSYMBOL__KW_OUTSW = 243,                /* _KW_OUTSW  */
  YYSYMBOL__KW_PADDING = 244,              /* _KW_PADDING  */
  YYSYMBOL__KW_PADSET = 245,               /* _KW_PADSET  */
  YYSYMBOL__KW_POP = 246,                  /* _KW_POP  */
  YYSYMBOL__KW_POPA = 247,                 /* _KW_POPA  */
  YYSYMBOL__KW_POPAD = 248,                /* _KW_POPAD  */
  YYSYMBOL__KW_POPAW = 249,                /* _KW_POPAW  */
  YYSYMBOL__KW_POPF = 250,                 /* _KW_POPF  */
  YYSYMBOL__KW_POPFD = 251,                /* _KW_POPFD  */
  YYSYMBOL__KW_POPFW = 252,                /* _KW_POPFW  */
  YYSYMBOL__KW_PUSH = 253,                 /* _KW_PUSH  */
  YYSYMBOL__KW_PUSHA = 254,                /* _KW_PUSHA  */
  YYSYMBOL__KW_PUSHAD = 255,               /* _KW_PUSHAD  */
  YYSYMBOL__KW_PUSHAW = 256,               /* _KW_PUSHAW  */
  YYSYMBOL__KW_PUSHD = 257,                /* _KW_PUSHD  */
  YYSYMBOL__KW_PUSHF = 258,                /* _KW_PUSHF  */
  YYSYMBOL__KW_PUSHFD = 259,               /* _KW_PUSHFD  */
  YYSYMBOL__KW_PUSHFW = 260,               /* _KW_PUSHFW  */
  YYSYMBOL__KW_RCL = 261,                  /* _KW_RCL  */
  YYSYMBOL__KW_RCR = 262,                  /* _KW_RCR  */
  YYSYMBOL__KW_RDMSR = 263,                /* _KW_RDMSR  */
  YYSYMBOL__KW_RDPMC = 264,                /* _KW_RDPMC  */
  YYSYMBOL__KW_REP = 265,                  /* _KW_REP  */
  YYSYMBOL__KW_REPE = 266,                 /* _KW_REPE  */
  YYSYMBOL__KW_REPNE = 267,                /* _KW_REPNE  */
  YYSYMBOL__KW_REPNZ = 268,                /* _KW_REPNZ  */
  YYSYMBOL__KW_REPZ = 269,                 /* _KW_REPZ  */
  YYSYMBOL__KW_RESB = 270,                 /* _KW_RESB  */
  YYSYMBOL__KW_RESD = 271,                 /* _KW_RESD  */
  YYSYMBOL__KW_RESQ = 272,                 /* _KW_RESQ  */
  YYSYMBOL__KW_REST = 273,                 /* _KW_REST  */
  YYSYMBOL__KW_RESW = 274,                 /* _KW_RESW  */
  YYSYMBOL__KW_RET = 275,                  /* _KW_RET  */
  YYSYMBOL__KW_RETF = 276,                 /* _KW_RETF  */
  YYSYMBOL__KW_RETN = 277,                 /* _KW_RETN  */
  YYSYMBOL__KW_ROL = 278,                  /* _KW_ROL  */
  YYSYMBOL__KW_ROR = 279,                  /* _KW_ROR  */
  YYSYMBOL__KW_RSM = 280,                  /* _KW_RSM  */
  YYSYMBOL__KW_SAHF = 281,                 /* _KW_SAHF  */
  YYSYMBOL__KW_SAL = 282,                  /* _KW_SAL  */
  YYSYMBOL__KW_SAR = 283,                  /* _KW_SAR  */
  YYSYMBOL__KW_SBB = 284,                  /* _KW_SBB  */
  YYSYMBOL__KW_SCASB = 285,                /* _KW_SCASB  */
  YYSYMBOL__KW_SCASD = 286,                /* _KW_SCASD  */
  YYSYMBOL__KW_SCASW = 287,                /* _KW_SCASW  */
  YYSYMBOL__KW_SECTION = 288,              /* _KW_SECTION  */
  YYSYMBOL__KW_SETA = 289,                 /* _KW_SETA  */
  YYSYMBOL__KW_SETAE = 290,                /* _KW_SETAE  */
  YYSYMBOL__KW_SETB = 291,                 /* _KW_SETB  */
  YYSYMBOL__KW_SETBE = 292,                /* _KW_SETBE  */
  YYSYMBOL__KW_SETC = 293,                 /* _KW_SETC  */
  YYSYMBOL__KW_SETE = 294,                 /* _KW_SETE  */
  YYSYMBOL__KW_SETG = 295,                 /* _KW_SETG  */
  YYSYMBOL__KW_SETGE = 296,                /* _KW_SETGE  */
  YYSYMBOL__KW_SETL = 297,                 /* _KW_SETL  */
  YYSYMBOL__KW_SETLE = 298,                /* _KW_SETLE  */
  YYSYMBOL__KW_SETNA = 299,                /* _KW_SETNA  */
  YYSYMBOL__KW_SETNAE = 300,               /* _KW_SETNAE  */
  YYSYMBOL__KW_SETNB = 301,                /* _KW_SETNB  */
  YYSYMBOL__KW_SETNBE = 302,               /* _KW_SETNBE  */
  YYSYMBOL__KW_SETNC = 303,                /* _KW_SETNC  */
  YYSYMBOL__KW_SETNE = 304,                /* _KW_SETNE  */
  YYSYMBOL__KW_SETNG = 305,                /* _KW_SETNG  */
  YYSYMBOL__KW_SETNGE = 306,               /* _KW_SETNGE  */
  YYSYMBOL__KW_SETNL = 307,                /* _KW_SETNL  */
  YYSYMBOL__KW_SETNLE = 308,               /* _KW_SETNLE  */
  YYSYMBOL__KW_SETNO = 309,                /* _KW_SETNO  */
  YYSYMBOL__KW_SETNP = 310,                /* _KW_SETNP  */
  YYSYMBOL__KW_SETNS = 311,                /* _KW_SETNS  */
  YYSYMBOL__KW_SETNZ = 312,                /* _KW_SETNZ  */
  YYSYMBOL__KW_SETO = 313,                 /* _KW_SETO  */
  YYSYMBOL__KW_SETP = 314,                 /* _KW_SETP  */
  YYSYMBOL__KW_SETPE = 315,                /* _KW_SETPE  */
  YYSYMBOL__KW_SETPO = 316,                /* _KW_SETPO  */
  YYSYMBOL__KW_SETS = 317,                 /* _KW_SETS  */
  YYSYMBOL__KW_SETZ = 318,                 /* _KW_SETZ  */
  YYSYMBOL__KW_SGDT = 319,                 /* _KW_SGDT  */
  YYSYMBOL__KW_SHL = 320,                  /* _KW_SHL  */
  YYSYMBOL__KW_SHLD = 321,                 /* _KW_SHLD  */
  YYSYMBOL__KW_SHR = 322,                  /* _KW_SHR  */
  YYSYMBOL__KW_SHRD = 323,                 /* _KW_SHRD  */
  YYSYMBOL__KW_SIDT = 324,                 /* _KW_SIDT  */
  YYSYMBOL__KW_SLDT = 325,                 /* _KW_SLDT  */
  YYSYMBOL__KW_SMSW = 326,                 /* _KW_SMSW  */
  YYSYMBOL__KW_STC = 327,                  /* _KW_STC  */
  YYSYMBOL__KW_STD = 328,                  /* _KW_STD  */
  YYSYMBOL__KW_STI = 329,                  /* _KW_STI  */
  YYSYMBOL__KW_STOSB = 330,                /* _KW_STOSB  */
  YYSYMBOL__KW_STOSD = 331,                /* _KW_STOSD  */
  YYSYMBOL__KW_STOSW = 332,                /* _KW_STOSW  */
  YYSYMBOL__KW_STR = 333,                  /* _KW_STR  */
  YYSYMBOL__KW_SUB = 334,                  /* _KW_SUB  */
  YYSYMBOL__KW_TEST = 335,                 /* _KW_TEST  */
  YYSYMBOL__KW_TIMES = 336,                /* _KW_TIMES  */
  YYSYMBOL__KW_UD2 = 337,                  /* _KW_UD2  */
  YYSYMBOL__KW_VERR = 338,                 /* _KW_VERR  */
  YYSYMBOL__KW_VERW = 339,                 /* _KW_VERW  */
  YYSYMBOL__KW_WAIT = 340,                 /* _KW_WAIT  */
  YYSYMBOL__KW_WBINVD = 341,               /* _KW_WBINVD  */
  YYSYMBOL__KW_WRMSR = 342,                /* _KW_WRMSR  */
  YYSYMBOL__KW_XADD = 343,                 /* _KW_XADD  */
  YYSYMBOL__KW_XCHG = 344,                 /* _KW_XCHG  */
  YYSYMBOL__KW_XLATB = 345,                /* _KW_XLATB  */
  YYSYMBOL__KW_XOR = 346,                  /* _KW_XOR  */
  YYSYMBOL__LBRACK = 347,                  /* _LBRACK  */
  YYSYMBOL__RBRACK = 348,                  /* _RBRACK  */
  YYSYMBOL_T_Hex = 349,                    /* T_Hex  */
  YYSYMBOL__STRING_ = 350,                 /* _STRING_  */
  YYSYMBOL__INTEGER_ = 351,                /* _INTEGER_  */
  YYSYMBOL__IDENT_ = 352,                  /* _IDENT_  */
  YYSYMBOL_YYACCEPT = 353,                 /* $accept  */
  YYSYMBOL_Program = 354,                  /* Program  */
  YYSYMBOL_ListStatement = 355,            /* ListStatement  */
  YYSYMBOL_Statement = 356,                /* Statement  */
  YYSYMBOL_ListMnemonicArgs = 357,         /* ListMnemonicArgs  */
  YYSYMBOL_MnemonicArgs = 358,             /* MnemonicArgs  */
  YYSYMBOL_Exp = 359,                      /* Exp  */
  YYSYMBOL_Factor = 360,                   /* Factor  */
  YYSYMBOL_ConfigType = 361,               /* ConfigType  */
  YYSYMBOL_Opcode = 362                    /* Opcode  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 63 "nask.y"

void yyerror(YYLTYPE *loc, yyscan_t scanner, YYSTYPE *result, const char *msg)
{
  fprintf(stderr, "error: %d,%d: %s at %s\n",
    loc->first_line, loc->first_column, msg, nask_get_text(scanner));
}

int yyparse(yyscan_t scanner, YYSTYPE *result);

extern int yylex(YYSTYPE *lvalp, YYLTYPE *llocp, yyscan_t scanner);

#line 512 "parser.cc"


#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  339
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   598

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  353
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  356
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  376

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   607


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int16 yytranslate[] =
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
     345,   346,   347,   348,   349,   350,   351,   352
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   440,   440,   442,   443,   445,   446,   447,   448,   450,
     451,   453,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   468,   469,   470,   471,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,   581,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   599,   600,   601,   602,   603,   604,   605,
     606,   607,   608,   609,   610,   611,   612,   613,   614,   615,
     616,   617,   618,   619,   620,   621,   622,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,   691,   692,   693,   694,   695,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,   710,   711,   712,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,   723,   724,   725,
     726,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,   760,   761,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,   801,   802
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "_ERROR_", "_BANGEQ",
  "_PERCENT", "_STAR", "_PLUS", "_COMMA", "_MINUS", "_SLASH", "_COLON",
  "_LT", "_LDARROW", "_DEQ", "_GT", "_GTEQ", "_KW_AAA", "_KW_AAD",
  "_KW_AAM", "_KW_AAS", "_KW_ABSOLUTE", "_KW_ADC", "_KW_ADD", "_KW_ALIGN",
  "_KW_ALIGNB", "_KW_AND", "_KW_ARPL", "_KW_BITS", "_KW_BOUND", "_KW_BSF",
  "_KW_BSR", "_KW_BSWAP", "_KW_BT", "_KW_BTC", "_KW_BTR", "_KW_BTS",
  "_KW_CALL", "_KW_CBW", "_KW_CDQ", "_KW_CLC", "_KW_CLD", "_KW_CLI",
  "_KW_CLTS", "_KW_CMC", "_KW_CMP", "_KW_CMPSB", "_KW_CMPSD", "_KW_CMPSW",
  "_KW_CMPXCHG", "_KW_CPUID", "_KW_CWD", "_KW_CWDE", "_KW_DAA", "_KW_DAS",
  "_KW_DB", "_KW_DD", "_KW_DEC", "_KW_DIV", "_KW_DQ", "_KW_DT", "_KW_DW",
  "_KW_END", "_KW_ENTER", "_KW_EQU", "_KW_EXTERN", "_KW_F2XM1", "_KW_FABS",
  "_KW_FADD", "_KW_FADDP", "_KW_FBLD", "_KW_FBSTP", "_KW_FCHS",
  "_KW_FCLEX", "_KW_FCOM", "_KW_FCOMP", "_KW_FCOMPP", "_KW_FCOS",
  "_KW_FDECSTP", "_KW_FDISI", "_KW_FDIV", "_KW_FDIVP", "_KW_FDIVR",
  "_KW_FDIVRP", "_KW_FENI", "_KW_FFREE", "_KW_FIADD", "_KW_FICOM",
  "_KW_FICOMP", "_KW_FIDIV", "_KW_FIDIVR", "_KW_FILD", "_KW_FILE",
  "_KW_FIMUL", "_KW_FINCSTP", "_KW_FINIT", "_KW_FIST", "_KW_FISTP",
  "_KW_FISUB", "_KW_FISUBR", "_KW_FLD", "_KW_FLD1", "_KW_FLDCW",
  "_KW_FLDENV", "_KW_FLDL2E", "_KW_FLDL2T", "_KW_FLDLG2", "_KW_FLDLN2",
  "_KW_FLDPI", "_KW_FLDZ", "_KW_FMUL", "_KW_FMULP", "_KW_FNCLEX",
  "_KW_FNDISI", "_KW_FNENI", "_KW_FNINIT", "_KW_FNOP", "_KW_FNSAVE",
  "_KW_FNSTCW", "_KW_FNSTENV", "_KW_FNSTSW", "_KW_FORMAT", "_KW_FPATAN",
  "_KW_FPREM", "_KW_FPREM1", "_KW_FPTAN", "_KW_FRNDINT", "_KW_FRSTOR",
  "_KW_FSAVE", "_KW_FSCALE", "_KW_FSETPM", "_KW_FSIN", "_KW_FSINCOS",
  "_KW_FSQRT", "_KW_FST", "_KW_FSTCW", "_KW_FSTENV", "_KW_FSTP",
  "_KW_FSTSW", "_KW_FSUB", "_KW_FSUBP", "_KW_FSUBR", "_KW_FSUBRP",
  "_KW_FTST", "_KW_FUCOM", "_KW_FUCOMP", "_KW_FUCOMPP", "_KW_FXAM",
  "_KW_FXCH", "_KW_FXTRACT", "_KW_FYL2X", "_KW_FYL2XP1", "_KW_HLT",
  "_KW_IDIV", "_KW_IMUL", "_KW_IN", "_KW_INC", "_KW_INCO", "_KW_INSB",
  "_KW_INSD", "_KW_INSTRSET", "_KW_INSW", "_KW_INT", "_KW_INT3",
  "_KW_INTO", "_KW_INVD", "_KW_INVLPG", "_KW_IRET", "_KW_IRETD",
  "_KW_IRETW", "_KW_JA", "_KW_JAE", "_KW_JB", "_KW_JBE", "_KW_JC",
  "_KW_JCXZ", "_KW_JE", "_KW_JECXZ", "_KW_JG", "_KW_JGE", "_KW_JL",
  "_KW_JLE", "_KW_JMP", "_KW_JNA", "_KW_JNAE", "_KW_JNB", "_KW_JNBE",
  "_KW_JNC", "_KW_JNE", "_KW_JNG", "_KW_JNGE", "_KW_JNL", "_KW_JNLE",
  "_KW_JNO", "_KW_JNP", "_KW_JNS", "_KW_JNZ", "_KW_JO", "_KW_JP",
  "_KW_JPE", "_KW_JPO", "_KW_JS", "_KW_JZ", "_KW_LAHF", "_KW_LAR",
  "_KW_LDS", "_KW_LEA", "_KW_LEAVE", "_KW_LES", "_KW_LFS", "_KW_LGDT",
  "_KW_LGS", "_KW_LIDT", "_KW_LLDT", "_KW_LMSW", "_KW_LOCK", "_KW_LODSB",
  "_KW_LODSD", "_KW_LODSW", "_KW_LOOP", "_KW_LOOPE", "_KW_LOOPNE",
  "_KW_LOOPNZ", "_KW_LOOPZ", "_KW_LSL", "_KW_LSS", "_KW_LTR", "_KW_MOV",
  "_KW_MOVSB", "_KW_MOVSD", "_KW_MOVSW", "_KW_MOVSX", "_KW_MOVZX",
  "_KW_MUL", "_KW_NEG", "_KW_NOP", "_KW_NOT", "_KW_OPTIMIZE", "_KW_OR",
  "_KW_ORG", "_KW_OUT", "_KW_OUTSB", "_KW_OUTSD", "_KW_OUTSW",
  "_KW_PADDING", "_KW_PADSET", "_KW_POP", "_KW_POPA", "_KW_POPAD",
  "_KW_POPAW", "_KW_POPF", "_KW_POPFD", "_KW_POPFW", "_KW_PUSH",
  "_KW_PUSHA", "_KW_PUSHAD", "_KW_PUSHAW", "_KW_PUSHD", "_KW_PUSHF",
  "_KW_PUSHFD", "_KW_PUSHFW", "_KW_RCL", "_KW_RCR", "_KW_RDMSR",
  "_KW_RDPMC", "_KW_REP", "_KW_REPE", "_KW_REPNE", "_KW_REPNZ", "_KW_REPZ",
  "_KW_RESB", "_KW_RESD", "_KW_RESQ", "_KW_REST", "_KW_RESW", "_KW_RET",
  "_KW_RETF", "_KW_RETN", "_KW_ROL", "_KW_ROR", "_KW_RSM", "_KW_SAHF",
  "_KW_SAL", "_KW_SAR", "_KW_SBB", "_KW_SCASB", "_KW_SCASD", "_KW_SCASW",
  "_KW_SECTION", "_KW_SETA", "_KW_SETAE", "_KW_SETB", "_KW_SETBE",
  "_KW_SETC", "_KW_SETE", "_KW_SETG", "_KW_SETGE", "_KW_SETL", "_KW_SETLE",
  "_KW_SETNA", "_KW_SETNAE", "_KW_SETNB", "_KW_SETNBE", "_KW_SETNC",
  "_KW_SETNE", "_KW_SETNG", "_KW_SETNGE", "_KW_SETNL", "_KW_SETNLE",
  "_KW_SETNO", "_KW_SETNP", "_KW_SETNS", "_KW_SETNZ", "_KW_SETO",
  "_KW_SETP", "_KW_SETPE", "_KW_SETPO", "_KW_SETS", "_KW_SETZ", "_KW_SGDT",
  "_KW_SHL", "_KW_SHLD", "_KW_SHR", "_KW_SHRD", "_KW_SIDT", "_KW_SLDT",
  "_KW_SMSW", "_KW_STC", "_KW_STD", "_KW_STI", "_KW_STOSB", "_KW_STOSD",
  "_KW_STOSW", "_KW_STR", "_KW_SUB", "_KW_TEST", "_KW_TIMES", "_KW_UD2",
  "_KW_VERR", "_KW_VERW", "_KW_WAIT", "_KW_WBINVD", "_KW_WRMSR",
  "_KW_XADD", "_KW_XCHG", "_KW_XLATB", "_KW_XOR", "_LBRACK", "_RBRACK",
  "T_Hex", "_STRING_", "_INTEGER_", "_IDENT_", "$accept", "Program",
  "ListStatement", "Statement", "ListMnemonicArgs", "MnemonicArgs", "Exp",
  "Factor", "ConfigType", "Opcode", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607
};
#endif

#define YYPACT_NINF (-340)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -17,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,   310,   321,     4,  -340,   -17,    14,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -339,  -340,    14,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,    39,  -340,   335,  -273,
    -340,    14,    14,    14,    14,    14,    14,    14,    14,    14,
      14,    14,    14,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,    37,    38,    40,    39,    41,    42,    44,    45,    43,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   139,   140,   138,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     267,   268,   266,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,     0,     0,     0,     2,     3,     0,    35,    28,    36,
      31,    29,    30,    32,    33,    34,     0,     5,     0,     1,
       4,    25,    27,    24,    26,     8,     9,    11,    23,     0,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,    10,    13,    22,    20,    18,    19,
      21,    14,    16,    12,    15,    17
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -340,  -340,  -221,  -340,  -208,  -340,  -118,     0,  -340,  -340
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,   323,   324,   325,   345,   346,   347,   348,   336,   326
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       1,     2,     3,     4,   339,     5,     6,     7,     8,     9,
      10,   349,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,   351,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,   363,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   340,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   364,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     350,   215,   216,   217,   218,   219,   220,     0,     0,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   327,   337,     0,     0,   322,     0,     0,   328,   352,
     353,   354,   355,     0,   356,   357,     0,   358,   359,   360,
     361,   362,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   341,   342,   343,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   329,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   330,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,   333,   334,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   335
};

static const yytype_int16 yycheck[] =
{
      17,    18,    19,    20,     0,    22,    23,    24,    25,    26,
      27,   350,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,     8,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,   348,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   325,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   351,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     338,   238,   239,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,    -1,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,    21,    11,    -1,    -1,   352,    -1,    -1,    28,     4,
       5,     6,     7,    -1,     9,    10,    -1,    12,    13,    14,
      15,    16,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   349,   350,   351,   352,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   237,    -1,    -1,
      -1,    -1,    -1,    -1,   244,   245,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    17,    18,    19,    20,    22,    23,    24,    25,    26,
      27,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   238,   239,   240,   241,   242,
     243,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   352,   354,   355,   356,   362,    21,    28,    92,
     121,   160,   237,   244,   245,   288,   361,    11,    64,     0,
     355,   349,   350,   351,   352,   357,   358,   359,   360,   350,
     359,     8,     4,     5,     6,     7,     9,    10,    12,    13,
      14,    15,    16,   348,   357,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   353,   354,   355,   355,   356,   356,   356,   356,   357,
     357,   358,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   360,   360,   360,   360,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     3,     4,     2,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
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


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, scanner, result, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, scanner, result); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, YYSTYPE *result)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (scanner);
  YY_USE (result);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, YYSTYPE *result)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, scanner, result);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, yyscan_t scanner, YYSTYPE *result)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), scanner, result);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, result); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner, YYSTYPE *result)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (scanner);
  YY_USE (result);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner, YYSTYPE *result)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: ListStatement  */
#line 440 "nask.y"
                        { std::reverse((yyvsp[0].liststatement_)->begin(),(yyvsp[0].liststatement_)->end()) ;(yyval.program_) = new Prog((yyvsp[0].liststatement_)); result->program_ = (yyval.program_); }
#line 2038 "parser.cc"
    break;

  case 3: /* ListStatement: Statement  */
#line 442 "nask.y"
                          { (yyval.liststatement_) = new ListStatement(); (yyval.liststatement_)->push_back((yyvsp[0].statement_)); result->liststatement_ = (yyval.liststatement_); }
#line 2044 "parser.cc"
    break;

  case 4: /* ListStatement: Statement ListStatement  */
#line 443 "nask.y"
                            { (yyvsp[0].liststatement_)->push_back((yyvsp[-1].statement_)); (yyval.liststatement_) = (yyvsp[0].liststatement_); result->liststatement_ = (yyval.liststatement_); }
#line 2050 "parser.cc"
    break;

  case 5: /* Statement: _IDENT_ _COLON  */
#line 445 "nask.y"
                           { (yyval.statement_) = new LabelStmt((yyvsp[-1]._string)); result->statement_ = (yyval.statement_); }
#line 2056 "parser.cc"
    break;

  case 6: /* Statement: _IDENT_ _KW_EQU Exp  */
#line 446 "nask.y"
                        { (yyval.statement_) = new DeclareStmt((yyvsp[-2]._string), (yyvsp[0].exp_)); result->statement_ = (yyval.statement_); }
#line 2062 "parser.cc"
    break;

  case 7: /* Statement: _LBRACK ConfigType _STRING_ _RBRACK  */
#line 447 "nask.y"
                                        { (yyval.statement_) = new ConfigStmt((yyvsp[-2].configtype_), (yyvsp[-1]._string)); result->statement_ = (yyval.statement_); }
#line 2068 "parser.cc"
    break;

  case 8: /* Statement: Opcode ListMnemonicArgs  */
#line 448 "nask.y"
                            { std::reverse((yyvsp[0].listmnemonicargs_)->begin(),(yyvsp[0].listmnemonicargs_)->end()) ;(yyval.statement_) = new MnemonicStmt((yyvsp[-1].opcode_), (yyvsp[0].listmnemonicargs_)); result->statement_ = (yyval.statement_); }
#line 2074 "parser.cc"
    break;

  case 9: /* ListMnemonicArgs: MnemonicArgs  */
#line 450 "nask.y"
                                { (yyval.listmnemonicargs_) = new ListMnemonicArgs(); (yyval.listmnemonicargs_)->push_back((yyvsp[0].mnemonicargs_)); result->listmnemonicargs_ = (yyval.listmnemonicargs_); }
#line 2080 "parser.cc"
    break;

  case 10: /* ListMnemonicArgs: MnemonicArgs _COMMA ListMnemonicArgs  */
#line 451 "nask.y"
                                         { (yyvsp[0].listmnemonicargs_)->push_back((yyvsp[-2].mnemonicargs_)); (yyval.listmnemonicargs_) = (yyvsp[0].listmnemonicargs_); result->listmnemonicargs_ = (yyval.listmnemonicargs_); }
#line 2086 "parser.cc"
    break;

  case 11: /* MnemonicArgs: Exp  */
#line 453 "nask.y"
                   { (yyval.mnemonicargs_) = new MnemoArgs((yyvsp[0].exp_)); result->mnemonicargs_ = (yyval.mnemonicargs_); }
#line 2092 "parser.cc"
    break;

  case 12: /* Exp: Factor _DEQ Factor  */
#line 455 "nask.y"
                         { (yyval.exp_) = new EqExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2098 "parser.cc"
    break;

  case 13: /* Exp: Factor _BANGEQ Factor  */
#line 456 "nask.y"
                          { (yyval.exp_) = new NeqExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2104 "parser.cc"
    break;

  case 14: /* Exp: Factor _LT Factor  */
#line 457 "nask.y"
                      { (yyval.exp_) = new LtExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2110 "parser.cc"
    break;

  case 15: /* Exp: Factor _GT Factor  */
#line 458 "nask.y"
                      { (yyval.exp_) = new GtExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2116 "parser.cc"
    break;

  case 16: /* Exp: Factor _LDARROW Factor  */
#line 459 "nask.y"
                           { (yyval.exp_) = new LteExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2122 "parser.cc"
    break;

  case 17: /* Exp: Factor _GTEQ Factor  */
#line 460 "nask.y"
                        { (yyval.exp_) = new GteExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2128 "parser.cc"
    break;

  case 18: /* Exp: Factor _PLUS Factor  */
#line 461 "nask.y"
                        { (yyval.exp_) = new PlusExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2134 "parser.cc"
    break;

  case 19: /* Exp: Factor _MINUS Factor  */
#line 462 "nask.y"
                         { (yyval.exp_) = new MinusExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2140 "parser.cc"
    break;

  case 20: /* Exp: Factor _STAR Factor  */
#line 463 "nask.y"
                        { (yyval.exp_) = new MulExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2146 "parser.cc"
    break;

  case 21: /* Exp: Factor _SLASH Factor  */
#line 464 "nask.y"
                         { (yyval.exp_) = new DivExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2152 "parser.cc"
    break;

  case 22: /* Exp: Factor _PERCENT Factor  */
#line 465 "nask.y"
                           { (yyval.exp_) = new ModExp((yyvsp[-2].factor_), (yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2158 "parser.cc"
    break;

  case 23: /* Exp: Factor  */
#line 466 "nask.y"
           { (yyval.exp_) = new ImmExp((yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2164 "parser.cc"
    break;

  case 24: /* Factor: _INTEGER_  */
#line 468 "nask.y"
                   { (yyval.factor_) = new NumberFactor((yyvsp[0]._int)); result->factor_ = (yyval.factor_); }
#line 2170 "parser.cc"
    break;

  case 25: /* Factor: T_Hex  */
#line 469 "nask.y"
          { (yyval.factor_) = new HexFactor((yyvsp[0]._string)); result->factor_ = (yyval.factor_); }
#line 2176 "parser.cc"
    break;

  case 26: /* Factor: _IDENT_  */
#line 470 "nask.y"
            { (yyval.factor_) = new IdentFactor((yyvsp[0]._string)); result->factor_ = (yyval.factor_); }
#line 2182 "parser.cc"
    break;

  case 27: /* Factor: _STRING_  */
#line 471 "nask.y"
             { (yyval.factor_) = new StringFactor((yyvsp[0]._string)); result->factor_ = (yyval.factor_); }
#line 2188 "parser.cc"
    break;

  case 28: /* ConfigType: _KW_BITS  */
#line 473 "nask.y"
                      { (yyval.configtype_) = new BitsConfig(); result->configtype_ = (yyval.configtype_); }
#line 2194 "parser.cc"
    break;

  case 29: /* ConfigType: _KW_INSTRSET  */
#line 474 "nask.y"
                 { (yyval.configtype_) = new InstConfig(); result->configtype_ = (yyval.configtype_); }
#line 2200 "parser.cc"
    break;

  case 30: /* ConfigType: _KW_OPTIMIZE  */
#line 475 "nask.y"
                 { (yyval.configtype_) = new OptiConfig(); result->configtype_ = (yyval.configtype_); }
#line 2206 "parser.cc"
    break;

  case 31: /* ConfigType: _KW_FORMAT  */
#line 476 "nask.y"
               { (yyval.configtype_) = new FormConfig(); result->configtype_ = (yyval.configtype_); }
#line 2212 "parser.cc"
    break;

  case 32: /* ConfigType: _KW_PADDING  */
#line 477 "nask.y"
                { (yyval.configtype_) = new PaddConfig(); result->configtype_ = (yyval.configtype_); }
#line 2218 "parser.cc"
    break;

  case 33: /* ConfigType: _KW_PADSET  */
#line 478 "nask.y"
               { (yyval.configtype_) = new PadsConfig(); result->configtype_ = (yyval.configtype_); }
#line 2224 "parser.cc"
    break;

  case 34: /* ConfigType: _KW_SECTION  */
#line 479 "nask.y"
                { (yyval.configtype_) = new SectConfig(); result->configtype_ = (yyval.configtype_); }
#line 2230 "parser.cc"
    break;

  case 35: /* ConfigType: _KW_ABSOLUTE  */
#line 480 "nask.y"
                 { (yyval.configtype_) = new AbsoConfig(); result->configtype_ = (yyval.configtype_); }
#line 2236 "parser.cc"
    break;

  case 36: /* ConfigType: _KW_FILE  */
#line 481 "nask.y"
             { (yyval.configtype_) = new FileConfig(); result->configtype_ = (yyval.configtype_); }
#line 2242 "parser.cc"
    break;

  case 37: /* Opcode: _KW_AAA  */
#line 483 "nask.y"
                 { (yyval.opcode_) = new OpcodesAAA(); result->opcode_ = (yyval.opcode_); }
#line 2248 "parser.cc"
    break;

  case 38: /* Opcode: _KW_AAD  */
#line 484 "nask.y"
            { (yyval.opcode_) = new OpcodesAAD(); result->opcode_ = (yyval.opcode_); }
#line 2254 "parser.cc"
    break;

  case 39: /* Opcode: _KW_AAS  */
#line 485 "nask.y"
            { (yyval.opcode_) = new OpcodesAAS(); result->opcode_ = (yyval.opcode_); }
#line 2260 "parser.cc"
    break;

  case 40: /* Opcode: _KW_AAM  */
#line 486 "nask.y"
            { (yyval.opcode_) = new OpcodesAAM(); result->opcode_ = (yyval.opcode_); }
#line 2266 "parser.cc"
    break;

  case 41: /* Opcode: _KW_ADC  */
#line 487 "nask.y"
            { (yyval.opcode_) = new OpcodesADC(); result->opcode_ = (yyval.opcode_); }
#line 2272 "parser.cc"
    break;

  case 42: /* Opcode: _KW_ADD  */
#line 488 "nask.y"
            { (yyval.opcode_) = new OpcodesADD(); result->opcode_ = (yyval.opcode_); }
#line 2278 "parser.cc"
    break;

  case 43: /* Opcode: _KW_AND  */
#line 489 "nask.y"
            { (yyval.opcode_) = new OpcodesAND(); result->opcode_ = (yyval.opcode_); }
#line 2284 "parser.cc"
    break;

  case 44: /* Opcode: _KW_ALIGN  */
#line 490 "nask.y"
              { (yyval.opcode_) = new OpcodesALIGN(); result->opcode_ = (yyval.opcode_); }
#line 2290 "parser.cc"
    break;

  case 45: /* Opcode: _KW_ALIGNB  */
#line 491 "nask.y"
               { (yyval.opcode_) = new OpcodesALIGNB(); result->opcode_ = (yyval.opcode_); }
#line 2296 "parser.cc"
    break;

  case 46: /* Opcode: _KW_ARPL  */
#line 492 "nask.y"
             { (yyval.opcode_) = new OpcodesARPL(); result->opcode_ = (yyval.opcode_); }
#line 2302 "parser.cc"
    break;

  case 47: /* Opcode: _KW_BOUND  */
#line 493 "nask.y"
              { (yyval.opcode_) = new OpcodesBOUND(); result->opcode_ = (yyval.opcode_); }
#line 2308 "parser.cc"
    break;

  case 48: /* Opcode: _KW_BSF  */
#line 494 "nask.y"
            { (yyval.opcode_) = new OpcodesBSF(); result->opcode_ = (yyval.opcode_); }
#line 2314 "parser.cc"
    break;

  case 49: /* Opcode: _KW_BSR  */
#line 495 "nask.y"
            { (yyval.opcode_) = new OpcodesBSR(); result->opcode_ = (yyval.opcode_); }
#line 2320 "parser.cc"
    break;

  case 50: /* Opcode: _KW_BSWAP  */
#line 496 "nask.y"
              { (yyval.opcode_) = new OpcodesBSWAP(); result->opcode_ = (yyval.opcode_); }
#line 2326 "parser.cc"
    break;

  case 51: /* Opcode: _KW_BT  */
#line 497 "nask.y"
           { (yyval.opcode_) = new OpcodesBT(); result->opcode_ = (yyval.opcode_); }
#line 2332 "parser.cc"
    break;

  case 52: /* Opcode: _KW_BTC  */
#line 498 "nask.y"
            { (yyval.opcode_) = new OpcodesBTC(); result->opcode_ = (yyval.opcode_); }
#line 2338 "parser.cc"
    break;

  case 53: /* Opcode: _KW_BTR  */
#line 499 "nask.y"
            { (yyval.opcode_) = new OpcodesBTR(); result->opcode_ = (yyval.opcode_); }
#line 2344 "parser.cc"
    break;

  case 54: /* Opcode: _KW_BTS  */
#line 500 "nask.y"
            { (yyval.opcode_) = new OpcodesBTS(); result->opcode_ = (yyval.opcode_); }
#line 2350 "parser.cc"
    break;

  case 55: /* Opcode: _KW_CALL  */
#line 501 "nask.y"
             { (yyval.opcode_) = new OpcodesCALL(); result->opcode_ = (yyval.opcode_); }
#line 2356 "parser.cc"
    break;

  case 56: /* Opcode: _KW_CBW  */
#line 502 "nask.y"
            { (yyval.opcode_) = new OpcodesCBW(); result->opcode_ = (yyval.opcode_); }
#line 2362 "parser.cc"
    break;

  case 57: /* Opcode: _KW_CDQ  */
#line 503 "nask.y"
            { (yyval.opcode_) = new OpcodesCDQ(); result->opcode_ = (yyval.opcode_); }
#line 2368 "parser.cc"
    break;

  case 58: /* Opcode: _KW_CLC  */
#line 504 "nask.y"
            { (yyval.opcode_) = new OpcodesCLC(); result->opcode_ = (yyval.opcode_); }
#line 2374 "parser.cc"
    break;

  case 59: /* Opcode: _KW_CLD  */
#line 505 "nask.y"
            { (yyval.opcode_) = new OpcodesCLD(); result->opcode_ = (yyval.opcode_); }
#line 2380 "parser.cc"
    break;

  case 60: /* Opcode: _KW_CLI  */
#line 506 "nask.y"
            { (yyval.opcode_) = new OpcodesCLI(); result->opcode_ = (yyval.opcode_); }
#line 2386 "parser.cc"
    break;

  case 61: /* Opcode: _KW_CLTS  */
#line 507 "nask.y"
             { (yyval.opcode_) = new OpcodesCLTS(); result->opcode_ = (yyval.opcode_); }
#line 2392 "parser.cc"
    break;

  case 62: /* Opcode: _KW_CMC  */
#line 508 "nask.y"
            { (yyval.opcode_) = new OpcodesCMC(); result->opcode_ = (yyval.opcode_); }
#line 2398 "parser.cc"
    break;

  case 63: /* Opcode: _KW_CMP  */
#line 509 "nask.y"
            { (yyval.opcode_) = new OpcodesCMP(); result->opcode_ = (yyval.opcode_); }
#line 2404 "parser.cc"
    break;

  case 64: /* Opcode: _KW_CMPSB  */
#line 510 "nask.y"
              { (yyval.opcode_) = new OpcodesCMPSB(); result->opcode_ = (yyval.opcode_); }
#line 2410 "parser.cc"
    break;

  case 65: /* Opcode: _KW_CMPSD  */
#line 511 "nask.y"
              { (yyval.opcode_) = new OpcodesCMPSD(); result->opcode_ = (yyval.opcode_); }
#line 2416 "parser.cc"
    break;

  case 66: /* Opcode: _KW_CMPSW  */
#line 512 "nask.y"
              { (yyval.opcode_) = new OpcodesCMPSW(); result->opcode_ = (yyval.opcode_); }
#line 2422 "parser.cc"
    break;

  case 67: /* Opcode: _KW_CMPXCHG  */
#line 513 "nask.y"
                { (yyval.opcode_) = new OpcodesCMPXCHG(); result->opcode_ = (yyval.opcode_); }
#line 2428 "parser.cc"
    break;

  case 68: /* Opcode: _KW_CPUID  */
#line 514 "nask.y"
              { (yyval.opcode_) = new OpcodesCPUID(); result->opcode_ = (yyval.opcode_); }
#line 2434 "parser.cc"
    break;

  case 69: /* Opcode: _KW_CWD  */
#line 515 "nask.y"
            { (yyval.opcode_) = new OpcodesCWD(); result->opcode_ = (yyval.opcode_); }
#line 2440 "parser.cc"
    break;

  case 70: /* Opcode: _KW_CWDE  */
#line 516 "nask.y"
             { (yyval.opcode_) = new OpcodesCWDE(); result->opcode_ = (yyval.opcode_); }
#line 2446 "parser.cc"
    break;

  case 71: /* Opcode: _KW_DAA  */
#line 517 "nask.y"
            { (yyval.opcode_) = new OpcodesDAA(); result->opcode_ = (yyval.opcode_); }
#line 2452 "parser.cc"
    break;

  case 72: /* Opcode: _KW_DAS  */
#line 518 "nask.y"
            { (yyval.opcode_) = new OpcodesDAS(); result->opcode_ = (yyval.opcode_); }
#line 2458 "parser.cc"
    break;

  case 73: /* Opcode: _KW_DB  */
#line 519 "nask.y"
           { (yyval.opcode_) = new OpcodesDB(); result->opcode_ = (yyval.opcode_); }
#line 2464 "parser.cc"
    break;

  case 74: /* Opcode: _KW_DD  */
#line 520 "nask.y"
           { (yyval.opcode_) = new OpcodesDD(); result->opcode_ = (yyval.opcode_); }
#line 2470 "parser.cc"
    break;

  case 75: /* Opcode: _KW_DEC  */
#line 521 "nask.y"
            { (yyval.opcode_) = new OpcodesDEC(); result->opcode_ = (yyval.opcode_); }
#line 2476 "parser.cc"
    break;

  case 76: /* Opcode: _KW_DIV  */
#line 522 "nask.y"
            { (yyval.opcode_) = new OpcodesDIV(); result->opcode_ = (yyval.opcode_); }
#line 2482 "parser.cc"
    break;

  case 77: /* Opcode: _KW_DQ  */
#line 523 "nask.y"
           { (yyval.opcode_) = new OpcodesDQ(); result->opcode_ = (yyval.opcode_); }
#line 2488 "parser.cc"
    break;

  case 78: /* Opcode: _KW_DT  */
#line 524 "nask.y"
           { (yyval.opcode_) = new OpcodesDT(); result->opcode_ = (yyval.opcode_); }
#line 2494 "parser.cc"
    break;

  case 79: /* Opcode: _KW_DW  */
#line 525 "nask.y"
           { (yyval.opcode_) = new OpcodesDW(); result->opcode_ = (yyval.opcode_); }
#line 2500 "parser.cc"
    break;

  case 80: /* Opcode: _KW_END  */
#line 526 "nask.y"
            { (yyval.opcode_) = new OpcodesEND(); result->opcode_ = (yyval.opcode_); }
#line 2506 "parser.cc"
    break;

  case 81: /* Opcode: _KW_ENTER  */
#line 527 "nask.y"
              { (yyval.opcode_) = new OpcodesENTER(); result->opcode_ = (yyval.opcode_); }
#line 2512 "parser.cc"
    break;

  case 82: /* Opcode: _KW_EXTERN  */
#line 528 "nask.y"
               { (yyval.opcode_) = new OpcodesEXTERN(); result->opcode_ = (yyval.opcode_); }
#line 2518 "parser.cc"
    break;

  case 83: /* Opcode: _KW_F2XM1  */
#line 529 "nask.y"
              { (yyval.opcode_) = new OpcodesF2XM1(); result->opcode_ = (yyval.opcode_); }
#line 2524 "parser.cc"
    break;

  case 84: /* Opcode: _KW_FABS  */
#line 530 "nask.y"
             { (yyval.opcode_) = new OpcodesFABS(); result->opcode_ = (yyval.opcode_); }
#line 2530 "parser.cc"
    break;

  case 85: /* Opcode: _KW_FADD  */
#line 531 "nask.y"
             { (yyval.opcode_) = new OpcodesFADD(); result->opcode_ = (yyval.opcode_); }
#line 2536 "parser.cc"
    break;

  case 86: /* Opcode: _KW_FADDP  */
#line 532 "nask.y"
              { (yyval.opcode_) = new OpcodesFADDP(); result->opcode_ = (yyval.opcode_); }
#line 2542 "parser.cc"
    break;

  case 87: /* Opcode: _KW_FBLD  */
#line 533 "nask.y"
             { (yyval.opcode_) = new OpcodesFBLD(); result->opcode_ = (yyval.opcode_); }
#line 2548 "parser.cc"
    break;

  case 88: /* Opcode: _KW_FBSTP  */
#line 534 "nask.y"
              { (yyval.opcode_) = new OpcodesFBSTP(); result->opcode_ = (yyval.opcode_); }
#line 2554 "parser.cc"
    break;

  case 89: /* Opcode: _KW_FCHS  */
#line 535 "nask.y"
             { (yyval.opcode_) = new OpcodesFCHS(); result->opcode_ = (yyval.opcode_); }
#line 2560 "parser.cc"
    break;

  case 90: /* Opcode: _KW_FCLEX  */
#line 536 "nask.y"
              { (yyval.opcode_) = new OpcodesFCLEX(); result->opcode_ = (yyval.opcode_); }
#line 2566 "parser.cc"
    break;

  case 91: /* Opcode: _KW_FCOM  */
#line 537 "nask.y"
             { (yyval.opcode_) = new OpcodesFCOM(); result->opcode_ = (yyval.opcode_); }
#line 2572 "parser.cc"
    break;

  case 92: /* Opcode: _KW_FCOMP  */
#line 538 "nask.y"
              { (yyval.opcode_) = new OpcodesFCOMP(); result->opcode_ = (yyval.opcode_); }
#line 2578 "parser.cc"
    break;

  case 93: /* Opcode: _KW_FCOMPP  */
#line 539 "nask.y"
               { (yyval.opcode_) = new OpcodesFCOMPP(); result->opcode_ = (yyval.opcode_); }
#line 2584 "parser.cc"
    break;

  case 94: /* Opcode: _KW_FCOS  */
#line 540 "nask.y"
             { (yyval.opcode_) = new OpcodesFCOS(); result->opcode_ = (yyval.opcode_); }
#line 2590 "parser.cc"
    break;

  case 95: /* Opcode: _KW_FDECSTP  */
#line 541 "nask.y"
                { (yyval.opcode_) = new OpcodesFDECSTP(); result->opcode_ = (yyval.opcode_); }
#line 2596 "parser.cc"
    break;

  case 96: /* Opcode: _KW_FDISI  */
#line 542 "nask.y"
              { (yyval.opcode_) = new OpcodesFDISI(); result->opcode_ = (yyval.opcode_); }
#line 2602 "parser.cc"
    break;

  case 97: /* Opcode: _KW_FDIV  */
#line 543 "nask.y"
             { (yyval.opcode_) = new OpcodesFDIV(); result->opcode_ = (yyval.opcode_); }
#line 2608 "parser.cc"
    break;

  case 98: /* Opcode: _KW_FDIVP  */
#line 544 "nask.y"
              { (yyval.opcode_) = new OpcodesFDIVP(); result->opcode_ = (yyval.opcode_); }
#line 2614 "parser.cc"
    break;

  case 99: /* Opcode: _KW_FDIVR  */
#line 545 "nask.y"
              { (yyval.opcode_) = new OpcodesFDIVR(); result->opcode_ = (yyval.opcode_); }
#line 2620 "parser.cc"
    break;

  case 100: /* Opcode: _KW_FDIVRP  */
#line 546 "nask.y"
               { (yyval.opcode_) = new OpcodesFDIVRP(); result->opcode_ = (yyval.opcode_); }
#line 2626 "parser.cc"
    break;

  case 101: /* Opcode: _KW_FENI  */
#line 547 "nask.y"
             { (yyval.opcode_) = new OpcodesFENI(); result->opcode_ = (yyval.opcode_); }
#line 2632 "parser.cc"
    break;

  case 102: /* Opcode: _KW_FFREE  */
#line 548 "nask.y"
              { (yyval.opcode_) = new OpcodesFFREE(); result->opcode_ = (yyval.opcode_); }
#line 2638 "parser.cc"
    break;

  case 103: /* Opcode: _KW_FIADD  */
#line 549 "nask.y"
              { (yyval.opcode_) = new OpcodesFIADD(); result->opcode_ = (yyval.opcode_); }
#line 2644 "parser.cc"
    break;

  case 104: /* Opcode: _KW_FICOM  */
#line 550 "nask.y"
              { (yyval.opcode_) = new OpcodesFICOM(); result->opcode_ = (yyval.opcode_); }
#line 2650 "parser.cc"
    break;

  case 105: /* Opcode: _KW_FICOMP  */
#line 551 "nask.y"
               { (yyval.opcode_) = new OpcodesFICOMP(); result->opcode_ = (yyval.opcode_); }
#line 2656 "parser.cc"
    break;

  case 106: /* Opcode: _KW_FIDIV  */
#line 552 "nask.y"
              { (yyval.opcode_) = new OpcodesFIDIV(); result->opcode_ = (yyval.opcode_); }
#line 2662 "parser.cc"
    break;

  case 107: /* Opcode: _KW_FIDIVR  */
#line 553 "nask.y"
               { (yyval.opcode_) = new OpcodesFIDIVR(); result->opcode_ = (yyval.opcode_); }
#line 2668 "parser.cc"
    break;

  case 108: /* Opcode: _KW_FILD  */
#line 554 "nask.y"
             { (yyval.opcode_) = new OpcodesFILD(); result->opcode_ = (yyval.opcode_); }
#line 2674 "parser.cc"
    break;

  case 109: /* Opcode: _KW_FIMUL  */
#line 555 "nask.y"
              { (yyval.opcode_) = new OpcodesFIMUL(); result->opcode_ = (yyval.opcode_); }
#line 2680 "parser.cc"
    break;

  case 110: /* Opcode: _KW_FINCSTP  */
#line 556 "nask.y"
                { (yyval.opcode_) = new OpcodesFINCSTP(); result->opcode_ = (yyval.opcode_); }
#line 2686 "parser.cc"
    break;

  case 111: /* Opcode: _KW_FINIT  */
#line 557 "nask.y"
              { (yyval.opcode_) = new OpcodesFINIT(); result->opcode_ = (yyval.opcode_); }
#line 2692 "parser.cc"
    break;

  case 112: /* Opcode: _KW_FIST  */
#line 558 "nask.y"
             { (yyval.opcode_) = new OpcodesFIST(); result->opcode_ = (yyval.opcode_); }
#line 2698 "parser.cc"
    break;

  case 113: /* Opcode: _KW_FISTP  */
#line 559 "nask.y"
              { (yyval.opcode_) = new OpcodesFISTP(); result->opcode_ = (yyval.opcode_); }
#line 2704 "parser.cc"
    break;

  case 114: /* Opcode: _KW_FISUB  */
#line 560 "nask.y"
              { (yyval.opcode_) = new OpcodesFISUB(); result->opcode_ = (yyval.opcode_); }
#line 2710 "parser.cc"
    break;

  case 115: /* Opcode: _KW_FISUBR  */
#line 561 "nask.y"
               { (yyval.opcode_) = new OpcodesFISUBR(); result->opcode_ = (yyval.opcode_); }
#line 2716 "parser.cc"
    break;

  case 116: /* Opcode: _KW_FLD  */
#line 562 "nask.y"
            { (yyval.opcode_) = new OpcodesFLD(); result->opcode_ = (yyval.opcode_); }
#line 2722 "parser.cc"
    break;

  case 117: /* Opcode: _KW_FLD1  */
#line 563 "nask.y"
             { (yyval.opcode_) = new OpcodesFLD1(); result->opcode_ = (yyval.opcode_); }
#line 2728 "parser.cc"
    break;

  case 118: /* Opcode: _KW_FLDCW  */
#line 564 "nask.y"
              { (yyval.opcode_) = new OpcodesFLDCW(); result->opcode_ = (yyval.opcode_); }
#line 2734 "parser.cc"
    break;

  case 119: /* Opcode: _KW_FLDENV  */
#line 565 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDENV(); result->opcode_ = (yyval.opcode_); }
#line 2740 "parser.cc"
    break;

  case 120: /* Opcode: _KW_FLDL2E  */
#line 566 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDL2E(); result->opcode_ = (yyval.opcode_); }
#line 2746 "parser.cc"
    break;

  case 121: /* Opcode: _KW_FLDL2T  */
#line 567 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDL2T(); result->opcode_ = (yyval.opcode_); }
#line 2752 "parser.cc"
    break;

  case 122: /* Opcode: _KW_FLDLG2  */
#line 568 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDLG2(); result->opcode_ = (yyval.opcode_); }
#line 2758 "parser.cc"
    break;

  case 123: /* Opcode: _KW_FLDLN2  */
#line 569 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDLN2(); result->opcode_ = (yyval.opcode_); }
#line 2764 "parser.cc"
    break;

  case 124: /* Opcode: _KW_FLDPI  */
#line 570 "nask.y"
              { (yyval.opcode_) = new OpcodesFLDPI(); result->opcode_ = (yyval.opcode_); }
#line 2770 "parser.cc"
    break;

  case 125: /* Opcode: _KW_FLDZ  */
#line 571 "nask.y"
             { (yyval.opcode_) = new OpcodesFLDZ(); result->opcode_ = (yyval.opcode_); }
#line 2776 "parser.cc"
    break;

  case 126: /* Opcode: _KW_FMUL  */
#line 572 "nask.y"
             { (yyval.opcode_) = new OpcodesFMUL(); result->opcode_ = (yyval.opcode_); }
#line 2782 "parser.cc"
    break;

  case 127: /* Opcode: _KW_FMULP  */
#line 573 "nask.y"
              { (yyval.opcode_) = new OpcodesFMULP(); result->opcode_ = (yyval.opcode_); }
#line 2788 "parser.cc"
    break;

  case 128: /* Opcode: _KW_FNCLEX  */
#line 574 "nask.y"
               { (yyval.opcode_) = new OpcodesFNCLEX(); result->opcode_ = (yyval.opcode_); }
#line 2794 "parser.cc"
    break;

  case 129: /* Opcode: _KW_FNDISI  */
#line 575 "nask.y"
               { (yyval.opcode_) = new OpcodesFNDISI(); result->opcode_ = (yyval.opcode_); }
#line 2800 "parser.cc"
    break;

  case 130: /* Opcode: _KW_FNENI  */
#line 576 "nask.y"
              { (yyval.opcode_) = new OpcodesFNENI(); result->opcode_ = (yyval.opcode_); }
#line 2806 "parser.cc"
    break;

  case 131: /* Opcode: _KW_FNINIT  */
#line 577 "nask.y"
               { (yyval.opcode_) = new OpcodesFNINIT(); result->opcode_ = (yyval.opcode_); }
#line 2812 "parser.cc"
    break;

  case 132: /* Opcode: _KW_FNOP  */
#line 578 "nask.y"
             { (yyval.opcode_) = new OpcodesFNOP(); result->opcode_ = (yyval.opcode_); }
#line 2818 "parser.cc"
    break;

  case 133: /* Opcode: _KW_FNSAVE  */
#line 579 "nask.y"
               { (yyval.opcode_) = new OpcodesFNSAVE(); result->opcode_ = (yyval.opcode_); }
#line 2824 "parser.cc"
    break;

  case 134: /* Opcode: _KW_FNSTCW  */
#line 580 "nask.y"
               { (yyval.opcode_) = new OpcodesFNSTCW(); result->opcode_ = (yyval.opcode_); }
#line 2830 "parser.cc"
    break;

  case 135: /* Opcode: _KW_FNSTENV  */
#line 581 "nask.y"
                { (yyval.opcode_) = new OpcodesFNSTENV(); result->opcode_ = (yyval.opcode_); }
#line 2836 "parser.cc"
    break;

  case 136: /* Opcode: _KW_FNSTSW  */
#line 582 "nask.y"
               { (yyval.opcode_) = new OpcodesFNSTSW(); result->opcode_ = (yyval.opcode_); }
#line 2842 "parser.cc"
    break;

  case 137: /* Opcode: _KW_FPATAN  */
#line 583 "nask.y"
               { (yyval.opcode_) = new OpcodesFPATAN(); result->opcode_ = (yyval.opcode_); }
#line 2848 "parser.cc"
    break;

  case 138: /* Opcode: _KW_FPTAN  */
#line 584 "nask.y"
              { (yyval.opcode_) = new OpcodesFPTAN(); result->opcode_ = (yyval.opcode_); }
#line 2854 "parser.cc"
    break;

  case 139: /* Opcode: _KW_FPREM  */
#line 585 "nask.y"
              { (yyval.opcode_) = new OpcodesFPREM(); result->opcode_ = (yyval.opcode_); }
#line 2860 "parser.cc"
    break;

  case 140: /* Opcode: _KW_FPREM1  */
#line 586 "nask.y"
               { (yyval.opcode_) = new OpcodesFPREM1(); result->opcode_ = (yyval.opcode_); }
#line 2866 "parser.cc"
    break;

  case 141: /* Opcode: _KW_FRNDINT  */
#line 587 "nask.y"
                { (yyval.opcode_) = new OpcodesFRNDINT(); result->opcode_ = (yyval.opcode_); }
#line 2872 "parser.cc"
    break;

  case 142: /* Opcode: _KW_FRSTOR  */
#line 588 "nask.y"
               { (yyval.opcode_) = new OpcodesFRSTOR(); result->opcode_ = (yyval.opcode_); }
#line 2878 "parser.cc"
    break;

  case 143: /* Opcode: _KW_FSAVE  */
#line 589 "nask.y"
              { (yyval.opcode_) = new OpcodesFSAVE(); result->opcode_ = (yyval.opcode_); }
#line 2884 "parser.cc"
    break;

  case 144: /* Opcode: _KW_FSCALE  */
#line 590 "nask.y"
               { (yyval.opcode_) = new OpcodesFSCALE(); result->opcode_ = (yyval.opcode_); }
#line 2890 "parser.cc"
    break;

  case 145: /* Opcode: _KW_FSETPM  */
#line 591 "nask.y"
               { (yyval.opcode_) = new OpcodesFSETPM(); result->opcode_ = (yyval.opcode_); }
#line 2896 "parser.cc"
    break;

  case 146: /* Opcode: _KW_FSIN  */
#line 592 "nask.y"
             { (yyval.opcode_) = new OpcodesFSIN(); result->opcode_ = (yyval.opcode_); }
#line 2902 "parser.cc"
    break;

  case 147: /* Opcode: _KW_FSINCOS  */
#line 593 "nask.y"
                { (yyval.opcode_) = new OpcodesFSINCOS(); result->opcode_ = (yyval.opcode_); }
#line 2908 "parser.cc"
    break;

  case 148: /* Opcode: _KW_FSQRT  */
#line 594 "nask.y"
              { (yyval.opcode_) = new OpcodesFSQRT(); result->opcode_ = (yyval.opcode_); }
#line 2914 "parser.cc"
    break;

  case 149: /* Opcode: _KW_FST  */
#line 595 "nask.y"
            { (yyval.opcode_) = new OpcodesFST(); result->opcode_ = (yyval.opcode_); }
#line 2920 "parser.cc"
    break;

  case 150: /* Opcode: _KW_FSTCW  */
#line 596 "nask.y"
              { (yyval.opcode_) = new OpcodesFSTCW(); result->opcode_ = (yyval.opcode_); }
#line 2926 "parser.cc"
    break;

  case 151: /* Opcode: _KW_FSTENV  */
#line 597 "nask.y"
               { (yyval.opcode_) = new OpcodesFSTENV(); result->opcode_ = (yyval.opcode_); }
#line 2932 "parser.cc"
    break;

  case 152: /* Opcode: _KW_FSTP  */
#line 598 "nask.y"
             { (yyval.opcode_) = new OpcodesFSTP(); result->opcode_ = (yyval.opcode_); }
#line 2938 "parser.cc"
    break;

  case 153: /* Opcode: _KW_FSTSW  */
#line 599 "nask.y"
              { (yyval.opcode_) = new OpcodesFSTSW(); result->opcode_ = (yyval.opcode_); }
#line 2944 "parser.cc"
    break;

  case 154: /* Opcode: _KW_FSUB  */
#line 600 "nask.y"
             { (yyval.opcode_) = new OpcodesFSUB(); result->opcode_ = (yyval.opcode_); }
#line 2950 "parser.cc"
    break;

  case 155: /* Opcode: _KW_FSUBP  */
#line 601 "nask.y"
              { (yyval.opcode_) = new OpcodesFSUBP(); result->opcode_ = (yyval.opcode_); }
#line 2956 "parser.cc"
    break;

  case 156: /* Opcode: _KW_FSUBR  */
#line 602 "nask.y"
              { (yyval.opcode_) = new OpcodesFSUBR(); result->opcode_ = (yyval.opcode_); }
#line 2962 "parser.cc"
    break;

  case 157: /* Opcode: _KW_FSUBRP  */
#line 603 "nask.y"
               { (yyval.opcode_) = new OpcodesFSUBRP(); result->opcode_ = (yyval.opcode_); }
#line 2968 "parser.cc"
    break;

  case 158: /* Opcode: _KW_FTST  */
#line 604 "nask.y"
             { (yyval.opcode_) = new OpcodesFTST(); result->opcode_ = (yyval.opcode_); }
#line 2974 "parser.cc"
    break;

  case 159: /* Opcode: _KW_FUCOM  */
#line 605 "nask.y"
              { (yyval.opcode_) = new OpcodesFUCOM(); result->opcode_ = (yyval.opcode_); }
#line 2980 "parser.cc"
    break;

  case 160: /* Opcode: _KW_FUCOMP  */
#line 606 "nask.y"
               { (yyval.opcode_) = new OpcodesFUCOMP(); result->opcode_ = (yyval.opcode_); }
#line 2986 "parser.cc"
    break;

  case 161: /* Opcode: _KW_FUCOMPP  */
#line 607 "nask.y"
                { (yyval.opcode_) = new OpcodesFUCOMPP(); result->opcode_ = (yyval.opcode_); }
#line 2992 "parser.cc"
    break;

  case 162: /* Opcode: _KW_FXAM  */
#line 608 "nask.y"
             { (yyval.opcode_) = new OpcodesFXAM(); result->opcode_ = (yyval.opcode_); }
#line 2998 "parser.cc"
    break;

  case 163: /* Opcode: _KW_FXCH  */
#line 609 "nask.y"
             { (yyval.opcode_) = new OpcodesFXCH(); result->opcode_ = (yyval.opcode_); }
#line 3004 "parser.cc"
    break;

  case 164: /* Opcode: _KW_FXTRACT  */
#line 610 "nask.y"
                { (yyval.opcode_) = new OpcodesFXTRACT(); result->opcode_ = (yyval.opcode_); }
#line 3010 "parser.cc"
    break;

  case 165: /* Opcode: _KW_FYL2X  */
#line 611 "nask.y"
              { (yyval.opcode_) = new OpcodesFYL2X(); result->opcode_ = (yyval.opcode_); }
#line 3016 "parser.cc"
    break;

  case 166: /* Opcode: _KW_FYL2XP1  */
#line 612 "nask.y"
                { (yyval.opcode_) = new OpcodesFYL2XP1(); result->opcode_ = (yyval.opcode_); }
#line 3022 "parser.cc"
    break;

  case 167: /* Opcode: _KW_HLT  */
#line 613 "nask.y"
            { (yyval.opcode_) = new OpcodesHLT(); result->opcode_ = (yyval.opcode_); }
#line 3028 "parser.cc"
    break;

  case 168: /* Opcode: _KW_IDIV  */
#line 614 "nask.y"
             { (yyval.opcode_) = new OpcodesIDIV(); result->opcode_ = (yyval.opcode_); }
#line 3034 "parser.cc"
    break;

  case 169: /* Opcode: _KW_IMUL  */
#line 615 "nask.y"
             { (yyval.opcode_) = new OpcodesIMUL(); result->opcode_ = (yyval.opcode_); }
#line 3040 "parser.cc"
    break;

  case 170: /* Opcode: _KW_IN  */
#line 616 "nask.y"
           { (yyval.opcode_) = new OpcodesIN(); result->opcode_ = (yyval.opcode_); }
#line 3046 "parser.cc"
    break;

  case 171: /* Opcode: _KW_INC  */
#line 617 "nask.y"
            { (yyval.opcode_) = new OpcodesINC(); result->opcode_ = (yyval.opcode_); }
#line 3052 "parser.cc"
    break;

  case 172: /* Opcode: _KW_INCO  */
#line 618 "nask.y"
             { (yyval.opcode_) = new OpcodesINCO(); result->opcode_ = (yyval.opcode_); }
#line 3058 "parser.cc"
    break;

  case 173: /* Opcode: _KW_INSB  */
#line 619 "nask.y"
             { (yyval.opcode_) = new OpcodesINSB(); result->opcode_ = (yyval.opcode_); }
#line 3064 "parser.cc"
    break;

  case 174: /* Opcode: _KW_INSD  */
#line 620 "nask.y"
             { (yyval.opcode_) = new OpcodesINSD(); result->opcode_ = (yyval.opcode_); }
#line 3070 "parser.cc"
    break;

  case 175: /* Opcode: _KW_INSW  */
#line 621 "nask.y"
             { (yyval.opcode_) = new OpcodesINSW(); result->opcode_ = (yyval.opcode_); }
#line 3076 "parser.cc"
    break;

  case 176: /* Opcode: _KW_INT  */
#line 622 "nask.y"
            { (yyval.opcode_) = new OpcodesINT(); result->opcode_ = (yyval.opcode_); }
#line 3082 "parser.cc"
    break;

  case 177: /* Opcode: _KW_INT3  */
#line 623 "nask.y"
             { (yyval.opcode_) = new OpcodesINT3(); result->opcode_ = (yyval.opcode_); }
#line 3088 "parser.cc"
    break;

  case 178: /* Opcode: _KW_INTO  */
#line 624 "nask.y"
             { (yyval.opcode_) = new OpcodesINTO(); result->opcode_ = (yyval.opcode_); }
#line 3094 "parser.cc"
    break;

  case 179: /* Opcode: _KW_INVD  */
#line 625 "nask.y"
             { (yyval.opcode_) = new OpcodesINVD(); result->opcode_ = (yyval.opcode_); }
#line 3100 "parser.cc"
    break;

  case 180: /* Opcode: _KW_INVLPG  */
#line 626 "nask.y"
               { (yyval.opcode_) = new OpcodesINVLPG(); result->opcode_ = (yyval.opcode_); }
#line 3106 "parser.cc"
    break;

  case 181: /* Opcode: _KW_IRET  */
#line 627 "nask.y"
             { (yyval.opcode_) = new OpcodesIRET(); result->opcode_ = (yyval.opcode_); }
#line 3112 "parser.cc"
    break;

  case 182: /* Opcode: _KW_IRETD  */
#line 628 "nask.y"
              { (yyval.opcode_) = new OpcodesIRETD(); result->opcode_ = (yyval.opcode_); }
#line 3118 "parser.cc"
    break;

  case 183: /* Opcode: _KW_IRETW  */
#line 629 "nask.y"
              { (yyval.opcode_) = new OpcodesIRETW(); result->opcode_ = (yyval.opcode_); }
#line 3124 "parser.cc"
    break;

  case 184: /* Opcode: _KW_JA  */
#line 630 "nask.y"
           { (yyval.opcode_) = new OpcodesJA(); result->opcode_ = (yyval.opcode_); }
#line 3130 "parser.cc"
    break;

  case 185: /* Opcode: _KW_JAE  */
#line 631 "nask.y"
            { (yyval.opcode_) = new OpcodesJAE(); result->opcode_ = (yyval.opcode_); }
#line 3136 "parser.cc"
    break;

  case 186: /* Opcode: _KW_JB  */
#line 632 "nask.y"
           { (yyval.opcode_) = new OpcodesJB(); result->opcode_ = (yyval.opcode_); }
#line 3142 "parser.cc"
    break;

  case 187: /* Opcode: _KW_JBE  */
#line 633 "nask.y"
            { (yyval.opcode_) = new OpcodesJBE(); result->opcode_ = (yyval.opcode_); }
#line 3148 "parser.cc"
    break;

  case 188: /* Opcode: _KW_JC  */
#line 634 "nask.y"
           { (yyval.opcode_) = new OpcodesJC(); result->opcode_ = (yyval.opcode_); }
#line 3154 "parser.cc"
    break;

  case 189: /* Opcode: _KW_JCXZ  */
#line 635 "nask.y"
             { (yyval.opcode_) = new OpcodesJCXZ(); result->opcode_ = (yyval.opcode_); }
#line 3160 "parser.cc"
    break;

  case 190: /* Opcode: _KW_JE  */
#line 636 "nask.y"
           { (yyval.opcode_) = new OpcodesJE(); result->opcode_ = (yyval.opcode_); }
#line 3166 "parser.cc"
    break;

  case 191: /* Opcode: _KW_JECXZ  */
#line 637 "nask.y"
              { (yyval.opcode_) = new OpcodesJECXZ(); result->opcode_ = (yyval.opcode_); }
#line 3172 "parser.cc"
    break;

  case 192: /* Opcode: _KW_JG  */
#line 638 "nask.y"
           { (yyval.opcode_) = new OpcodesJG(); result->opcode_ = (yyval.opcode_); }
#line 3178 "parser.cc"
    break;

  case 193: /* Opcode: _KW_JGE  */
#line 639 "nask.y"
            { (yyval.opcode_) = new OpcodesJGE(); result->opcode_ = (yyval.opcode_); }
#line 3184 "parser.cc"
    break;

  case 194: /* Opcode: _KW_JL  */
#line 640 "nask.y"
           { (yyval.opcode_) = new OpcodesJL(); result->opcode_ = (yyval.opcode_); }
#line 3190 "parser.cc"
    break;

  case 195: /* Opcode: _KW_JLE  */
#line 641 "nask.y"
            { (yyval.opcode_) = new OpcodesJLE(); result->opcode_ = (yyval.opcode_); }
#line 3196 "parser.cc"
    break;

  case 196: /* Opcode: _KW_JMP  */
#line 642 "nask.y"
            { (yyval.opcode_) = new OpcodesJMP(); result->opcode_ = (yyval.opcode_); }
#line 3202 "parser.cc"
    break;

  case 197: /* Opcode: _KW_JNA  */
#line 643 "nask.y"
            { (yyval.opcode_) = new OpcodesJNA(); result->opcode_ = (yyval.opcode_); }
#line 3208 "parser.cc"
    break;

  case 198: /* Opcode: _KW_JNAE  */
#line 644 "nask.y"
             { (yyval.opcode_) = new OpcodesJNAE(); result->opcode_ = (yyval.opcode_); }
#line 3214 "parser.cc"
    break;

  case 199: /* Opcode: _KW_JNB  */
#line 645 "nask.y"
            { (yyval.opcode_) = new OpcodesJNB(); result->opcode_ = (yyval.opcode_); }
#line 3220 "parser.cc"
    break;

  case 200: /* Opcode: _KW_JNBE  */
#line 646 "nask.y"
             { (yyval.opcode_) = new OpcodesJNBE(); result->opcode_ = (yyval.opcode_); }
#line 3226 "parser.cc"
    break;

  case 201: /* Opcode: _KW_JNC  */
#line 647 "nask.y"
            { (yyval.opcode_) = new OpcodesJNC(); result->opcode_ = (yyval.opcode_); }
#line 3232 "parser.cc"
    break;

  case 202: /* Opcode: _KW_JNE  */
#line 648 "nask.y"
            { (yyval.opcode_) = new OpcodesJNE(); result->opcode_ = (yyval.opcode_); }
#line 3238 "parser.cc"
    break;

  case 203: /* Opcode: _KW_JNG  */
#line 649 "nask.y"
            { (yyval.opcode_) = new OpcodesJNG(); result->opcode_ = (yyval.opcode_); }
#line 3244 "parser.cc"
    break;

  case 204: /* Opcode: _KW_JNGE  */
#line 650 "nask.y"
             { (yyval.opcode_) = new OpcodesJNGE(); result->opcode_ = (yyval.opcode_); }
#line 3250 "parser.cc"
    break;

  case 205: /* Opcode: _KW_JNL  */
#line 651 "nask.y"
            { (yyval.opcode_) = new OpcodesJNL(); result->opcode_ = (yyval.opcode_); }
#line 3256 "parser.cc"
    break;

  case 206: /* Opcode: _KW_JNLE  */
#line 652 "nask.y"
             { (yyval.opcode_) = new OpcodesJNLE(); result->opcode_ = (yyval.opcode_); }
#line 3262 "parser.cc"
    break;

  case 207: /* Opcode: _KW_JNO  */
#line 653 "nask.y"
            { (yyval.opcode_) = new OpcodesJNO(); result->opcode_ = (yyval.opcode_); }
#line 3268 "parser.cc"
    break;

  case 208: /* Opcode: _KW_JNP  */
#line 654 "nask.y"
            { (yyval.opcode_) = new OpcodesJNP(); result->opcode_ = (yyval.opcode_); }
#line 3274 "parser.cc"
    break;

  case 209: /* Opcode: _KW_JNS  */
#line 655 "nask.y"
            { (yyval.opcode_) = new OpcodesJNS(); result->opcode_ = (yyval.opcode_); }
#line 3280 "parser.cc"
    break;

  case 210: /* Opcode: _KW_JNZ  */
#line 656 "nask.y"
            { (yyval.opcode_) = new OpcodesJNZ(); result->opcode_ = (yyval.opcode_); }
#line 3286 "parser.cc"
    break;

  case 211: /* Opcode: _KW_JO  */
#line 657 "nask.y"
           { (yyval.opcode_) = new OpcodesJO(); result->opcode_ = (yyval.opcode_); }
#line 3292 "parser.cc"
    break;

  case 212: /* Opcode: _KW_JP  */
#line 658 "nask.y"
           { (yyval.opcode_) = new OpcodesJP(); result->opcode_ = (yyval.opcode_); }
#line 3298 "parser.cc"
    break;

  case 213: /* Opcode: _KW_JPE  */
#line 659 "nask.y"
            { (yyval.opcode_) = new OpcodesJPE(); result->opcode_ = (yyval.opcode_); }
#line 3304 "parser.cc"
    break;

  case 214: /* Opcode: _KW_JPO  */
#line 660 "nask.y"
            { (yyval.opcode_) = new OpcodesJPO(); result->opcode_ = (yyval.opcode_); }
#line 3310 "parser.cc"
    break;

  case 215: /* Opcode: _KW_JS  */
#line 661 "nask.y"
           { (yyval.opcode_) = new OpcodesJS(); result->opcode_ = (yyval.opcode_); }
#line 3316 "parser.cc"
    break;

  case 216: /* Opcode: _KW_JZ  */
#line 662 "nask.y"
           { (yyval.opcode_) = new OpcodesJZ(); result->opcode_ = (yyval.opcode_); }
#line 3322 "parser.cc"
    break;

  case 217: /* Opcode: _KW_LAHF  */
#line 663 "nask.y"
             { (yyval.opcode_) = new OpcodesLAHF(); result->opcode_ = (yyval.opcode_); }
#line 3328 "parser.cc"
    break;

  case 218: /* Opcode: _KW_LAR  */
#line 664 "nask.y"
            { (yyval.opcode_) = new OpcodesLAR(); result->opcode_ = (yyval.opcode_); }
#line 3334 "parser.cc"
    break;

  case 219: /* Opcode: _KW_LDS  */
#line 665 "nask.y"
            { (yyval.opcode_) = new OpcodesLDS(); result->opcode_ = (yyval.opcode_); }
#line 3340 "parser.cc"
    break;

  case 220: /* Opcode: _KW_LEA  */
#line 666 "nask.y"
            { (yyval.opcode_) = new OpcodesLEA(); result->opcode_ = (yyval.opcode_); }
#line 3346 "parser.cc"
    break;

  case 221: /* Opcode: _KW_LEAVE  */
#line 667 "nask.y"
              { (yyval.opcode_) = new OpcodesLEAVE(); result->opcode_ = (yyval.opcode_); }
#line 3352 "parser.cc"
    break;

  case 222: /* Opcode: _KW_LES  */
#line 668 "nask.y"
            { (yyval.opcode_) = new OpcodesLES(); result->opcode_ = (yyval.opcode_); }
#line 3358 "parser.cc"
    break;

  case 223: /* Opcode: _KW_LFS  */
#line 669 "nask.y"
            { (yyval.opcode_) = new OpcodesLFS(); result->opcode_ = (yyval.opcode_); }
#line 3364 "parser.cc"
    break;

  case 224: /* Opcode: _KW_LGDT  */
#line 670 "nask.y"
             { (yyval.opcode_) = new OpcodesLGDT(); result->opcode_ = (yyval.opcode_); }
#line 3370 "parser.cc"
    break;

  case 225: /* Opcode: _KW_LGS  */
#line 671 "nask.y"
            { (yyval.opcode_) = new OpcodesLGS(); result->opcode_ = (yyval.opcode_); }
#line 3376 "parser.cc"
    break;

  case 226: /* Opcode: _KW_LIDT  */
#line 672 "nask.y"
             { (yyval.opcode_) = new OpcodesLIDT(); result->opcode_ = (yyval.opcode_); }
#line 3382 "parser.cc"
    break;

  case 227: /* Opcode: _KW_LLDT  */
#line 673 "nask.y"
             { (yyval.opcode_) = new OpcodesLLDT(); result->opcode_ = (yyval.opcode_); }
#line 3388 "parser.cc"
    break;

  case 228: /* Opcode: _KW_LMSW  */
#line 674 "nask.y"
             { (yyval.opcode_) = new OpcodesLMSW(); result->opcode_ = (yyval.opcode_); }
#line 3394 "parser.cc"
    break;

  case 229: /* Opcode: _KW_LOCK  */
#line 675 "nask.y"
             { (yyval.opcode_) = new OpcodesLOCK(); result->opcode_ = (yyval.opcode_); }
#line 3400 "parser.cc"
    break;

  case 230: /* Opcode: _KW_LODSB  */
#line 676 "nask.y"
              { (yyval.opcode_) = new OpcodesLODSB(); result->opcode_ = (yyval.opcode_); }
#line 3406 "parser.cc"
    break;

  case 231: /* Opcode: _KW_LODSD  */
#line 677 "nask.y"
              { (yyval.opcode_) = new OpcodesLODSD(); result->opcode_ = (yyval.opcode_); }
#line 3412 "parser.cc"
    break;

  case 232: /* Opcode: _KW_LODSW  */
#line 678 "nask.y"
              { (yyval.opcode_) = new OpcodesLODSW(); result->opcode_ = (yyval.opcode_); }
#line 3418 "parser.cc"
    break;

  case 233: /* Opcode: _KW_LOOP  */
#line 679 "nask.y"
             { (yyval.opcode_) = new OpcodesLOOP(); result->opcode_ = (yyval.opcode_); }
#line 3424 "parser.cc"
    break;

  case 234: /* Opcode: _KW_LOOPE  */
#line 680 "nask.y"
              { (yyval.opcode_) = new OpcodesLOOPE(); result->opcode_ = (yyval.opcode_); }
#line 3430 "parser.cc"
    break;

  case 235: /* Opcode: _KW_LOOPNE  */
#line 681 "nask.y"
               { (yyval.opcode_) = new OpcodesLOOPNE(); result->opcode_ = (yyval.opcode_); }
#line 3436 "parser.cc"
    break;

  case 236: /* Opcode: _KW_LOOPNZ  */
#line 682 "nask.y"
               { (yyval.opcode_) = new OpcodesLOOPNZ(); result->opcode_ = (yyval.opcode_); }
#line 3442 "parser.cc"
    break;

  case 237: /* Opcode: _KW_LOOPZ  */
#line 683 "nask.y"
              { (yyval.opcode_) = new OpcodesLOOPZ(); result->opcode_ = (yyval.opcode_); }
#line 3448 "parser.cc"
    break;

  case 238: /* Opcode: _KW_LSL  */
#line 684 "nask.y"
            { (yyval.opcode_) = new OpcodesLSL(); result->opcode_ = (yyval.opcode_); }
#line 3454 "parser.cc"
    break;

  case 239: /* Opcode: _KW_LSS  */
#line 685 "nask.y"
            { (yyval.opcode_) = new OpcodesLSS(); result->opcode_ = (yyval.opcode_); }
#line 3460 "parser.cc"
    break;

  case 240: /* Opcode: _KW_LTR  */
#line 686 "nask.y"
            { (yyval.opcode_) = new OpcodesLTR(); result->opcode_ = (yyval.opcode_); }
#line 3466 "parser.cc"
    break;

  case 241: /* Opcode: _KW_MOV  */
#line 687 "nask.y"
            { (yyval.opcode_) = new OpcodesMOV(); result->opcode_ = (yyval.opcode_); }
#line 3472 "parser.cc"
    break;

  case 242: /* Opcode: _KW_MOVSB  */
#line 688 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSB(); result->opcode_ = (yyval.opcode_); }
#line 3478 "parser.cc"
    break;

  case 243: /* Opcode: _KW_MOVSD  */
#line 689 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSD(); result->opcode_ = (yyval.opcode_); }
#line 3484 "parser.cc"
    break;

  case 244: /* Opcode: _KW_MOVSW  */
#line 690 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSW(); result->opcode_ = (yyval.opcode_); }
#line 3490 "parser.cc"
    break;

  case 245: /* Opcode: _KW_MOVSX  */
#line 691 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSX(); result->opcode_ = (yyval.opcode_); }
#line 3496 "parser.cc"
    break;

  case 246: /* Opcode: _KW_MOVZX  */
#line 692 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVZX(); result->opcode_ = (yyval.opcode_); }
#line 3502 "parser.cc"
    break;

  case 247: /* Opcode: _KW_MUL  */
#line 693 "nask.y"
            { (yyval.opcode_) = new OpcodesMUL(); result->opcode_ = (yyval.opcode_); }
#line 3508 "parser.cc"
    break;

  case 248: /* Opcode: _KW_NEG  */
#line 694 "nask.y"
            { (yyval.opcode_) = new OpcodesNEG(); result->opcode_ = (yyval.opcode_); }
#line 3514 "parser.cc"
    break;

  case 249: /* Opcode: _KW_NOP  */
#line 695 "nask.y"
            { (yyval.opcode_) = new OpcodesNOP(); result->opcode_ = (yyval.opcode_); }
#line 3520 "parser.cc"
    break;

  case 250: /* Opcode: _KW_NOT  */
#line 696 "nask.y"
            { (yyval.opcode_) = new OpcodesNOT(); result->opcode_ = (yyval.opcode_); }
#line 3526 "parser.cc"
    break;

  case 251: /* Opcode: _KW_OR  */
#line 697 "nask.y"
           { (yyval.opcode_) = new OpcodesOR(); result->opcode_ = (yyval.opcode_); }
#line 3532 "parser.cc"
    break;

  case 252: /* Opcode: _KW_ORG  */
#line 698 "nask.y"
            { (yyval.opcode_) = new OpcodesORG(); result->opcode_ = (yyval.opcode_); }
#line 3538 "parser.cc"
    break;

  case 253: /* Opcode: _KW_OUT  */
#line 699 "nask.y"
            { (yyval.opcode_) = new OpcodesOUT(); result->opcode_ = (yyval.opcode_); }
#line 3544 "parser.cc"
    break;

  case 254: /* Opcode: _KW_OUTSB  */
#line 700 "nask.y"
              { (yyval.opcode_) = new OpcodesOUTSB(); result->opcode_ = (yyval.opcode_); }
#line 3550 "parser.cc"
    break;

  case 255: /* Opcode: _KW_OUTSD  */
#line 701 "nask.y"
              { (yyval.opcode_) = new OpcodesOUTSD(); result->opcode_ = (yyval.opcode_); }
#line 3556 "parser.cc"
    break;

  case 256: /* Opcode: _KW_OUTSW  */
#line 702 "nask.y"
              { (yyval.opcode_) = new OpcodesOUTSW(); result->opcode_ = (yyval.opcode_); }
#line 3562 "parser.cc"
    break;

  case 257: /* Opcode: _KW_POP  */
#line 703 "nask.y"
            { (yyval.opcode_) = new OpcodesPOP(); result->opcode_ = (yyval.opcode_); }
#line 3568 "parser.cc"
    break;

  case 258: /* Opcode: _KW_POPA  */
#line 704 "nask.y"
             { (yyval.opcode_) = new OpcodesPOPA(); result->opcode_ = (yyval.opcode_); }
#line 3574 "parser.cc"
    break;

  case 259: /* Opcode: _KW_POPAD  */
#line 705 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPAD(); result->opcode_ = (yyval.opcode_); }
#line 3580 "parser.cc"
    break;

  case 260: /* Opcode: _KW_POPAW  */
#line 706 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPAW(); result->opcode_ = (yyval.opcode_); }
#line 3586 "parser.cc"
    break;

  case 261: /* Opcode: _KW_POPF  */
#line 707 "nask.y"
             { (yyval.opcode_) = new OpcodesPOPF(); result->opcode_ = (yyval.opcode_); }
#line 3592 "parser.cc"
    break;

  case 262: /* Opcode: _KW_POPFD  */
#line 708 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPFD(); result->opcode_ = (yyval.opcode_); }
#line 3598 "parser.cc"
    break;

  case 263: /* Opcode: _KW_POPFW  */
#line 709 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPFW(); result->opcode_ = (yyval.opcode_); }
#line 3604 "parser.cc"
    break;

  case 264: /* Opcode: _KW_PUSH  */
#line 710 "nask.y"
             { (yyval.opcode_) = new OpcodesPUSH(); result->opcode_ = (yyval.opcode_); }
#line 3610 "parser.cc"
    break;

  case 265: /* Opcode: _KW_PUSHA  */
#line 711 "nask.y"
              { (yyval.opcode_) = new OpcodesPUSHA(); result->opcode_ = (yyval.opcode_); }
#line 3616 "parser.cc"
    break;

  case 266: /* Opcode: _KW_PUSHD  */
#line 712 "nask.y"
              { (yyval.opcode_) = new OpcodesPUSHD(); result->opcode_ = (yyval.opcode_); }
#line 3622 "parser.cc"
    break;

  case 267: /* Opcode: _KW_PUSHAD  */
#line 713 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHAD(); result->opcode_ = (yyval.opcode_); }
#line 3628 "parser.cc"
    break;

  case 268: /* Opcode: _KW_PUSHAW  */
#line 714 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHAW(); result->opcode_ = (yyval.opcode_); }
#line 3634 "parser.cc"
    break;

  case 269: /* Opcode: _KW_PUSHF  */
#line 715 "nask.y"
              { (yyval.opcode_) = new OpcodesPUSHF(); result->opcode_ = (yyval.opcode_); }
#line 3640 "parser.cc"
    break;

  case 270: /* Opcode: _KW_PUSHFD  */
#line 716 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHFD(); result->opcode_ = (yyval.opcode_); }
#line 3646 "parser.cc"
    break;

  case 271: /* Opcode: _KW_PUSHFW  */
#line 717 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHFW(); result->opcode_ = (yyval.opcode_); }
#line 3652 "parser.cc"
    break;

  case 272: /* Opcode: _KW_RCL  */
#line 718 "nask.y"
            { (yyval.opcode_) = new OpcodesRCL(); result->opcode_ = (yyval.opcode_); }
#line 3658 "parser.cc"
    break;

  case 273: /* Opcode: _KW_RCR  */
#line 719 "nask.y"
            { (yyval.opcode_) = new OpcodesRCR(); result->opcode_ = (yyval.opcode_); }
#line 3664 "parser.cc"
    break;

  case 274: /* Opcode: _KW_RDMSR  */
#line 720 "nask.y"
              { (yyval.opcode_) = new OpcodesRDMSR(); result->opcode_ = (yyval.opcode_); }
#line 3670 "parser.cc"
    break;

  case 275: /* Opcode: _KW_RDPMC  */
#line 721 "nask.y"
              { (yyval.opcode_) = new OpcodesRDPMC(); result->opcode_ = (yyval.opcode_); }
#line 3676 "parser.cc"
    break;

  case 276: /* Opcode: _KW_REP  */
#line 722 "nask.y"
            { (yyval.opcode_) = new OpcodesREP(); result->opcode_ = (yyval.opcode_); }
#line 3682 "parser.cc"
    break;

  case 277: /* Opcode: _KW_REPE  */
#line 723 "nask.y"
             { (yyval.opcode_) = new OpcodesREPE(); result->opcode_ = (yyval.opcode_); }
#line 3688 "parser.cc"
    break;

  case 278: /* Opcode: _KW_REPNE  */
#line 724 "nask.y"
              { (yyval.opcode_) = new OpcodesREPNE(); result->opcode_ = (yyval.opcode_); }
#line 3694 "parser.cc"
    break;

  case 279: /* Opcode: _KW_REPNZ  */
#line 725 "nask.y"
              { (yyval.opcode_) = new OpcodesREPNZ(); result->opcode_ = (yyval.opcode_); }
#line 3700 "parser.cc"
    break;

  case 280: /* Opcode: _KW_REPZ  */
#line 726 "nask.y"
             { (yyval.opcode_) = new OpcodesREPZ(); result->opcode_ = (yyval.opcode_); }
#line 3706 "parser.cc"
    break;

  case 281: /* Opcode: _KW_RESB  */
#line 727 "nask.y"
             { (yyval.opcode_) = new OpcodesRESB(); result->opcode_ = (yyval.opcode_); }
#line 3712 "parser.cc"
    break;

  case 282: /* Opcode: _KW_RESD  */
#line 728 "nask.y"
             { (yyval.opcode_) = new OpcodesRESD(); result->opcode_ = (yyval.opcode_); }
#line 3718 "parser.cc"
    break;

  case 283: /* Opcode: _KW_RESQ  */
#line 729 "nask.y"
             { (yyval.opcode_) = new OpcodesRESQ(); result->opcode_ = (yyval.opcode_); }
#line 3724 "parser.cc"
    break;

  case 284: /* Opcode: _KW_REST  */
#line 730 "nask.y"
             { (yyval.opcode_) = new OpcodesREST(); result->opcode_ = (yyval.opcode_); }
#line 3730 "parser.cc"
    break;

  case 285: /* Opcode: _KW_RESW  */
#line 731 "nask.y"
             { (yyval.opcode_) = new OpcodesRESW(); result->opcode_ = (yyval.opcode_); }
#line 3736 "parser.cc"
    break;

  case 286: /* Opcode: _KW_RET  */
#line 732 "nask.y"
            { (yyval.opcode_) = new OpcodesRET(); result->opcode_ = (yyval.opcode_); }
#line 3742 "parser.cc"
    break;

  case 287: /* Opcode: _KW_RETF  */
#line 733 "nask.y"
             { (yyval.opcode_) = new OpcodesRETF(); result->opcode_ = (yyval.opcode_); }
#line 3748 "parser.cc"
    break;

  case 288: /* Opcode: _KW_RETN  */
#line 734 "nask.y"
             { (yyval.opcode_) = new OpcodesRETN(); result->opcode_ = (yyval.opcode_); }
#line 3754 "parser.cc"
    break;

  case 289: /* Opcode: _KW_ROL  */
#line 735 "nask.y"
            { (yyval.opcode_) = new OpcodesROL(); result->opcode_ = (yyval.opcode_); }
#line 3760 "parser.cc"
    break;

  case 290: /* Opcode: _KW_ROR  */
#line 736 "nask.y"
            { (yyval.opcode_) = new OpcodesROR(); result->opcode_ = (yyval.opcode_); }
#line 3766 "parser.cc"
    break;

  case 291: /* Opcode: _KW_RSM  */
#line 737 "nask.y"
            { (yyval.opcode_) = new OpcodesRSM(); result->opcode_ = (yyval.opcode_); }
#line 3772 "parser.cc"
    break;

  case 292: /* Opcode: _KW_SAHF  */
#line 738 "nask.y"
             { (yyval.opcode_) = new OpcodesSAHF(); result->opcode_ = (yyval.opcode_); }
#line 3778 "parser.cc"
    break;

  case 293: /* Opcode: _KW_SAL  */
#line 739 "nask.y"
            { (yyval.opcode_) = new OpcodesSAL(); result->opcode_ = (yyval.opcode_); }
#line 3784 "parser.cc"
    break;

  case 294: /* Opcode: _KW_SAR  */
#line 740 "nask.y"
            { (yyval.opcode_) = new OpcodesSAR(); result->opcode_ = (yyval.opcode_); }
#line 3790 "parser.cc"
    break;

  case 295: /* Opcode: _KW_SBB  */
#line 741 "nask.y"
            { (yyval.opcode_) = new OpcodesSBB(); result->opcode_ = (yyval.opcode_); }
#line 3796 "parser.cc"
    break;

  case 296: /* Opcode: _KW_SCASB  */
#line 742 "nask.y"
              { (yyval.opcode_) = new OpcodesSCASB(); result->opcode_ = (yyval.opcode_); }
#line 3802 "parser.cc"
    break;

  case 297: /* Opcode: _KW_SCASD  */
#line 743 "nask.y"
              { (yyval.opcode_) = new OpcodesSCASD(); result->opcode_ = (yyval.opcode_); }
#line 3808 "parser.cc"
    break;

  case 298: /* Opcode: _KW_SCASW  */
#line 744 "nask.y"
              { (yyval.opcode_) = new OpcodesSCASW(); result->opcode_ = (yyval.opcode_); }
#line 3814 "parser.cc"
    break;

  case 299: /* Opcode: _KW_SETA  */
#line 745 "nask.y"
             { (yyval.opcode_) = new OpcodesSETA(); result->opcode_ = (yyval.opcode_); }
#line 3820 "parser.cc"
    break;

  case 300: /* Opcode: _KW_SETAE  */
#line 746 "nask.y"
              { (yyval.opcode_) = new OpcodesSETAE(); result->opcode_ = (yyval.opcode_); }
#line 3826 "parser.cc"
    break;

  case 301: /* Opcode: _KW_SETB  */
#line 747 "nask.y"
             { (yyval.opcode_) = new OpcodesSETB(); result->opcode_ = (yyval.opcode_); }
#line 3832 "parser.cc"
    break;

  case 302: /* Opcode: _KW_SETBE  */
#line 748 "nask.y"
              { (yyval.opcode_) = new OpcodesSETBE(); result->opcode_ = (yyval.opcode_); }
#line 3838 "parser.cc"
    break;

  case 303: /* Opcode: _KW_SETC  */
#line 749 "nask.y"
             { (yyval.opcode_) = new OpcodesSETC(); result->opcode_ = (yyval.opcode_); }
#line 3844 "parser.cc"
    break;

  case 304: /* Opcode: _KW_SETE  */
#line 750 "nask.y"
             { (yyval.opcode_) = new OpcodesSETE(); result->opcode_ = (yyval.opcode_); }
#line 3850 "parser.cc"
    break;

  case 305: /* Opcode: _KW_SETG  */
#line 751 "nask.y"
             { (yyval.opcode_) = new OpcodesSETG(); result->opcode_ = (yyval.opcode_); }
#line 3856 "parser.cc"
    break;

  case 306: /* Opcode: _KW_SETGE  */
#line 752 "nask.y"
              { (yyval.opcode_) = new OpcodesSETGE(); result->opcode_ = (yyval.opcode_); }
#line 3862 "parser.cc"
    break;

  case 307: /* Opcode: _KW_SETL  */
#line 753 "nask.y"
             { (yyval.opcode_) = new OpcodesSETL(); result->opcode_ = (yyval.opcode_); }
#line 3868 "parser.cc"
    break;

  case 308: /* Opcode: _KW_SETLE  */
#line 754 "nask.y"
              { (yyval.opcode_) = new OpcodesSETLE(); result->opcode_ = (yyval.opcode_); }
#line 3874 "parser.cc"
    break;

  case 309: /* Opcode: _KW_SETNA  */
#line 755 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNA(); result->opcode_ = (yyval.opcode_); }
#line 3880 "parser.cc"
    break;

  case 310: /* Opcode: _KW_SETNAE  */
#line 756 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNAE(); result->opcode_ = (yyval.opcode_); }
#line 3886 "parser.cc"
    break;

  case 311: /* Opcode: _KW_SETNB  */
#line 757 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNB(); result->opcode_ = (yyval.opcode_); }
#line 3892 "parser.cc"
    break;

  case 312: /* Opcode: _KW_SETNBE  */
#line 758 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNBE(); result->opcode_ = (yyval.opcode_); }
#line 3898 "parser.cc"
    break;

  case 313: /* Opcode: _KW_SETNC  */
#line 759 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNC(); result->opcode_ = (yyval.opcode_); }
#line 3904 "parser.cc"
    break;

  case 314: /* Opcode: _KW_SETNE  */
#line 760 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNE(); result->opcode_ = (yyval.opcode_); }
#line 3910 "parser.cc"
    break;

  case 315: /* Opcode: _KW_SETNG  */
#line 761 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNG(); result->opcode_ = (yyval.opcode_); }
#line 3916 "parser.cc"
    break;

  case 316: /* Opcode: _KW_SETNGE  */
#line 762 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNGE(); result->opcode_ = (yyval.opcode_); }
#line 3922 "parser.cc"
    break;

  case 317: /* Opcode: _KW_SETNL  */
#line 763 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNL(); result->opcode_ = (yyval.opcode_); }
#line 3928 "parser.cc"
    break;

  case 318: /* Opcode: _KW_SETNLE  */
#line 764 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNLE(); result->opcode_ = (yyval.opcode_); }
#line 3934 "parser.cc"
    break;

  case 319: /* Opcode: _KW_SETNO  */
#line 765 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNO(); result->opcode_ = (yyval.opcode_); }
#line 3940 "parser.cc"
    break;

  case 320: /* Opcode: _KW_SETNP  */
#line 766 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNP(); result->opcode_ = (yyval.opcode_); }
#line 3946 "parser.cc"
    break;

  case 321: /* Opcode: _KW_SETNS  */
#line 767 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNS(); result->opcode_ = (yyval.opcode_); }
#line 3952 "parser.cc"
    break;

  case 322: /* Opcode: _KW_SETNZ  */
#line 768 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNZ(); result->opcode_ = (yyval.opcode_); }
#line 3958 "parser.cc"
    break;

  case 323: /* Opcode: _KW_SETO  */
#line 769 "nask.y"
             { (yyval.opcode_) = new OpcodesSETO(); result->opcode_ = (yyval.opcode_); }
#line 3964 "parser.cc"
    break;

  case 324: /* Opcode: _KW_SETP  */
#line 770 "nask.y"
             { (yyval.opcode_) = new OpcodesSETP(); result->opcode_ = (yyval.opcode_); }
#line 3970 "parser.cc"
    break;

  case 325: /* Opcode: _KW_SETPE  */
#line 771 "nask.y"
              { (yyval.opcode_) = new OpcodesSETPE(); result->opcode_ = (yyval.opcode_); }
#line 3976 "parser.cc"
    break;

  case 326: /* Opcode: _KW_SETPO  */
#line 772 "nask.y"
              { (yyval.opcode_) = new OpcodesSETPO(); result->opcode_ = (yyval.opcode_); }
#line 3982 "parser.cc"
    break;

  case 327: /* Opcode: _KW_SETS  */
#line 773 "nask.y"
             { (yyval.opcode_) = new OpcodesSETS(); result->opcode_ = (yyval.opcode_); }
#line 3988 "parser.cc"
    break;

  case 328: /* Opcode: _KW_SETZ  */
#line 774 "nask.y"
             { (yyval.opcode_) = new OpcodesSETZ(); result->opcode_ = (yyval.opcode_); }
#line 3994 "parser.cc"
    break;

  case 329: /* Opcode: _KW_SGDT  */
#line 775 "nask.y"
             { (yyval.opcode_) = new OpcodesSGDT(); result->opcode_ = (yyval.opcode_); }
#line 4000 "parser.cc"
    break;

  case 330: /* Opcode: _KW_SHL  */
#line 776 "nask.y"
            { (yyval.opcode_) = new OpcodesSHL(); result->opcode_ = (yyval.opcode_); }
#line 4006 "parser.cc"
    break;

  case 331: /* Opcode: _KW_SHLD  */
#line 777 "nask.y"
             { (yyval.opcode_) = new OpcodesSHLD(); result->opcode_ = (yyval.opcode_); }
#line 4012 "parser.cc"
    break;

  case 332: /* Opcode: _KW_SHR  */
#line 778 "nask.y"
            { (yyval.opcode_) = new OpcodesSHR(); result->opcode_ = (yyval.opcode_); }
#line 4018 "parser.cc"
    break;

  case 333: /* Opcode: _KW_SHRD  */
#line 779 "nask.y"
             { (yyval.opcode_) = new OpcodesSHRD(); result->opcode_ = (yyval.opcode_); }
#line 4024 "parser.cc"
    break;

  case 334: /* Opcode: _KW_SIDT  */
#line 780 "nask.y"
             { (yyval.opcode_) = new OpcodesSIDT(); result->opcode_ = (yyval.opcode_); }
#line 4030 "parser.cc"
    break;

  case 335: /* Opcode: _KW_SLDT  */
#line 781 "nask.y"
             { (yyval.opcode_) = new OpcodesSLDT(); result->opcode_ = (yyval.opcode_); }
#line 4036 "parser.cc"
    break;

  case 336: /* Opcode: _KW_SMSW  */
#line 782 "nask.y"
             { (yyval.opcode_) = new OpcodesSMSW(); result->opcode_ = (yyval.opcode_); }
#line 4042 "parser.cc"
    break;

  case 337: /* Opcode: _KW_STC  */
#line 783 "nask.y"
            { (yyval.opcode_) = new OpcodesSTC(); result->opcode_ = (yyval.opcode_); }
#line 4048 "parser.cc"
    break;

  case 338: /* Opcode: _KW_STD  */
#line 784 "nask.y"
            { (yyval.opcode_) = new OpcodesSTD(); result->opcode_ = (yyval.opcode_); }
#line 4054 "parser.cc"
    break;

  case 339: /* Opcode: _KW_STI  */
#line 785 "nask.y"
            { (yyval.opcode_) = new OpcodesSTI(); result->opcode_ = (yyval.opcode_); }
#line 4060 "parser.cc"
    break;

  case 340: /* Opcode: _KW_STOSB  */
#line 786 "nask.y"
              { (yyval.opcode_) = new OpcodesSTOSB(); result->opcode_ = (yyval.opcode_); }
#line 4066 "parser.cc"
    break;

  case 341: /* Opcode: _KW_STOSD  */
#line 787 "nask.y"
              { (yyval.opcode_) = new OpcodesSTOSD(); result->opcode_ = (yyval.opcode_); }
#line 4072 "parser.cc"
    break;

  case 342: /* Opcode: _KW_STOSW  */
#line 788 "nask.y"
              { (yyval.opcode_) = new OpcodesSTOSW(); result->opcode_ = (yyval.opcode_); }
#line 4078 "parser.cc"
    break;

  case 343: /* Opcode: _KW_STR  */
#line 789 "nask.y"
            { (yyval.opcode_) = new OpcodesSTR(); result->opcode_ = (yyval.opcode_); }
#line 4084 "parser.cc"
    break;

  case 344: /* Opcode: _KW_SUB  */
#line 790 "nask.y"
            { (yyval.opcode_) = new OpcodesSUB(); result->opcode_ = (yyval.opcode_); }
#line 4090 "parser.cc"
    break;

  case 345: /* Opcode: _KW_TEST  */
#line 791 "nask.y"
             { (yyval.opcode_) = new OpcodesTEST(); result->opcode_ = (yyval.opcode_); }
#line 4096 "parser.cc"
    break;

  case 346: /* Opcode: _KW_TIMES  */
#line 792 "nask.y"
              { (yyval.opcode_) = new OpcodesTIMES(); result->opcode_ = (yyval.opcode_); }
#line 4102 "parser.cc"
    break;

  case 347: /* Opcode: _KW_UD2  */
#line 793 "nask.y"
            { (yyval.opcode_) = new OpcodesUD2(); result->opcode_ = (yyval.opcode_); }
#line 4108 "parser.cc"
    break;

  case 348: /* Opcode: _KW_VERR  */
#line 794 "nask.y"
             { (yyval.opcode_) = new OpcodesVERR(); result->opcode_ = (yyval.opcode_); }
#line 4114 "parser.cc"
    break;

  case 349: /* Opcode: _KW_VERW  */
#line 795 "nask.y"
             { (yyval.opcode_) = new OpcodesVERW(); result->opcode_ = (yyval.opcode_); }
#line 4120 "parser.cc"
    break;

  case 350: /* Opcode: _KW_WAIT  */
#line 796 "nask.y"
             { (yyval.opcode_) = new OpcodesWAIT(); result->opcode_ = (yyval.opcode_); }
#line 4126 "parser.cc"
    break;

  case 351: /* Opcode: _KW_WBINVD  */
#line 797 "nask.y"
               { (yyval.opcode_) = new OpcodesWBINVD(); result->opcode_ = (yyval.opcode_); }
#line 4132 "parser.cc"
    break;

  case 352: /* Opcode: _KW_WRMSR  */
#line 798 "nask.y"
              { (yyval.opcode_) = new OpcodesWRMSR(); result->opcode_ = (yyval.opcode_); }
#line 4138 "parser.cc"
    break;

  case 353: /* Opcode: _KW_XADD  */
#line 799 "nask.y"
             { (yyval.opcode_) = new OpcodesXADD(); result->opcode_ = (yyval.opcode_); }
#line 4144 "parser.cc"
    break;

  case 354: /* Opcode: _KW_XCHG  */
#line 800 "nask.y"
             { (yyval.opcode_) = new OpcodesXCHG(); result->opcode_ = (yyval.opcode_); }
#line 4150 "parser.cc"
    break;

  case 355: /* Opcode: _KW_XLATB  */
#line 801 "nask.y"
              { (yyval.opcode_) = new OpcodesXLATB(); result->opcode_ = (yyval.opcode_); }
#line 4156 "parser.cc"
    break;

  case 356: /* Opcode: _KW_XOR  */
#line 802 "nask.y"
            { (yyval.opcode_) = new OpcodesXOR(); result->opcode_ = (yyval.opcode_); }
#line 4162 "parser.cc"
    break;


#line 4166 "parser.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (&yylloc, scanner, result, YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner, result);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, scanner, result);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, result, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner, result);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, scanner, result);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 805 "nask.y"



/* Entrypoint: parse Program* from file. */
Program* pProgram(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.program_;
  }
}

/* Entrypoint: parse Program* from string. */
Program* psProgram(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.program_;
  }
}

/* Entrypoint: parse ListStatement* from file. */
ListStatement* pListStatement(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
std::reverse(result.liststatement_->begin(), result.liststatement_->end());
    return result.liststatement_;
  }
}

/* Entrypoint: parse ListStatement* from string. */
ListStatement* psListStatement(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
std::reverse(result.liststatement_->begin(), result.liststatement_->end());
    return result.liststatement_;
  }
}

/* Entrypoint: parse Statement* from file. */
Statement* pStatement(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.statement_;
  }
}

/* Entrypoint: parse Statement* from string. */
Statement* psStatement(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.statement_;
  }
}

/* Entrypoint: parse ListMnemonicArgs* from file. */
ListMnemonicArgs* pListMnemonicArgs(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
std::reverse(result.listmnemonicargs_->begin(), result.listmnemonicargs_->end());
    return result.listmnemonicargs_;
  }
}

/* Entrypoint: parse ListMnemonicArgs* from string. */
ListMnemonicArgs* psListMnemonicArgs(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
std::reverse(result.listmnemonicargs_->begin(), result.listmnemonicargs_->end());
    return result.listmnemonicargs_;
  }
}

/* Entrypoint: parse MnemonicArgs* from file. */
MnemonicArgs* pMnemonicArgs(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.mnemonicargs_;
  }
}

/* Entrypoint: parse MnemonicArgs* from string. */
MnemonicArgs* psMnemonicArgs(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.mnemonicargs_;
  }
}

/* Entrypoint: parse Exp* from file. */
Exp* pExp(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.exp_;
  }
}

/* Entrypoint: parse Exp* from string. */
Exp* psExp(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.exp_;
  }
}

/* Entrypoint: parse Factor* from file. */
Factor* pFactor(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.factor_;
  }
}

/* Entrypoint: parse Factor* from string. */
Factor* psFactor(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.factor_;
  }
}

/* Entrypoint: parse ConfigType* from file. */
ConfigType* pConfigType(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.configtype_;
  }
}

/* Entrypoint: parse ConfigType* from string. */
ConfigType* psConfigType(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.configtype_;
  }
}

/* Entrypoint: parse Opcode* from file. */
Opcode* pOpcode(FILE *inp)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(inp);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  int error = yyparse(scanner, &result);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.opcode_;
  }
}

/* Entrypoint: parse Opcode* from string. */
Opcode* psOpcode(const char *str)
{
  YYSTYPE result;
  yyscan_t scanner = nask__initialize_lexer(0);
  if (!scanner) {
    fprintf(stderr, "Failed to initialize lexer.\n");
    return 0;
  }
  YY_BUFFER_STATE buf = nask__scan_string(str, scanner);
  int error = yyparse(scanner, &result);
  nask__delete_buffer(buf, scanner);
  nask_lex_destroy(scanner);
  if (error)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return result.opcode_;
  }
}



