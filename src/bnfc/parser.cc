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
  YYSYMBOL__KW_BYTE = 37,                  /* _KW_BYTE  */
  YYSYMBOL__KW_CALL = 38,                  /* _KW_CALL  */
  YYSYMBOL__KW_CBW = 39,                   /* _KW_CBW  */
  YYSYMBOL__KW_CDQ = 40,                   /* _KW_CDQ  */
  YYSYMBOL__KW_CLC = 41,                   /* _KW_CLC  */
  YYSYMBOL__KW_CLD = 42,                   /* _KW_CLD  */
  YYSYMBOL__KW_CLI = 43,                   /* _KW_CLI  */
  YYSYMBOL__KW_CLTS = 44,                  /* _KW_CLTS  */
  YYSYMBOL__KW_CMC = 45,                   /* _KW_CMC  */
  YYSYMBOL__KW_CMP = 46,                   /* _KW_CMP  */
  YYSYMBOL__KW_CMPSB = 47,                 /* _KW_CMPSB  */
  YYSYMBOL__KW_CMPSD = 48,                 /* _KW_CMPSD  */
  YYSYMBOL__KW_CMPSW = 49,                 /* _KW_CMPSW  */
  YYSYMBOL__KW_CMPXCHG = 50,               /* _KW_CMPXCHG  */
  YYSYMBOL__KW_CPUID = 51,                 /* _KW_CPUID  */
  YYSYMBOL__KW_CWD = 52,                   /* _KW_CWD  */
  YYSYMBOL__KW_CWDE = 53,                  /* _KW_CWDE  */
  YYSYMBOL__KW_DAA = 54,                   /* _KW_DAA  */
  YYSYMBOL__KW_DAS = 55,                   /* _KW_DAS  */
  YYSYMBOL__KW_DB = 56,                    /* _KW_DB  */
  YYSYMBOL__KW_DD = 57,                    /* _KW_DD  */
  YYSYMBOL__KW_DEC = 58,                   /* _KW_DEC  */
  YYSYMBOL__KW_DIV = 59,                   /* _KW_DIV  */
  YYSYMBOL__KW_DQ = 60,                    /* _KW_DQ  */
  YYSYMBOL__KW_DT = 61,                    /* _KW_DT  */
  YYSYMBOL__KW_DW = 62,                    /* _KW_DW  */
  YYSYMBOL__KW_DWORD = 63,                 /* _KW_DWORD  */
  YYSYMBOL__KW_END = 64,                   /* _KW_END  */
  YYSYMBOL__KW_ENTER = 65,                 /* _KW_ENTER  */
  YYSYMBOL__KW_EQU = 66,                   /* _KW_EQU  */
  YYSYMBOL__KW_EXTERN = 67,                /* _KW_EXTERN  */
  YYSYMBOL__KW_F2XM1 = 68,                 /* _KW_F2XM1  */
  YYSYMBOL__KW_FABS = 69,                  /* _KW_FABS  */
  YYSYMBOL__KW_FADD = 70,                  /* _KW_FADD  */
  YYSYMBOL__KW_FADDP = 71,                 /* _KW_FADDP  */
  YYSYMBOL__KW_FBLD = 72,                  /* _KW_FBLD  */
  YYSYMBOL__KW_FBSTP = 73,                 /* _KW_FBSTP  */
  YYSYMBOL__KW_FCHS = 74,                  /* _KW_FCHS  */
  YYSYMBOL__KW_FCLEX = 75,                 /* _KW_FCLEX  */
  YYSYMBOL__KW_FCOM = 76,                  /* _KW_FCOM  */
  YYSYMBOL__KW_FCOMP = 77,                 /* _KW_FCOMP  */
  YYSYMBOL__KW_FCOMPP = 78,                /* _KW_FCOMPP  */
  YYSYMBOL__KW_FCOS = 79,                  /* _KW_FCOS  */
  YYSYMBOL__KW_FDECSTP = 80,               /* _KW_FDECSTP  */
  YYSYMBOL__KW_FDISI = 81,                 /* _KW_FDISI  */
  YYSYMBOL__KW_FDIV = 82,                  /* _KW_FDIV  */
  YYSYMBOL__KW_FDIVP = 83,                 /* _KW_FDIVP  */
  YYSYMBOL__KW_FDIVR = 84,                 /* _KW_FDIVR  */
  YYSYMBOL__KW_FDIVRP = 85,                /* _KW_FDIVRP  */
  YYSYMBOL__KW_FENI = 86,                  /* _KW_FENI  */
  YYSYMBOL__KW_FFREE = 87,                 /* _KW_FFREE  */
  YYSYMBOL__KW_FIADD = 88,                 /* _KW_FIADD  */
  YYSYMBOL__KW_FICOM = 89,                 /* _KW_FICOM  */
  YYSYMBOL__KW_FICOMP = 90,                /* _KW_FICOMP  */
  YYSYMBOL__KW_FIDIV = 91,                 /* _KW_FIDIV  */
  YYSYMBOL__KW_FIDIVR = 92,                /* _KW_FIDIVR  */
  YYSYMBOL__KW_FILD = 93,                  /* _KW_FILD  */
  YYSYMBOL__KW_FILE = 94,                  /* _KW_FILE  */
  YYSYMBOL__KW_FIMUL = 95,                 /* _KW_FIMUL  */
  YYSYMBOL__KW_FINCSTP = 96,               /* _KW_FINCSTP  */
  YYSYMBOL__KW_FINIT = 97,                 /* _KW_FINIT  */
  YYSYMBOL__KW_FIST = 98,                  /* _KW_FIST  */
  YYSYMBOL__KW_FISTP = 99,                 /* _KW_FISTP  */
  YYSYMBOL__KW_FISUB = 100,                /* _KW_FISUB  */
  YYSYMBOL__KW_FISUBR = 101,               /* _KW_FISUBR  */
  YYSYMBOL__KW_FLD = 102,                  /* _KW_FLD  */
  YYSYMBOL__KW_FLD1 = 103,                 /* _KW_FLD1  */
  YYSYMBOL__KW_FLDCW = 104,                /* _KW_FLDCW  */
  YYSYMBOL__KW_FLDENV = 105,               /* _KW_FLDENV  */
  YYSYMBOL__KW_FLDL2E = 106,               /* _KW_FLDL2E  */
  YYSYMBOL__KW_FLDL2T = 107,               /* _KW_FLDL2T  */
  YYSYMBOL__KW_FLDLG2 = 108,               /* _KW_FLDLG2  */
  YYSYMBOL__KW_FLDLN2 = 109,               /* _KW_FLDLN2  */
  YYSYMBOL__KW_FLDPI = 110,                /* _KW_FLDPI  */
  YYSYMBOL__KW_FLDZ = 111,                 /* _KW_FLDZ  */
  YYSYMBOL__KW_FMUL = 112,                 /* _KW_FMUL  */
  YYSYMBOL__KW_FMULP = 113,                /* _KW_FMULP  */
  YYSYMBOL__KW_FNCLEX = 114,               /* _KW_FNCLEX  */
  YYSYMBOL__KW_FNDISI = 115,               /* _KW_FNDISI  */
  YYSYMBOL__KW_FNENI = 116,                /* _KW_FNENI  */
  YYSYMBOL__KW_FNINIT = 117,               /* _KW_FNINIT  */
  YYSYMBOL__KW_FNOP = 118,                 /* _KW_FNOP  */
  YYSYMBOL__KW_FNSAVE = 119,               /* _KW_FNSAVE  */
  YYSYMBOL__KW_FNSTCW = 120,               /* _KW_FNSTCW  */
  YYSYMBOL__KW_FNSTENV = 121,              /* _KW_FNSTENV  */
  YYSYMBOL__KW_FNSTSW = 122,               /* _KW_FNSTSW  */
  YYSYMBOL__KW_FORMAT = 123,               /* _KW_FORMAT  */
  YYSYMBOL__KW_FPATAN = 124,               /* _KW_FPATAN  */
  YYSYMBOL__KW_FPREM = 125,                /* _KW_FPREM  */
  YYSYMBOL__KW_FPREM1 = 126,               /* _KW_FPREM1  */
  YYSYMBOL__KW_FPTAN = 127,                /* _KW_FPTAN  */
  YYSYMBOL__KW_FRNDINT = 128,              /* _KW_FRNDINT  */
  YYSYMBOL__KW_FRSTOR = 129,               /* _KW_FRSTOR  */
  YYSYMBOL__KW_FSAVE = 130,                /* _KW_FSAVE  */
  YYSYMBOL__KW_FSCALE = 131,               /* _KW_FSCALE  */
  YYSYMBOL__KW_FSETPM = 132,               /* _KW_FSETPM  */
  YYSYMBOL__KW_FSIN = 133,                 /* _KW_FSIN  */
  YYSYMBOL__KW_FSINCOS = 134,              /* _KW_FSINCOS  */
  YYSYMBOL__KW_FSQRT = 135,                /* _KW_FSQRT  */
  YYSYMBOL__KW_FST = 136,                  /* _KW_FST  */
  YYSYMBOL__KW_FSTCW = 137,                /* _KW_FSTCW  */
  YYSYMBOL__KW_FSTENV = 138,               /* _KW_FSTENV  */
  YYSYMBOL__KW_FSTP = 139,                 /* _KW_FSTP  */
  YYSYMBOL__KW_FSTSW = 140,                /* _KW_FSTSW  */
  YYSYMBOL__KW_FSUB = 141,                 /* _KW_FSUB  */
  YYSYMBOL__KW_FSUBP = 142,                /* _KW_FSUBP  */
  YYSYMBOL__KW_FSUBR = 143,                /* _KW_FSUBR  */
  YYSYMBOL__KW_FSUBRP = 144,               /* _KW_FSUBRP  */
  YYSYMBOL__KW_FTST = 145,                 /* _KW_FTST  */
  YYSYMBOL__KW_FUCOM = 146,                /* _KW_FUCOM  */
  YYSYMBOL__KW_FUCOMP = 147,               /* _KW_FUCOMP  */
  YYSYMBOL__KW_FUCOMPP = 148,              /* _KW_FUCOMPP  */
  YYSYMBOL__KW_FXAM = 149,                 /* _KW_FXAM  */
  YYSYMBOL__KW_FXCH = 150,                 /* _KW_FXCH  */
  YYSYMBOL__KW_FXTRACT = 151,              /* _KW_FXTRACT  */
  YYSYMBOL__KW_FYL2X = 152,                /* _KW_FYL2X  */
  YYSYMBOL__KW_FYL2XP1 = 153,              /* _KW_FYL2XP1  */
  YYSYMBOL__KW_HLT = 154,                  /* _KW_HLT  */
  YYSYMBOL__KW_IDIV = 155,                 /* _KW_IDIV  */
  YYSYMBOL__KW_IMUL = 156,                 /* _KW_IMUL  */
  YYSYMBOL__KW_IN = 157,                   /* _KW_IN  */
  YYSYMBOL__KW_INC = 158,                  /* _KW_INC  */
  YYSYMBOL__KW_INCO = 159,                 /* _KW_INCO  */
  YYSYMBOL__KW_INSB = 160,                 /* _KW_INSB  */
  YYSYMBOL__KW_INSD = 161,                 /* _KW_INSD  */
  YYSYMBOL__KW_INSTRSET = 162,             /* _KW_INSTRSET  */
  YYSYMBOL__KW_INSW = 163,                 /* _KW_INSW  */
  YYSYMBOL__KW_INT = 164,                  /* _KW_INT  */
  YYSYMBOL__KW_INT3 = 165,                 /* _KW_INT3  */
  YYSYMBOL__KW_INTO = 166,                 /* _KW_INTO  */
  YYSYMBOL__KW_INVD = 167,                 /* _KW_INVD  */
  YYSYMBOL__KW_INVLPG = 168,               /* _KW_INVLPG  */
  YYSYMBOL__KW_IRET = 169,                 /* _KW_IRET  */
  YYSYMBOL__KW_IRETD = 170,                /* _KW_IRETD  */
  YYSYMBOL__KW_IRETW = 171,                /* _KW_IRETW  */
  YYSYMBOL__KW_JA = 172,                   /* _KW_JA  */
  YYSYMBOL__KW_JAE = 173,                  /* _KW_JAE  */
  YYSYMBOL__KW_JB = 174,                   /* _KW_JB  */
  YYSYMBOL__KW_JBE = 175,                  /* _KW_JBE  */
  YYSYMBOL__KW_JC = 176,                   /* _KW_JC  */
  YYSYMBOL__KW_JCXZ = 177,                 /* _KW_JCXZ  */
  YYSYMBOL__KW_JE = 178,                   /* _KW_JE  */
  YYSYMBOL__KW_JECXZ = 179,                /* _KW_JECXZ  */
  YYSYMBOL__KW_JG = 180,                   /* _KW_JG  */
  YYSYMBOL__KW_JGE = 181,                  /* _KW_JGE  */
  YYSYMBOL__KW_JL = 182,                   /* _KW_JL  */
  YYSYMBOL__KW_JLE = 183,                  /* _KW_JLE  */
  YYSYMBOL__KW_JMP = 184,                  /* _KW_JMP  */
  YYSYMBOL__KW_JNA = 185,                  /* _KW_JNA  */
  YYSYMBOL__KW_JNAE = 186,                 /* _KW_JNAE  */
  YYSYMBOL__KW_JNB = 187,                  /* _KW_JNB  */
  YYSYMBOL__KW_JNBE = 188,                 /* _KW_JNBE  */
  YYSYMBOL__KW_JNC = 189,                  /* _KW_JNC  */
  YYSYMBOL__KW_JNE = 190,                  /* _KW_JNE  */
  YYSYMBOL__KW_JNG = 191,                  /* _KW_JNG  */
  YYSYMBOL__KW_JNGE = 192,                 /* _KW_JNGE  */
  YYSYMBOL__KW_JNL = 193,                  /* _KW_JNL  */
  YYSYMBOL__KW_JNLE = 194,                 /* _KW_JNLE  */
  YYSYMBOL__KW_JNO = 195,                  /* _KW_JNO  */
  YYSYMBOL__KW_JNP = 196,                  /* _KW_JNP  */
  YYSYMBOL__KW_JNS = 197,                  /* _KW_JNS  */
  YYSYMBOL__KW_JNZ = 198,                  /* _KW_JNZ  */
  YYSYMBOL__KW_JO = 199,                   /* _KW_JO  */
  YYSYMBOL__KW_JP = 200,                   /* _KW_JP  */
  YYSYMBOL__KW_JPE = 201,                  /* _KW_JPE  */
  YYSYMBOL__KW_JPO = 202,                  /* _KW_JPO  */
  YYSYMBOL__KW_JS = 203,                   /* _KW_JS  */
  YYSYMBOL__KW_JZ = 204,                   /* _KW_JZ  */
  YYSYMBOL__KW_LAHF = 205,                 /* _KW_LAHF  */
  YYSYMBOL__KW_LAR = 206,                  /* _KW_LAR  */
  YYSYMBOL__KW_LDS = 207,                  /* _KW_LDS  */
  YYSYMBOL__KW_LEA = 208,                  /* _KW_LEA  */
  YYSYMBOL__KW_LEAVE = 209,                /* _KW_LEAVE  */
  YYSYMBOL__KW_LES = 210,                  /* _KW_LES  */
  YYSYMBOL__KW_LFS = 211,                  /* _KW_LFS  */
  YYSYMBOL__KW_LGDT = 212,                 /* _KW_LGDT  */
  YYSYMBOL__KW_LGS = 213,                  /* _KW_LGS  */
  YYSYMBOL__KW_LIDT = 214,                 /* _KW_LIDT  */
  YYSYMBOL__KW_LLDT = 215,                 /* _KW_LLDT  */
  YYSYMBOL__KW_LMSW = 216,                 /* _KW_LMSW  */
  YYSYMBOL__KW_LOCK = 217,                 /* _KW_LOCK  */
  YYSYMBOL__KW_LODSB = 218,                /* _KW_LODSB  */
  YYSYMBOL__KW_LODSD = 219,                /* _KW_LODSD  */
  YYSYMBOL__KW_LODSW = 220,                /* _KW_LODSW  */
  YYSYMBOL__KW_LOOP = 221,                 /* _KW_LOOP  */
  YYSYMBOL__KW_LOOPE = 222,                /* _KW_LOOPE  */
  YYSYMBOL__KW_LOOPNE = 223,               /* _KW_LOOPNE  */
  YYSYMBOL__KW_LOOPNZ = 224,               /* _KW_LOOPNZ  */
  YYSYMBOL__KW_LOOPZ = 225,                /* _KW_LOOPZ  */
  YYSYMBOL__KW_LSL = 226,                  /* _KW_LSL  */
  YYSYMBOL__KW_LSS = 227,                  /* _KW_LSS  */
  YYSYMBOL__KW_LTR = 228,                  /* _KW_LTR  */
  YYSYMBOL__KW_MOV = 229,                  /* _KW_MOV  */
  YYSYMBOL__KW_MOVSB = 230,                /* _KW_MOVSB  */
  YYSYMBOL__KW_MOVSD = 231,                /* _KW_MOVSD  */
  YYSYMBOL__KW_MOVSW = 232,                /* _KW_MOVSW  */
  YYSYMBOL__KW_MOVSX = 233,                /* _KW_MOVSX  */
  YYSYMBOL__KW_MOVZX = 234,                /* _KW_MOVZX  */
  YYSYMBOL__KW_MUL = 235,                  /* _KW_MUL  */
  YYSYMBOL__KW_NEG = 236,                  /* _KW_NEG  */
  YYSYMBOL__KW_NOP = 237,                  /* _KW_NOP  */
  YYSYMBOL__KW_NOT = 238,                  /* _KW_NOT  */
  YYSYMBOL__KW_OPTIMIZE = 239,             /* _KW_OPTIMIZE  */
  YYSYMBOL__KW_OR = 240,                   /* _KW_OR  */
  YYSYMBOL__KW_ORG = 241,                  /* _KW_ORG  */
  YYSYMBOL__KW_OUT = 242,                  /* _KW_OUT  */
  YYSYMBOL__KW_OUTSB = 243,                /* _KW_OUTSB  */
  YYSYMBOL__KW_OUTSD = 244,                /* _KW_OUTSD  */
  YYSYMBOL__KW_OUTSW = 245,                /* _KW_OUTSW  */
  YYSYMBOL__KW_PADDING = 246,              /* _KW_PADDING  */
  YYSYMBOL__KW_PADSET = 247,               /* _KW_PADSET  */
  YYSYMBOL__KW_POP = 248,                  /* _KW_POP  */
  YYSYMBOL__KW_POPA = 249,                 /* _KW_POPA  */
  YYSYMBOL__KW_POPAD = 250,                /* _KW_POPAD  */
  YYSYMBOL__KW_POPAW = 251,                /* _KW_POPAW  */
  YYSYMBOL__KW_POPF = 252,                 /* _KW_POPF  */
  YYSYMBOL__KW_POPFD = 253,                /* _KW_POPFD  */
  YYSYMBOL__KW_POPFW = 254,                /* _KW_POPFW  */
  YYSYMBOL__KW_PUSH = 255,                 /* _KW_PUSH  */
  YYSYMBOL__KW_PUSHA = 256,                /* _KW_PUSHA  */
  YYSYMBOL__KW_PUSHAD = 257,               /* _KW_PUSHAD  */
  YYSYMBOL__KW_PUSHAW = 258,               /* _KW_PUSHAW  */
  YYSYMBOL__KW_PUSHD = 259,                /* _KW_PUSHD  */
  YYSYMBOL__KW_PUSHF = 260,                /* _KW_PUSHF  */
  YYSYMBOL__KW_PUSHFD = 261,               /* _KW_PUSHFD  */
  YYSYMBOL__KW_PUSHFW = 262,               /* _KW_PUSHFW  */
  YYSYMBOL__KW_RCL = 263,                  /* _KW_RCL  */
  YYSYMBOL__KW_RCR = 264,                  /* _KW_RCR  */
  YYSYMBOL__KW_RDMSR = 265,                /* _KW_RDMSR  */
  YYSYMBOL__KW_RDPMC = 266,                /* _KW_RDPMC  */
  YYSYMBOL__KW_REP = 267,                  /* _KW_REP  */
  YYSYMBOL__KW_REPE = 268,                 /* _KW_REPE  */
  YYSYMBOL__KW_REPNE = 269,                /* _KW_REPNE  */
  YYSYMBOL__KW_REPNZ = 270,                /* _KW_REPNZ  */
  YYSYMBOL__KW_REPZ = 271,                 /* _KW_REPZ  */
  YYSYMBOL__KW_RESB = 272,                 /* _KW_RESB  */
  YYSYMBOL__KW_RESD = 273,                 /* _KW_RESD  */
  YYSYMBOL__KW_RESQ = 274,                 /* _KW_RESQ  */
  YYSYMBOL__KW_REST = 275,                 /* _KW_REST  */
  YYSYMBOL__KW_RESW = 276,                 /* _KW_RESW  */
  YYSYMBOL__KW_RET = 277,                  /* _KW_RET  */
  YYSYMBOL__KW_RETF = 278,                 /* _KW_RETF  */
  YYSYMBOL__KW_RETN = 279,                 /* _KW_RETN  */
  YYSYMBOL__KW_ROL = 280,                  /* _KW_ROL  */
  YYSYMBOL__KW_ROR = 281,                  /* _KW_ROR  */
  YYSYMBOL__KW_RSM = 282,                  /* _KW_RSM  */
  YYSYMBOL__KW_SAHF = 283,                 /* _KW_SAHF  */
  YYSYMBOL__KW_SAL = 284,                  /* _KW_SAL  */
  YYSYMBOL__KW_SAR = 285,                  /* _KW_SAR  */
  YYSYMBOL__KW_SBB = 286,                  /* _KW_SBB  */
  YYSYMBOL__KW_SCASB = 287,                /* _KW_SCASB  */
  YYSYMBOL__KW_SCASD = 288,                /* _KW_SCASD  */
  YYSYMBOL__KW_SCASW = 289,                /* _KW_SCASW  */
  YYSYMBOL__KW_SECTION = 290,              /* _KW_SECTION  */
  YYSYMBOL__KW_SETA = 291,                 /* _KW_SETA  */
  YYSYMBOL__KW_SETAE = 292,                /* _KW_SETAE  */
  YYSYMBOL__KW_SETB = 293,                 /* _KW_SETB  */
  YYSYMBOL__KW_SETBE = 294,                /* _KW_SETBE  */
  YYSYMBOL__KW_SETC = 295,                 /* _KW_SETC  */
  YYSYMBOL__KW_SETE = 296,                 /* _KW_SETE  */
  YYSYMBOL__KW_SETG = 297,                 /* _KW_SETG  */
  YYSYMBOL__KW_SETGE = 298,                /* _KW_SETGE  */
  YYSYMBOL__KW_SETL = 299,                 /* _KW_SETL  */
  YYSYMBOL__KW_SETLE = 300,                /* _KW_SETLE  */
  YYSYMBOL__KW_SETNA = 301,                /* _KW_SETNA  */
  YYSYMBOL__KW_SETNAE = 302,               /* _KW_SETNAE  */
  YYSYMBOL__KW_SETNB = 303,                /* _KW_SETNB  */
  YYSYMBOL__KW_SETNBE = 304,               /* _KW_SETNBE  */
  YYSYMBOL__KW_SETNC = 305,                /* _KW_SETNC  */
  YYSYMBOL__KW_SETNE = 306,                /* _KW_SETNE  */
  YYSYMBOL__KW_SETNG = 307,                /* _KW_SETNG  */
  YYSYMBOL__KW_SETNGE = 308,               /* _KW_SETNGE  */
  YYSYMBOL__KW_SETNL = 309,                /* _KW_SETNL  */
  YYSYMBOL__KW_SETNLE = 310,               /* _KW_SETNLE  */
  YYSYMBOL__KW_SETNO = 311,                /* _KW_SETNO  */
  YYSYMBOL__KW_SETNP = 312,                /* _KW_SETNP  */
  YYSYMBOL__KW_SETNS = 313,                /* _KW_SETNS  */
  YYSYMBOL__KW_SETNZ = 314,                /* _KW_SETNZ  */
  YYSYMBOL__KW_SETO = 315,                 /* _KW_SETO  */
  YYSYMBOL__KW_SETP = 316,                 /* _KW_SETP  */
  YYSYMBOL__KW_SETPE = 317,                /* _KW_SETPE  */
  YYSYMBOL__KW_SETPO = 318,                /* _KW_SETPO  */
  YYSYMBOL__KW_SETS = 319,                 /* _KW_SETS  */
  YYSYMBOL__KW_SETZ = 320,                 /* _KW_SETZ  */
  YYSYMBOL__KW_SGDT = 321,                 /* _KW_SGDT  */
  YYSYMBOL__KW_SHL = 322,                  /* _KW_SHL  */
  YYSYMBOL__KW_SHLD = 323,                 /* _KW_SHLD  */
  YYSYMBOL__KW_SHR = 324,                  /* _KW_SHR  */
  YYSYMBOL__KW_SHRD = 325,                 /* _KW_SHRD  */
  YYSYMBOL__KW_SIDT = 326,                 /* _KW_SIDT  */
  YYSYMBOL__KW_SLDT = 327,                 /* _KW_SLDT  */
  YYSYMBOL__KW_SMSW = 328,                 /* _KW_SMSW  */
  YYSYMBOL__KW_STC = 329,                  /* _KW_STC  */
  YYSYMBOL__KW_STD = 330,                  /* _KW_STD  */
  YYSYMBOL__KW_STI = 331,                  /* _KW_STI  */
  YYSYMBOL__KW_STOSB = 332,                /* _KW_STOSB  */
  YYSYMBOL__KW_STOSD = 333,                /* _KW_STOSD  */
  YYSYMBOL__KW_STOSW = 334,                /* _KW_STOSW  */
  YYSYMBOL__KW_STR = 335,                  /* _KW_STR  */
  YYSYMBOL__KW_SUB = 336,                  /* _KW_SUB  */
  YYSYMBOL__KW_TEST = 337,                 /* _KW_TEST  */
  YYSYMBOL__KW_TIMES = 338,                /* _KW_TIMES  */
  YYSYMBOL__KW_UD2 = 339,                  /* _KW_UD2  */
  YYSYMBOL__KW_VERR = 340,                 /* _KW_VERR  */
  YYSYMBOL__KW_VERW = 341,                 /* _KW_VERW  */
  YYSYMBOL__KW_WAIT = 342,                 /* _KW_WAIT  */
  YYSYMBOL__KW_WBINVD = 343,               /* _KW_WBINVD  */
  YYSYMBOL__KW_WORD = 344,                 /* _KW_WORD  */
  YYSYMBOL__KW_WRMSR = 345,                /* _KW_WRMSR  */
  YYSYMBOL__KW_XADD = 346,                 /* _KW_XADD  */
  YYSYMBOL__KW_XCHG = 347,                 /* _KW_XCHG  */
  YYSYMBOL__KW_XLATB = 348,                /* _KW_XLATB  */
  YYSYMBOL__KW_XOR = 349,                  /* _KW_XOR  */
  YYSYMBOL__LBRACK = 350,                  /* _LBRACK  */
  YYSYMBOL__RBRACK = 351,                  /* _RBRACK  */
  YYSYMBOL_T_Hex = 352,                    /* T_Hex  */
  YYSYMBOL_T_Label = 353,                  /* T_Label  */
  YYSYMBOL__STRING_ = 354,                 /* _STRING_  */
  YYSYMBOL__INTEGER_ = 355,                /* _INTEGER_  */
  YYSYMBOL__IDENT_ = 356,                  /* _IDENT_  */
  YYSYMBOL_YYACCEPT = 357,                 /* $accept  */
  YYSYMBOL_Program = 358,                  /* Program  */
  YYSYMBOL_ListStatement = 359,            /* ListStatement  */
  YYSYMBOL_Statement = 360,                /* Statement  */
  YYSYMBOL_ListMnemonicArgs = 361,         /* ListMnemonicArgs  */
  YYSYMBOL_MnemonicArgs = 362,             /* MnemonicArgs  */
  YYSYMBOL_Exp = 363,                      /* Exp  */
  YYSYMBOL_Factor = 364,                   /* Factor  */
  YYSYMBOL_ConfigType = 365,               /* ConfigType  */
  YYSYMBOL_DataType = 366,                 /* DataType  */
  YYSYMBOL_Opcode = 367                    /* Opcode  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 64 "nask.y"

void yyerror(YYLTYPE *loc, yyscan_t scanner, YYSTYPE *result, const char *msg)
{
  fprintf(stderr, "error: %d,%d: %s at %s\n",
    loc->first_line, loc->first_column, msg, nask_get_text(scanner));
}

int yyparse(yyscan_t scanner, YYSTYPE *result);

extern int yylex(YYSTYPE *lvalp, YYLTYPE *llocp, yyscan_t scanner);

#line 517 "parser.cc"


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
#define YYLAST   688

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  357
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  364
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  390

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   611


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
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   446,   446,   448,   449,   451,   452,   453,   454,   455,
     457,   458,   460,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   479,
     480,   481,   482,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   494,   495,   496,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   596,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,   710,   711,   712,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
     723,   724,   725,   726,   727,   728,   729,   730,   731,   732,
     733,   734,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,   760,   761,   762,
     763,   764,   765,   766,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   817
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
  "_KW_BYTE", "_KW_CALL", "_KW_CBW", "_KW_CDQ", "_KW_CLC", "_KW_CLD",
  "_KW_CLI", "_KW_CLTS", "_KW_CMC", "_KW_CMP", "_KW_CMPSB", "_KW_CMPSD",
  "_KW_CMPSW", "_KW_CMPXCHG", "_KW_CPUID", "_KW_CWD", "_KW_CWDE",
  "_KW_DAA", "_KW_DAS", "_KW_DB", "_KW_DD", "_KW_DEC", "_KW_DIV", "_KW_DQ",
  "_KW_DT", "_KW_DW", "_KW_DWORD", "_KW_END", "_KW_ENTER", "_KW_EQU",
  "_KW_EXTERN", "_KW_F2XM1", "_KW_FABS", "_KW_FADD", "_KW_FADDP",
  "_KW_FBLD", "_KW_FBSTP", "_KW_FCHS", "_KW_FCLEX", "_KW_FCOM",
  "_KW_FCOMP", "_KW_FCOMPP", "_KW_FCOS", "_KW_FDECSTP", "_KW_FDISI",
  "_KW_FDIV", "_KW_FDIVP", "_KW_FDIVR", "_KW_FDIVRP", "_KW_FENI",
  "_KW_FFREE", "_KW_FIADD", "_KW_FICOM", "_KW_FICOMP", "_KW_FIDIV",
  "_KW_FIDIVR", "_KW_FILD", "_KW_FILE", "_KW_FIMUL", "_KW_FINCSTP",
  "_KW_FINIT", "_KW_FIST", "_KW_FISTP", "_KW_FISUB", "_KW_FISUBR",
  "_KW_FLD", "_KW_FLD1", "_KW_FLDCW", "_KW_FLDENV", "_KW_FLDL2E",
  "_KW_FLDL2T", "_KW_FLDLG2", "_KW_FLDLN2", "_KW_FLDPI", "_KW_FLDZ",
  "_KW_FMUL", "_KW_FMULP", "_KW_FNCLEX", "_KW_FNDISI", "_KW_FNENI",
  "_KW_FNINIT", "_KW_FNOP", "_KW_FNSAVE", "_KW_FNSTCW", "_KW_FNSTENV",
  "_KW_FNSTSW", "_KW_FORMAT", "_KW_FPATAN", "_KW_FPREM", "_KW_FPREM1",
  "_KW_FPTAN", "_KW_FRNDINT", "_KW_FRSTOR", "_KW_FSAVE", "_KW_FSCALE",
  "_KW_FSETPM", "_KW_FSIN", "_KW_FSINCOS", "_KW_FSQRT", "_KW_FST",
  "_KW_FSTCW", "_KW_FSTENV", "_KW_FSTP", "_KW_FSTSW", "_KW_FSUB",
  "_KW_FSUBP", "_KW_FSUBR", "_KW_FSUBRP", "_KW_FTST", "_KW_FUCOM",
  "_KW_FUCOMP", "_KW_FUCOMPP", "_KW_FXAM", "_KW_FXCH", "_KW_FXTRACT",
  "_KW_FYL2X", "_KW_FYL2XP1", "_KW_HLT", "_KW_IDIV", "_KW_IMUL", "_KW_IN",
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
  "_KW_MOVZX", "_KW_MUL", "_KW_NEG", "_KW_NOP", "_KW_NOT", "_KW_OPTIMIZE",
  "_KW_OR", "_KW_ORG", "_KW_OUT", "_KW_OUTSB", "_KW_OUTSD", "_KW_OUTSW",
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
  "_KW_VERR", "_KW_VERW", "_KW_WAIT", "_KW_WBINVD", "_KW_WORD",
  "_KW_WRMSR", "_KW_XADD", "_KW_XCHG", "_KW_XLATB", "_KW_XOR", "_LBRACK",
  "_RBRACK", "T_Hex", "T_Label", "_STRING_", "_INTEGER_", "_IDENT_",
  "$accept", "Program", "ListStatement", "Statement", "ListMnemonicArgs",
  "MnemonicArgs", "Exp", "Factor", "ConfigType", "DataType", "Opcode", YY_NULLPTR
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
     605,   606,   607,   608,   609,   610,   611
};
#endif

#define YYPACT_NINF (-326)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-25)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     332,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,  -326,
    -326,    -1,  -326,   -59,    28,  -326,   332,   -33,  -326,  -326,
    -326,  -326,  -326,  -326,  -326,  -326,  -326,  -325,   -33,  -326,
    -326,  -326,  -326,  -326,   -33,  -326,  -326,  -326,  -326,  -326,
    -326,    23,    85,  -326,   -20,  -318,    85,    -4,   -33,   -33,
     -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,
     -33,    55,  -326,  -326,  -326,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,     9,   -33,    72,    85
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,    45,    46,    48,    47,    49,    50,    52,    53,    51,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   147,   148,   146,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     275,   276,   274,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,     0,     5,     0,     0,     2,     3,     9,    40,    33,
      41,    36,    34,    35,    37,    38,    39,     0,     0,     1,
       4,    42,    44,    43,     0,    30,    27,    32,    29,    31,
       8,    10,    12,    28,     0,     0,     6,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,    24,    11,    14,    23,    21,    19,    20,
      22,    15,    17,    13,    16,    18,     0,     0,    25,    26
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -326,  -326,  -292,  -326,  -323,  -326,  -312,  -326,  -326,  -326,
    -326
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,   324,   325,   326,   350,   351,   352,   353,   337,   354,
     327
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     359,   360,   361,   362,   341,   363,   364,   338,   365,   366,
     367,   368,   369,   359,   360,   361,   362,   341,   363,   364,
     328,   365,   366,   367,   368,   369,   356,   329,   339,   355,
     342,   358,   357,   372,   340,   374,     0,     0,     0,     0,
       0,     0,   371,   342,     0,     0,     0,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   359,
     360,   361,   362,     0,   363,   364,   387,   365,   366,   367,
     368,   369,     0,     0,     0,   389,   -24,   -24,   -24,   -24,
       0,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   359,
     360,   361,   362,   330,   363,   364,     0,   365,   366,   367,
     368,   369,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,   334,   335,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   344,     0,   345,
     346,   347,   348,   349,   343,     0,     0,     0,     0,     0,
     370,     0,   345,   346,   347,   348,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     1,
       2,     3,     4,     0,     5,     6,     7,     8,     9,    10,
     388,    11,    12,    13,    14,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,     0,    44,    45,     0,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,     0,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,     0,   215,   216,   217,   218,   219,   220,     0,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,     0,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,     0,   316,   317,   318,
     319,   320,   321,     0,     0,   322,     0,     0,   323
};

static const yytype_int16 yycheck[] =
{
       4,     5,     6,     7,    37,     9,    10,    66,    12,    13,
      14,    15,    16,     4,     5,     6,     7,    37,     9,    10,
      21,    12,    13,    14,    15,    16,   338,    28,     0,   354,
      63,     8,   344,   351,   326,   358,    -1,    -1,    -1,    -1,
      -1,    -1,   354,    63,    -1,    -1,    -1,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,     4,
       5,     6,     7,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    -1,   387,     4,     5,     6,     7,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,     4,
       5,     6,     7,    94,     9,    10,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,
      -1,    -1,    -1,    -1,    -1,   246,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   344,    -1,    -1,    -1,    -1,    -1,   350,    -1,   352,
     353,   354,   355,   356,   344,    -1,    -1,    -1,    -1,    -1,
     350,    -1,   352,   353,   354,   355,   356,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
     351,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,    -1,   240,   241,   242,   243,   244,   245,    -1,    -1,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,    -1,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,    -1,   345,   346,   347,
     348,   349,   350,    -1,    -1,   353,    -1,    -1,   356
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    17,    18,    19,    20,    22,    23,    24,    25,    26,
      27,    29,    30,    31,    32,    33,    34,    35,    36,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    64,    65,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   240,   241,   242,   243,   244,
     245,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   345,   346,   347,   348,
     349,   350,   353,   356,   358,   359,   360,   367,    21,    28,
      94,   123,   162,   239,   246,   247,   290,   365,    66,     0,
     359,    37,    63,   344,   350,   352,   353,   354,   355,   356,
     361,   362,   363,   364,   366,   354,   363,   363,     8,     4,
       5,     6,     7,     9,    10,    12,    13,    14,    15,    16,
     350,   363,   351,   351,   361,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   363,   363,   363,    11,   351,   363
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   357,   358,   359,   359,   360,   360,   360,   360,   360,
     361,   361,   362,   363,   363,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   363,   363,   363,   363,   363,   364,
     364,   364,   364,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   366,   366,   366,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     3,     4,     2,     1,
       1,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     4,     1,     1,     1,
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
       1,     1,     1,     1,     1
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
#line 446 "nask.y"
                        { std::reverse((yyvsp[0].liststatement_)->begin(),(yyvsp[0].liststatement_)->end()) ;(yyval.program_) = new Prog((yyvsp[0].liststatement_)); result->program_ = (yyval.program_); }
#line 2070 "parser.cc"
    break;

  case 3: /* ListStatement: Statement  */
#line 448 "nask.y"
                          { (yyval.liststatement_) = new ListStatement(); (yyval.liststatement_)->push_back((yyvsp[0].statement_)); result->liststatement_ = (yyval.liststatement_); }
#line 2076 "parser.cc"
    break;

  case 4: /* ListStatement: Statement ListStatement  */
#line 449 "nask.y"
                            { (yyvsp[0].liststatement_)->push_back((yyvsp[-1].statement_)); (yyval.liststatement_) = (yyvsp[0].liststatement_); result->liststatement_ = (yyval.liststatement_); }
#line 2082 "parser.cc"
    break;

  case 5: /* Statement: T_Label  */
#line 451 "nask.y"
                    { (yyval.statement_) = new LabelStmt((yyvsp[0]._string)); result->statement_ = (yyval.statement_); }
#line 2088 "parser.cc"
    break;

  case 6: /* Statement: _IDENT_ _KW_EQU Exp  */
#line 452 "nask.y"
                        { (yyval.statement_) = new DeclareStmt((yyvsp[-2]._string), (yyvsp[0].exp_)); result->statement_ = (yyval.statement_); }
#line 2094 "parser.cc"
    break;

  case 7: /* Statement: _LBRACK ConfigType _STRING_ _RBRACK  */
#line 453 "nask.y"
                                        { (yyval.statement_) = new ConfigStmt((yyvsp[-2].configtype_), (yyvsp[-1]._string)); result->statement_ = (yyval.statement_); }
#line 2100 "parser.cc"
    break;

  case 8: /* Statement: Opcode ListMnemonicArgs  */
#line 454 "nask.y"
                            { std::reverse((yyvsp[0].listmnemonicargs_)->begin(),(yyvsp[0].listmnemonicargs_)->end()) ;(yyval.statement_) = new MnemonicStmt((yyvsp[-1].opcode_), (yyvsp[0].listmnemonicargs_)); result->statement_ = (yyval.statement_); }
#line 2106 "parser.cc"
    break;

  case 9: /* Statement: Opcode  */
#line 455 "nask.y"
           { (yyval.statement_) = new OpcodeStmt((yyvsp[0].opcode_)); result->statement_ = (yyval.statement_); }
#line 2112 "parser.cc"
    break;

  case 10: /* ListMnemonicArgs: MnemonicArgs  */
#line 457 "nask.y"
                                { (yyval.listmnemonicargs_) = new ListMnemonicArgs(); (yyval.listmnemonicargs_)->push_back((yyvsp[0].mnemonicargs_)); result->listmnemonicargs_ = (yyval.listmnemonicargs_); }
#line 2118 "parser.cc"
    break;

  case 11: /* ListMnemonicArgs: MnemonicArgs _COMMA ListMnemonicArgs  */
#line 458 "nask.y"
                                         { (yyvsp[0].listmnemonicargs_)->push_back((yyvsp[-2].mnemonicargs_)); (yyval.listmnemonicargs_) = (yyvsp[0].listmnemonicargs_); result->listmnemonicargs_ = (yyval.listmnemonicargs_); }
#line 2124 "parser.cc"
    break;

  case 12: /* MnemonicArgs: Exp  */
#line 460 "nask.y"
                   { (yyval.mnemonicargs_) = new MnemoArg((yyvsp[0].exp_)); result->mnemonicargs_ = (yyval.mnemonicargs_); }
#line 2130 "parser.cc"
    break;

  case 13: /* Exp: Exp _DEQ Exp  */
#line 462 "nask.y"
                   { (yyval.exp_) = new EqExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2136 "parser.cc"
    break;

  case 14: /* Exp: Exp _BANGEQ Exp  */
#line 463 "nask.y"
                    { (yyval.exp_) = new NeqExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2142 "parser.cc"
    break;

  case 15: /* Exp: Exp _LT Exp  */
#line 464 "nask.y"
                { (yyval.exp_) = new LtExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2148 "parser.cc"
    break;

  case 16: /* Exp: Exp _GT Exp  */
#line 465 "nask.y"
                { (yyval.exp_) = new GtExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2154 "parser.cc"
    break;

  case 17: /* Exp: Exp _LDARROW Exp  */
#line 466 "nask.y"
                     { (yyval.exp_) = new LteExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2160 "parser.cc"
    break;

  case 18: /* Exp: Exp _GTEQ Exp  */
#line 467 "nask.y"
                  { (yyval.exp_) = new GteExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2166 "parser.cc"
    break;

  case 19: /* Exp: Exp _PLUS Exp  */
#line 468 "nask.y"
                  { (yyval.exp_) = new PlusExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2172 "parser.cc"
    break;

  case 20: /* Exp: Exp _MINUS Exp  */
#line 469 "nask.y"
                   { (yyval.exp_) = new MinusExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2178 "parser.cc"
    break;

  case 21: /* Exp: Exp _STAR Exp  */
#line 470 "nask.y"
                  { (yyval.exp_) = new MulExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2184 "parser.cc"
    break;

  case 22: /* Exp: Exp _SLASH Exp  */
#line 471 "nask.y"
                   { (yyval.exp_) = new DivExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2190 "parser.cc"
    break;

  case 23: /* Exp: Exp _PERCENT Exp  */
#line 472 "nask.y"
                     { (yyval.exp_) = new ModExp((yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2196 "parser.cc"
    break;

  case 24: /* Exp: _LBRACK Exp _RBRACK  */
#line 473 "nask.y"
                        { (yyval.exp_) = new IndirectAddrExp((yyvsp[-1].exp_)); result->exp_ = (yyval.exp_); }
#line 2202 "parser.cc"
    break;

  case 25: /* Exp: DataType _LBRACK Exp _RBRACK  */
#line 474 "nask.y"
                                 { (yyval.exp_) = new DatatypeExp((yyvsp[-3].datatype_), (yyvsp[-1].exp_)); result->exp_ = (yyval.exp_); }
#line 2208 "parser.cc"
    break;

  case 26: /* Exp: DataType Exp _COLON Exp  */
#line 475 "nask.y"
                            { (yyval.exp_) = new RangeExp((yyvsp[-3].datatype_), (yyvsp[-2].exp_), (yyvsp[0].exp_)); result->exp_ = (yyval.exp_); }
#line 2214 "parser.cc"
    break;

  case 27: /* Exp: T_Label  */
#line 476 "nask.y"
            { (yyval.exp_) = new LabelExp((yyvsp[0]._string)); result->exp_ = (yyval.exp_); }
#line 2220 "parser.cc"
    break;

  case 28: /* Exp: Factor  */
#line 477 "nask.y"
           { (yyval.exp_) = new ImmExp((yyvsp[0].factor_)); result->exp_ = (yyval.exp_); }
#line 2226 "parser.cc"
    break;

  case 29: /* Factor: _INTEGER_  */
#line 479 "nask.y"
                   { (yyval.factor_) = new NumberFactor((yyvsp[0]._int)); result->factor_ = (yyval.factor_); }
#line 2232 "parser.cc"
    break;

  case 30: /* Factor: T_Hex  */
#line 480 "nask.y"
          { (yyval.factor_) = new HexFactor((yyvsp[0]._string)); result->factor_ = (yyval.factor_); }
#line 2238 "parser.cc"
    break;

  case 31: /* Factor: _IDENT_  */
#line 481 "nask.y"
            { (yyval.factor_) = new IdentFactor((yyvsp[0]._string)); result->factor_ = (yyval.factor_); }
#line 2244 "parser.cc"
    break;

  case 32: /* Factor: _STRING_  */
#line 482 "nask.y"
             { (yyval.factor_) = new StringFactor((yyvsp[0]._string)); result->factor_ = (yyval.factor_); }
#line 2250 "parser.cc"
    break;

  case 33: /* ConfigType: _KW_BITS  */
#line 484 "nask.y"
                      { (yyval.configtype_) = new BitsConfig(); result->configtype_ = (yyval.configtype_); }
#line 2256 "parser.cc"
    break;

  case 34: /* ConfigType: _KW_INSTRSET  */
#line 485 "nask.y"
                 { (yyval.configtype_) = new InstConfig(); result->configtype_ = (yyval.configtype_); }
#line 2262 "parser.cc"
    break;

  case 35: /* ConfigType: _KW_OPTIMIZE  */
#line 486 "nask.y"
                 { (yyval.configtype_) = new OptiConfig(); result->configtype_ = (yyval.configtype_); }
#line 2268 "parser.cc"
    break;

  case 36: /* ConfigType: _KW_FORMAT  */
#line 487 "nask.y"
               { (yyval.configtype_) = new FormConfig(); result->configtype_ = (yyval.configtype_); }
#line 2274 "parser.cc"
    break;

  case 37: /* ConfigType: _KW_PADDING  */
#line 488 "nask.y"
                { (yyval.configtype_) = new PaddConfig(); result->configtype_ = (yyval.configtype_); }
#line 2280 "parser.cc"
    break;

  case 38: /* ConfigType: _KW_PADSET  */
#line 489 "nask.y"
               { (yyval.configtype_) = new PadsConfig(); result->configtype_ = (yyval.configtype_); }
#line 2286 "parser.cc"
    break;

  case 39: /* ConfigType: _KW_SECTION  */
#line 490 "nask.y"
                { (yyval.configtype_) = new SectConfig(); result->configtype_ = (yyval.configtype_); }
#line 2292 "parser.cc"
    break;

  case 40: /* ConfigType: _KW_ABSOLUTE  */
#line 491 "nask.y"
                 { (yyval.configtype_) = new AbsoConfig(); result->configtype_ = (yyval.configtype_); }
#line 2298 "parser.cc"
    break;

  case 41: /* ConfigType: _KW_FILE  */
#line 492 "nask.y"
             { (yyval.configtype_) = new FileConfig(); result->configtype_ = (yyval.configtype_); }
#line 2304 "parser.cc"
    break;

  case 42: /* DataType: _KW_BYTE  */
#line 494 "nask.y"
                    { (yyval.datatype_) = new ByteDataType(); result->datatype_ = (yyval.datatype_); }
#line 2310 "parser.cc"
    break;

  case 43: /* DataType: _KW_WORD  */
#line 495 "nask.y"
             { (yyval.datatype_) = new WordDataType(); result->datatype_ = (yyval.datatype_); }
#line 2316 "parser.cc"
    break;

  case 44: /* DataType: _KW_DWORD  */
#line 496 "nask.y"
              { (yyval.datatype_) = new DwordDataType(); result->datatype_ = (yyval.datatype_); }
#line 2322 "parser.cc"
    break;

  case 45: /* Opcode: _KW_AAA  */
#line 498 "nask.y"
                 { (yyval.opcode_) = new OpcodesAAA(); result->opcode_ = (yyval.opcode_); }
#line 2328 "parser.cc"
    break;

  case 46: /* Opcode: _KW_AAD  */
#line 499 "nask.y"
            { (yyval.opcode_) = new OpcodesAAD(); result->opcode_ = (yyval.opcode_); }
#line 2334 "parser.cc"
    break;

  case 47: /* Opcode: _KW_AAS  */
#line 500 "nask.y"
            { (yyval.opcode_) = new OpcodesAAS(); result->opcode_ = (yyval.opcode_); }
#line 2340 "parser.cc"
    break;

  case 48: /* Opcode: _KW_AAM  */
#line 501 "nask.y"
            { (yyval.opcode_) = new OpcodesAAM(); result->opcode_ = (yyval.opcode_); }
#line 2346 "parser.cc"
    break;

  case 49: /* Opcode: _KW_ADC  */
#line 502 "nask.y"
            { (yyval.opcode_) = new OpcodesADC(); result->opcode_ = (yyval.opcode_); }
#line 2352 "parser.cc"
    break;

  case 50: /* Opcode: _KW_ADD  */
#line 503 "nask.y"
            { (yyval.opcode_) = new OpcodesADD(); result->opcode_ = (yyval.opcode_); }
#line 2358 "parser.cc"
    break;

  case 51: /* Opcode: _KW_AND  */
#line 504 "nask.y"
            { (yyval.opcode_) = new OpcodesAND(); result->opcode_ = (yyval.opcode_); }
#line 2364 "parser.cc"
    break;

  case 52: /* Opcode: _KW_ALIGN  */
#line 505 "nask.y"
              { (yyval.opcode_) = new OpcodesALIGN(); result->opcode_ = (yyval.opcode_); }
#line 2370 "parser.cc"
    break;

  case 53: /* Opcode: _KW_ALIGNB  */
#line 506 "nask.y"
               { (yyval.opcode_) = new OpcodesALIGNB(); result->opcode_ = (yyval.opcode_); }
#line 2376 "parser.cc"
    break;

  case 54: /* Opcode: _KW_ARPL  */
#line 507 "nask.y"
             { (yyval.opcode_) = new OpcodesARPL(); result->opcode_ = (yyval.opcode_); }
#line 2382 "parser.cc"
    break;

  case 55: /* Opcode: _KW_BOUND  */
#line 508 "nask.y"
              { (yyval.opcode_) = new OpcodesBOUND(); result->opcode_ = (yyval.opcode_); }
#line 2388 "parser.cc"
    break;

  case 56: /* Opcode: _KW_BSF  */
#line 509 "nask.y"
            { (yyval.opcode_) = new OpcodesBSF(); result->opcode_ = (yyval.opcode_); }
#line 2394 "parser.cc"
    break;

  case 57: /* Opcode: _KW_BSR  */
#line 510 "nask.y"
            { (yyval.opcode_) = new OpcodesBSR(); result->opcode_ = (yyval.opcode_); }
#line 2400 "parser.cc"
    break;

  case 58: /* Opcode: _KW_BSWAP  */
#line 511 "nask.y"
              { (yyval.opcode_) = new OpcodesBSWAP(); result->opcode_ = (yyval.opcode_); }
#line 2406 "parser.cc"
    break;

  case 59: /* Opcode: _KW_BT  */
#line 512 "nask.y"
           { (yyval.opcode_) = new OpcodesBT(); result->opcode_ = (yyval.opcode_); }
#line 2412 "parser.cc"
    break;

  case 60: /* Opcode: _KW_BTC  */
#line 513 "nask.y"
            { (yyval.opcode_) = new OpcodesBTC(); result->opcode_ = (yyval.opcode_); }
#line 2418 "parser.cc"
    break;

  case 61: /* Opcode: _KW_BTR  */
#line 514 "nask.y"
            { (yyval.opcode_) = new OpcodesBTR(); result->opcode_ = (yyval.opcode_); }
#line 2424 "parser.cc"
    break;

  case 62: /* Opcode: _KW_BTS  */
#line 515 "nask.y"
            { (yyval.opcode_) = new OpcodesBTS(); result->opcode_ = (yyval.opcode_); }
#line 2430 "parser.cc"
    break;

  case 63: /* Opcode: _KW_CALL  */
#line 516 "nask.y"
             { (yyval.opcode_) = new OpcodesCALL(); result->opcode_ = (yyval.opcode_); }
#line 2436 "parser.cc"
    break;

  case 64: /* Opcode: _KW_CBW  */
#line 517 "nask.y"
            { (yyval.opcode_) = new OpcodesCBW(); result->opcode_ = (yyval.opcode_); }
#line 2442 "parser.cc"
    break;

  case 65: /* Opcode: _KW_CDQ  */
#line 518 "nask.y"
            { (yyval.opcode_) = new OpcodesCDQ(); result->opcode_ = (yyval.opcode_); }
#line 2448 "parser.cc"
    break;

  case 66: /* Opcode: _KW_CLC  */
#line 519 "nask.y"
            { (yyval.opcode_) = new OpcodesCLC(); result->opcode_ = (yyval.opcode_); }
#line 2454 "parser.cc"
    break;

  case 67: /* Opcode: _KW_CLD  */
#line 520 "nask.y"
            { (yyval.opcode_) = new OpcodesCLD(); result->opcode_ = (yyval.opcode_); }
#line 2460 "parser.cc"
    break;

  case 68: /* Opcode: _KW_CLI  */
#line 521 "nask.y"
            { (yyval.opcode_) = new OpcodesCLI(); result->opcode_ = (yyval.opcode_); }
#line 2466 "parser.cc"
    break;

  case 69: /* Opcode: _KW_CLTS  */
#line 522 "nask.y"
             { (yyval.opcode_) = new OpcodesCLTS(); result->opcode_ = (yyval.opcode_); }
#line 2472 "parser.cc"
    break;

  case 70: /* Opcode: _KW_CMC  */
#line 523 "nask.y"
            { (yyval.opcode_) = new OpcodesCMC(); result->opcode_ = (yyval.opcode_); }
#line 2478 "parser.cc"
    break;

  case 71: /* Opcode: _KW_CMP  */
#line 524 "nask.y"
            { (yyval.opcode_) = new OpcodesCMP(); result->opcode_ = (yyval.opcode_); }
#line 2484 "parser.cc"
    break;

  case 72: /* Opcode: _KW_CMPSB  */
#line 525 "nask.y"
              { (yyval.opcode_) = new OpcodesCMPSB(); result->opcode_ = (yyval.opcode_); }
#line 2490 "parser.cc"
    break;

  case 73: /* Opcode: _KW_CMPSD  */
#line 526 "nask.y"
              { (yyval.opcode_) = new OpcodesCMPSD(); result->opcode_ = (yyval.opcode_); }
#line 2496 "parser.cc"
    break;

  case 74: /* Opcode: _KW_CMPSW  */
#line 527 "nask.y"
              { (yyval.opcode_) = new OpcodesCMPSW(); result->opcode_ = (yyval.opcode_); }
#line 2502 "parser.cc"
    break;

  case 75: /* Opcode: _KW_CMPXCHG  */
#line 528 "nask.y"
                { (yyval.opcode_) = new OpcodesCMPXCHG(); result->opcode_ = (yyval.opcode_); }
#line 2508 "parser.cc"
    break;

  case 76: /* Opcode: _KW_CPUID  */
#line 529 "nask.y"
              { (yyval.opcode_) = new OpcodesCPUID(); result->opcode_ = (yyval.opcode_); }
#line 2514 "parser.cc"
    break;

  case 77: /* Opcode: _KW_CWD  */
#line 530 "nask.y"
            { (yyval.opcode_) = new OpcodesCWD(); result->opcode_ = (yyval.opcode_); }
#line 2520 "parser.cc"
    break;

  case 78: /* Opcode: _KW_CWDE  */
#line 531 "nask.y"
             { (yyval.opcode_) = new OpcodesCWDE(); result->opcode_ = (yyval.opcode_); }
#line 2526 "parser.cc"
    break;

  case 79: /* Opcode: _KW_DAA  */
#line 532 "nask.y"
            { (yyval.opcode_) = new OpcodesDAA(); result->opcode_ = (yyval.opcode_); }
#line 2532 "parser.cc"
    break;

  case 80: /* Opcode: _KW_DAS  */
#line 533 "nask.y"
            { (yyval.opcode_) = new OpcodesDAS(); result->opcode_ = (yyval.opcode_); }
#line 2538 "parser.cc"
    break;

  case 81: /* Opcode: _KW_DB  */
#line 534 "nask.y"
           { (yyval.opcode_) = new OpcodesDB(); result->opcode_ = (yyval.opcode_); }
#line 2544 "parser.cc"
    break;

  case 82: /* Opcode: _KW_DD  */
#line 535 "nask.y"
           { (yyval.opcode_) = new OpcodesDD(); result->opcode_ = (yyval.opcode_); }
#line 2550 "parser.cc"
    break;

  case 83: /* Opcode: _KW_DEC  */
#line 536 "nask.y"
            { (yyval.opcode_) = new OpcodesDEC(); result->opcode_ = (yyval.opcode_); }
#line 2556 "parser.cc"
    break;

  case 84: /* Opcode: _KW_DIV  */
#line 537 "nask.y"
            { (yyval.opcode_) = new OpcodesDIV(); result->opcode_ = (yyval.opcode_); }
#line 2562 "parser.cc"
    break;

  case 85: /* Opcode: _KW_DQ  */
#line 538 "nask.y"
           { (yyval.opcode_) = new OpcodesDQ(); result->opcode_ = (yyval.opcode_); }
#line 2568 "parser.cc"
    break;

  case 86: /* Opcode: _KW_DT  */
#line 539 "nask.y"
           { (yyval.opcode_) = new OpcodesDT(); result->opcode_ = (yyval.opcode_); }
#line 2574 "parser.cc"
    break;

  case 87: /* Opcode: _KW_DW  */
#line 540 "nask.y"
           { (yyval.opcode_) = new OpcodesDW(); result->opcode_ = (yyval.opcode_); }
#line 2580 "parser.cc"
    break;

  case 88: /* Opcode: _KW_END  */
#line 541 "nask.y"
            { (yyval.opcode_) = new OpcodesEND(); result->opcode_ = (yyval.opcode_); }
#line 2586 "parser.cc"
    break;

  case 89: /* Opcode: _KW_ENTER  */
#line 542 "nask.y"
              { (yyval.opcode_) = new OpcodesENTER(); result->opcode_ = (yyval.opcode_); }
#line 2592 "parser.cc"
    break;

  case 90: /* Opcode: _KW_EXTERN  */
#line 543 "nask.y"
               { (yyval.opcode_) = new OpcodesEXTERN(); result->opcode_ = (yyval.opcode_); }
#line 2598 "parser.cc"
    break;

  case 91: /* Opcode: _KW_F2XM1  */
#line 544 "nask.y"
              { (yyval.opcode_) = new OpcodesF2XM1(); result->opcode_ = (yyval.opcode_); }
#line 2604 "parser.cc"
    break;

  case 92: /* Opcode: _KW_FABS  */
#line 545 "nask.y"
             { (yyval.opcode_) = new OpcodesFABS(); result->opcode_ = (yyval.opcode_); }
#line 2610 "parser.cc"
    break;

  case 93: /* Opcode: _KW_FADD  */
#line 546 "nask.y"
             { (yyval.opcode_) = new OpcodesFADD(); result->opcode_ = (yyval.opcode_); }
#line 2616 "parser.cc"
    break;

  case 94: /* Opcode: _KW_FADDP  */
#line 547 "nask.y"
              { (yyval.opcode_) = new OpcodesFADDP(); result->opcode_ = (yyval.opcode_); }
#line 2622 "parser.cc"
    break;

  case 95: /* Opcode: _KW_FBLD  */
#line 548 "nask.y"
             { (yyval.opcode_) = new OpcodesFBLD(); result->opcode_ = (yyval.opcode_); }
#line 2628 "parser.cc"
    break;

  case 96: /* Opcode: _KW_FBSTP  */
#line 549 "nask.y"
              { (yyval.opcode_) = new OpcodesFBSTP(); result->opcode_ = (yyval.opcode_); }
#line 2634 "parser.cc"
    break;

  case 97: /* Opcode: _KW_FCHS  */
#line 550 "nask.y"
             { (yyval.opcode_) = new OpcodesFCHS(); result->opcode_ = (yyval.opcode_); }
#line 2640 "parser.cc"
    break;

  case 98: /* Opcode: _KW_FCLEX  */
#line 551 "nask.y"
              { (yyval.opcode_) = new OpcodesFCLEX(); result->opcode_ = (yyval.opcode_); }
#line 2646 "parser.cc"
    break;

  case 99: /* Opcode: _KW_FCOM  */
#line 552 "nask.y"
             { (yyval.opcode_) = new OpcodesFCOM(); result->opcode_ = (yyval.opcode_); }
#line 2652 "parser.cc"
    break;

  case 100: /* Opcode: _KW_FCOMP  */
#line 553 "nask.y"
              { (yyval.opcode_) = new OpcodesFCOMP(); result->opcode_ = (yyval.opcode_); }
#line 2658 "parser.cc"
    break;

  case 101: /* Opcode: _KW_FCOMPP  */
#line 554 "nask.y"
               { (yyval.opcode_) = new OpcodesFCOMPP(); result->opcode_ = (yyval.opcode_); }
#line 2664 "parser.cc"
    break;

  case 102: /* Opcode: _KW_FCOS  */
#line 555 "nask.y"
             { (yyval.opcode_) = new OpcodesFCOS(); result->opcode_ = (yyval.opcode_); }
#line 2670 "parser.cc"
    break;

  case 103: /* Opcode: _KW_FDECSTP  */
#line 556 "nask.y"
                { (yyval.opcode_) = new OpcodesFDECSTP(); result->opcode_ = (yyval.opcode_); }
#line 2676 "parser.cc"
    break;

  case 104: /* Opcode: _KW_FDISI  */
#line 557 "nask.y"
              { (yyval.opcode_) = new OpcodesFDISI(); result->opcode_ = (yyval.opcode_); }
#line 2682 "parser.cc"
    break;

  case 105: /* Opcode: _KW_FDIV  */
#line 558 "nask.y"
             { (yyval.opcode_) = new OpcodesFDIV(); result->opcode_ = (yyval.opcode_); }
#line 2688 "parser.cc"
    break;

  case 106: /* Opcode: _KW_FDIVP  */
#line 559 "nask.y"
              { (yyval.opcode_) = new OpcodesFDIVP(); result->opcode_ = (yyval.opcode_); }
#line 2694 "parser.cc"
    break;

  case 107: /* Opcode: _KW_FDIVR  */
#line 560 "nask.y"
              { (yyval.opcode_) = new OpcodesFDIVR(); result->opcode_ = (yyval.opcode_); }
#line 2700 "parser.cc"
    break;

  case 108: /* Opcode: _KW_FDIVRP  */
#line 561 "nask.y"
               { (yyval.opcode_) = new OpcodesFDIVRP(); result->opcode_ = (yyval.opcode_); }
#line 2706 "parser.cc"
    break;

  case 109: /* Opcode: _KW_FENI  */
#line 562 "nask.y"
             { (yyval.opcode_) = new OpcodesFENI(); result->opcode_ = (yyval.opcode_); }
#line 2712 "parser.cc"
    break;

  case 110: /* Opcode: _KW_FFREE  */
#line 563 "nask.y"
              { (yyval.opcode_) = new OpcodesFFREE(); result->opcode_ = (yyval.opcode_); }
#line 2718 "parser.cc"
    break;

  case 111: /* Opcode: _KW_FIADD  */
#line 564 "nask.y"
              { (yyval.opcode_) = new OpcodesFIADD(); result->opcode_ = (yyval.opcode_); }
#line 2724 "parser.cc"
    break;

  case 112: /* Opcode: _KW_FICOM  */
#line 565 "nask.y"
              { (yyval.opcode_) = new OpcodesFICOM(); result->opcode_ = (yyval.opcode_); }
#line 2730 "parser.cc"
    break;

  case 113: /* Opcode: _KW_FICOMP  */
#line 566 "nask.y"
               { (yyval.opcode_) = new OpcodesFICOMP(); result->opcode_ = (yyval.opcode_); }
#line 2736 "parser.cc"
    break;

  case 114: /* Opcode: _KW_FIDIV  */
#line 567 "nask.y"
              { (yyval.opcode_) = new OpcodesFIDIV(); result->opcode_ = (yyval.opcode_); }
#line 2742 "parser.cc"
    break;

  case 115: /* Opcode: _KW_FIDIVR  */
#line 568 "nask.y"
               { (yyval.opcode_) = new OpcodesFIDIVR(); result->opcode_ = (yyval.opcode_); }
#line 2748 "parser.cc"
    break;

  case 116: /* Opcode: _KW_FILD  */
#line 569 "nask.y"
             { (yyval.opcode_) = new OpcodesFILD(); result->opcode_ = (yyval.opcode_); }
#line 2754 "parser.cc"
    break;

  case 117: /* Opcode: _KW_FIMUL  */
#line 570 "nask.y"
              { (yyval.opcode_) = new OpcodesFIMUL(); result->opcode_ = (yyval.opcode_); }
#line 2760 "parser.cc"
    break;

  case 118: /* Opcode: _KW_FINCSTP  */
#line 571 "nask.y"
                { (yyval.opcode_) = new OpcodesFINCSTP(); result->opcode_ = (yyval.opcode_); }
#line 2766 "parser.cc"
    break;

  case 119: /* Opcode: _KW_FINIT  */
#line 572 "nask.y"
              { (yyval.opcode_) = new OpcodesFINIT(); result->opcode_ = (yyval.opcode_); }
#line 2772 "parser.cc"
    break;

  case 120: /* Opcode: _KW_FIST  */
#line 573 "nask.y"
             { (yyval.opcode_) = new OpcodesFIST(); result->opcode_ = (yyval.opcode_); }
#line 2778 "parser.cc"
    break;

  case 121: /* Opcode: _KW_FISTP  */
#line 574 "nask.y"
              { (yyval.opcode_) = new OpcodesFISTP(); result->opcode_ = (yyval.opcode_); }
#line 2784 "parser.cc"
    break;

  case 122: /* Opcode: _KW_FISUB  */
#line 575 "nask.y"
              { (yyval.opcode_) = new OpcodesFISUB(); result->opcode_ = (yyval.opcode_); }
#line 2790 "parser.cc"
    break;

  case 123: /* Opcode: _KW_FISUBR  */
#line 576 "nask.y"
               { (yyval.opcode_) = new OpcodesFISUBR(); result->opcode_ = (yyval.opcode_); }
#line 2796 "parser.cc"
    break;

  case 124: /* Opcode: _KW_FLD  */
#line 577 "nask.y"
            { (yyval.opcode_) = new OpcodesFLD(); result->opcode_ = (yyval.opcode_); }
#line 2802 "parser.cc"
    break;

  case 125: /* Opcode: _KW_FLD1  */
#line 578 "nask.y"
             { (yyval.opcode_) = new OpcodesFLD1(); result->opcode_ = (yyval.opcode_); }
#line 2808 "parser.cc"
    break;

  case 126: /* Opcode: _KW_FLDCW  */
#line 579 "nask.y"
              { (yyval.opcode_) = new OpcodesFLDCW(); result->opcode_ = (yyval.opcode_); }
#line 2814 "parser.cc"
    break;

  case 127: /* Opcode: _KW_FLDENV  */
#line 580 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDENV(); result->opcode_ = (yyval.opcode_); }
#line 2820 "parser.cc"
    break;

  case 128: /* Opcode: _KW_FLDL2E  */
#line 581 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDL2E(); result->opcode_ = (yyval.opcode_); }
#line 2826 "parser.cc"
    break;

  case 129: /* Opcode: _KW_FLDL2T  */
#line 582 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDL2T(); result->opcode_ = (yyval.opcode_); }
#line 2832 "parser.cc"
    break;

  case 130: /* Opcode: _KW_FLDLG2  */
#line 583 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDLG2(); result->opcode_ = (yyval.opcode_); }
#line 2838 "parser.cc"
    break;

  case 131: /* Opcode: _KW_FLDLN2  */
#line 584 "nask.y"
               { (yyval.opcode_) = new OpcodesFLDLN2(); result->opcode_ = (yyval.opcode_); }
#line 2844 "parser.cc"
    break;

  case 132: /* Opcode: _KW_FLDPI  */
#line 585 "nask.y"
              { (yyval.opcode_) = new OpcodesFLDPI(); result->opcode_ = (yyval.opcode_); }
#line 2850 "parser.cc"
    break;

  case 133: /* Opcode: _KW_FLDZ  */
#line 586 "nask.y"
             { (yyval.opcode_) = new OpcodesFLDZ(); result->opcode_ = (yyval.opcode_); }
#line 2856 "parser.cc"
    break;

  case 134: /* Opcode: _KW_FMUL  */
#line 587 "nask.y"
             { (yyval.opcode_) = new OpcodesFMUL(); result->opcode_ = (yyval.opcode_); }
#line 2862 "parser.cc"
    break;

  case 135: /* Opcode: _KW_FMULP  */
#line 588 "nask.y"
              { (yyval.opcode_) = new OpcodesFMULP(); result->opcode_ = (yyval.opcode_); }
#line 2868 "parser.cc"
    break;

  case 136: /* Opcode: _KW_FNCLEX  */
#line 589 "nask.y"
               { (yyval.opcode_) = new OpcodesFNCLEX(); result->opcode_ = (yyval.opcode_); }
#line 2874 "parser.cc"
    break;

  case 137: /* Opcode: _KW_FNDISI  */
#line 590 "nask.y"
               { (yyval.opcode_) = new OpcodesFNDISI(); result->opcode_ = (yyval.opcode_); }
#line 2880 "parser.cc"
    break;

  case 138: /* Opcode: _KW_FNENI  */
#line 591 "nask.y"
              { (yyval.opcode_) = new OpcodesFNENI(); result->opcode_ = (yyval.opcode_); }
#line 2886 "parser.cc"
    break;

  case 139: /* Opcode: _KW_FNINIT  */
#line 592 "nask.y"
               { (yyval.opcode_) = new OpcodesFNINIT(); result->opcode_ = (yyval.opcode_); }
#line 2892 "parser.cc"
    break;

  case 140: /* Opcode: _KW_FNOP  */
#line 593 "nask.y"
             { (yyval.opcode_) = new OpcodesFNOP(); result->opcode_ = (yyval.opcode_); }
#line 2898 "parser.cc"
    break;

  case 141: /* Opcode: _KW_FNSAVE  */
#line 594 "nask.y"
               { (yyval.opcode_) = new OpcodesFNSAVE(); result->opcode_ = (yyval.opcode_); }
#line 2904 "parser.cc"
    break;

  case 142: /* Opcode: _KW_FNSTCW  */
#line 595 "nask.y"
               { (yyval.opcode_) = new OpcodesFNSTCW(); result->opcode_ = (yyval.opcode_); }
#line 2910 "parser.cc"
    break;

  case 143: /* Opcode: _KW_FNSTENV  */
#line 596 "nask.y"
                { (yyval.opcode_) = new OpcodesFNSTENV(); result->opcode_ = (yyval.opcode_); }
#line 2916 "parser.cc"
    break;

  case 144: /* Opcode: _KW_FNSTSW  */
#line 597 "nask.y"
               { (yyval.opcode_) = new OpcodesFNSTSW(); result->opcode_ = (yyval.opcode_); }
#line 2922 "parser.cc"
    break;

  case 145: /* Opcode: _KW_FPATAN  */
#line 598 "nask.y"
               { (yyval.opcode_) = new OpcodesFPATAN(); result->opcode_ = (yyval.opcode_); }
#line 2928 "parser.cc"
    break;

  case 146: /* Opcode: _KW_FPTAN  */
#line 599 "nask.y"
              { (yyval.opcode_) = new OpcodesFPTAN(); result->opcode_ = (yyval.opcode_); }
#line 2934 "parser.cc"
    break;

  case 147: /* Opcode: _KW_FPREM  */
#line 600 "nask.y"
              { (yyval.opcode_) = new OpcodesFPREM(); result->opcode_ = (yyval.opcode_); }
#line 2940 "parser.cc"
    break;

  case 148: /* Opcode: _KW_FPREM1  */
#line 601 "nask.y"
               { (yyval.opcode_) = new OpcodesFPREM1(); result->opcode_ = (yyval.opcode_); }
#line 2946 "parser.cc"
    break;

  case 149: /* Opcode: _KW_FRNDINT  */
#line 602 "nask.y"
                { (yyval.opcode_) = new OpcodesFRNDINT(); result->opcode_ = (yyval.opcode_); }
#line 2952 "parser.cc"
    break;

  case 150: /* Opcode: _KW_FRSTOR  */
#line 603 "nask.y"
               { (yyval.opcode_) = new OpcodesFRSTOR(); result->opcode_ = (yyval.opcode_); }
#line 2958 "parser.cc"
    break;

  case 151: /* Opcode: _KW_FSAVE  */
#line 604 "nask.y"
              { (yyval.opcode_) = new OpcodesFSAVE(); result->opcode_ = (yyval.opcode_); }
#line 2964 "parser.cc"
    break;

  case 152: /* Opcode: _KW_FSCALE  */
#line 605 "nask.y"
               { (yyval.opcode_) = new OpcodesFSCALE(); result->opcode_ = (yyval.opcode_); }
#line 2970 "parser.cc"
    break;

  case 153: /* Opcode: _KW_FSETPM  */
#line 606 "nask.y"
               { (yyval.opcode_) = new OpcodesFSETPM(); result->opcode_ = (yyval.opcode_); }
#line 2976 "parser.cc"
    break;

  case 154: /* Opcode: _KW_FSIN  */
#line 607 "nask.y"
             { (yyval.opcode_) = new OpcodesFSIN(); result->opcode_ = (yyval.opcode_); }
#line 2982 "parser.cc"
    break;

  case 155: /* Opcode: _KW_FSINCOS  */
#line 608 "nask.y"
                { (yyval.opcode_) = new OpcodesFSINCOS(); result->opcode_ = (yyval.opcode_); }
#line 2988 "parser.cc"
    break;

  case 156: /* Opcode: _KW_FSQRT  */
#line 609 "nask.y"
              { (yyval.opcode_) = new OpcodesFSQRT(); result->opcode_ = (yyval.opcode_); }
#line 2994 "parser.cc"
    break;

  case 157: /* Opcode: _KW_FST  */
#line 610 "nask.y"
            { (yyval.opcode_) = new OpcodesFST(); result->opcode_ = (yyval.opcode_); }
#line 3000 "parser.cc"
    break;

  case 158: /* Opcode: _KW_FSTCW  */
#line 611 "nask.y"
              { (yyval.opcode_) = new OpcodesFSTCW(); result->opcode_ = (yyval.opcode_); }
#line 3006 "parser.cc"
    break;

  case 159: /* Opcode: _KW_FSTENV  */
#line 612 "nask.y"
               { (yyval.opcode_) = new OpcodesFSTENV(); result->opcode_ = (yyval.opcode_); }
#line 3012 "parser.cc"
    break;

  case 160: /* Opcode: _KW_FSTP  */
#line 613 "nask.y"
             { (yyval.opcode_) = new OpcodesFSTP(); result->opcode_ = (yyval.opcode_); }
#line 3018 "parser.cc"
    break;

  case 161: /* Opcode: _KW_FSTSW  */
#line 614 "nask.y"
              { (yyval.opcode_) = new OpcodesFSTSW(); result->opcode_ = (yyval.opcode_); }
#line 3024 "parser.cc"
    break;

  case 162: /* Opcode: _KW_FSUB  */
#line 615 "nask.y"
             { (yyval.opcode_) = new OpcodesFSUB(); result->opcode_ = (yyval.opcode_); }
#line 3030 "parser.cc"
    break;

  case 163: /* Opcode: _KW_FSUBP  */
#line 616 "nask.y"
              { (yyval.opcode_) = new OpcodesFSUBP(); result->opcode_ = (yyval.opcode_); }
#line 3036 "parser.cc"
    break;

  case 164: /* Opcode: _KW_FSUBR  */
#line 617 "nask.y"
              { (yyval.opcode_) = new OpcodesFSUBR(); result->opcode_ = (yyval.opcode_); }
#line 3042 "parser.cc"
    break;

  case 165: /* Opcode: _KW_FSUBRP  */
#line 618 "nask.y"
               { (yyval.opcode_) = new OpcodesFSUBRP(); result->opcode_ = (yyval.opcode_); }
#line 3048 "parser.cc"
    break;

  case 166: /* Opcode: _KW_FTST  */
#line 619 "nask.y"
             { (yyval.opcode_) = new OpcodesFTST(); result->opcode_ = (yyval.opcode_); }
#line 3054 "parser.cc"
    break;

  case 167: /* Opcode: _KW_FUCOM  */
#line 620 "nask.y"
              { (yyval.opcode_) = new OpcodesFUCOM(); result->opcode_ = (yyval.opcode_); }
#line 3060 "parser.cc"
    break;

  case 168: /* Opcode: _KW_FUCOMP  */
#line 621 "nask.y"
               { (yyval.opcode_) = new OpcodesFUCOMP(); result->opcode_ = (yyval.opcode_); }
#line 3066 "parser.cc"
    break;

  case 169: /* Opcode: _KW_FUCOMPP  */
#line 622 "nask.y"
                { (yyval.opcode_) = new OpcodesFUCOMPP(); result->opcode_ = (yyval.opcode_); }
#line 3072 "parser.cc"
    break;

  case 170: /* Opcode: _KW_FXAM  */
#line 623 "nask.y"
             { (yyval.opcode_) = new OpcodesFXAM(); result->opcode_ = (yyval.opcode_); }
#line 3078 "parser.cc"
    break;

  case 171: /* Opcode: _KW_FXCH  */
#line 624 "nask.y"
             { (yyval.opcode_) = new OpcodesFXCH(); result->opcode_ = (yyval.opcode_); }
#line 3084 "parser.cc"
    break;

  case 172: /* Opcode: _KW_FXTRACT  */
#line 625 "nask.y"
                { (yyval.opcode_) = new OpcodesFXTRACT(); result->opcode_ = (yyval.opcode_); }
#line 3090 "parser.cc"
    break;

  case 173: /* Opcode: _KW_FYL2X  */
#line 626 "nask.y"
              { (yyval.opcode_) = new OpcodesFYL2X(); result->opcode_ = (yyval.opcode_); }
#line 3096 "parser.cc"
    break;

  case 174: /* Opcode: _KW_FYL2XP1  */
#line 627 "nask.y"
                { (yyval.opcode_) = new OpcodesFYL2XP1(); result->opcode_ = (yyval.opcode_); }
#line 3102 "parser.cc"
    break;

  case 175: /* Opcode: _KW_HLT  */
#line 628 "nask.y"
            { (yyval.opcode_) = new OpcodesHLT(); result->opcode_ = (yyval.opcode_); }
#line 3108 "parser.cc"
    break;

  case 176: /* Opcode: _KW_IDIV  */
#line 629 "nask.y"
             { (yyval.opcode_) = new OpcodesIDIV(); result->opcode_ = (yyval.opcode_); }
#line 3114 "parser.cc"
    break;

  case 177: /* Opcode: _KW_IMUL  */
#line 630 "nask.y"
             { (yyval.opcode_) = new OpcodesIMUL(); result->opcode_ = (yyval.opcode_); }
#line 3120 "parser.cc"
    break;

  case 178: /* Opcode: _KW_IN  */
#line 631 "nask.y"
           { (yyval.opcode_) = new OpcodesIN(); result->opcode_ = (yyval.opcode_); }
#line 3126 "parser.cc"
    break;

  case 179: /* Opcode: _KW_INC  */
#line 632 "nask.y"
            { (yyval.opcode_) = new OpcodesINC(); result->opcode_ = (yyval.opcode_); }
#line 3132 "parser.cc"
    break;

  case 180: /* Opcode: _KW_INCO  */
#line 633 "nask.y"
             { (yyval.opcode_) = new OpcodesINCO(); result->opcode_ = (yyval.opcode_); }
#line 3138 "parser.cc"
    break;

  case 181: /* Opcode: _KW_INSB  */
#line 634 "nask.y"
             { (yyval.opcode_) = new OpcodesINSB(); result->opcode_ = (yyval.opcode_); }
#line 3144 "parser.cc"
    break;

  case 182: /* Opcode: _KW_INSD  */
#line 635 "nask.y"
             { (yyval.opcode_) = new OpcodesINSD(); result->opcode_ = (yyval.opcode_); }
#line 3150 "parser.cc"
    break;

  case 183: /* Opcode: _KW_INSW  */
#line 636 "nask.y"
             { (yyval.opcode_) = new OpcodesINSW(); result->opcode_ = (yyval.opcode_); }
#line 3156 "parser.cc"
    break;

  case 184: /* Opcode: _KW_INT  */
#line 637 "nask.y"
            { (yyval.opcode_) = new OpcodesINT(); result->opcode_ = (yyval.opcode_); }
#line 3162 "parser.cc"
    break;

  case 185: /* Opcode: _KW_INT3  */
#line 638 "nask.y"
             { (yyval.opcode_) = new OpcodesINT3(); result->opcode_ = (yyval.opcode_); }
#line 3168 "parser.cc"
    break;

  case 186: /* Opcode: _KW_INTO  */
#line 639 "nask.y"
             { (yyval.opcode_) = new OpcodesINTO(); result->opcode_ = (yyval.opcode_); }
#line 3174 "parser.cc"
    break;

  case 187: /* Opcode: _KW_INVD  */
#line 640 "nask.y"
             { (yyval.opcode_) = new OpcodesINVD(); result->opcode_ = (yyval.opcode_); }
#line 3180 "parser.cc"
    break;

  case 188: /* Opcode: _KW_INVLPG  */
#line 641 "nask.y"
               { (yyval.opcode_) = new OpcodesINVLPG(); result->opcode_ = (yyval.opcode_); }
#line 3186 "parser.cc"
    break;

  case 189: /* Opcode: _KW_IRET  */
#line 642 "nask.y"
             { (yyval.opcode_) = new OpcodesIRET(); result->opcode_ = (yyval.opcode_); }
#line 3192 "parser.cc"
    break;

  case 190: /* Opcode: _KW_IRETD  */
#line 643 "nask.y"
              { (yyval.opcode_) = new OpcodesIRETD(); result->opcode_ = (yyval.opcode_); }
#line 3198 "parser.cc"
    break;

  case 191: /* Opcode: _KW_IRETW  */
#line 644 "nask.y"
              { (yyval.opcode_) = new OpcodesIRETW(); result->opcode_ = (yyval.opcode_); }
#line 3204 "parser.cc"
    break;

  case 192: /* Opcode: _KW_JA  */
#line 645 "nask.y"
           { (yyval.opcode_) = new OpcodesJA(); result->opcode_ = (yyval.opcode_); }
#line 3210 "parser.cc"
    break;

  case 193: /* Opcode: _KW_JAE  */
#line 646 "nask.y"
            { (yyval.opcode_) = new OpcodesJAE(); result->opcode_ = (yyval.opcode_); }
#line 3216 "parser.cc"
    break;

  case 194: /* Opcode: _KW_JB  */
#line 647 "nask.y"
           { (yyval.opcode_) = new OpcodesJB(); result->opcode_ = (yyval.opcode_); }
#line 3222 "parser.cc"
    break;

  case 195: /* Opcode: _KW_JBE  */
#line 648 "nask.y"
            { (yyval.opcode_) = new OpcodesJBE(); result->opcode_ = (yyval.opcode_); }
#line 3228 "parser.cc"
    break;

  case 196: /* Opcode: _KW_JC  */
#line 649 "nask.y"
           { (yyval.opcode_) = new OpcodesJC(); result->opcode_ = (yyval.opcode_); }
#line 3234 "parser.cc"
    break;

  case 197: /* Opcode: _KW_JCXZ  */
#line 650 "nask.y"
             { (yyval.opcode_) = new OpcodesJCXZ(); result->opcode_ = (yyval.opcode_); }
#line 3240 "parser.cc"
    break;

  case 198: /* Opcode: _KW_JE  */
#line 651 "nask.y"
           { (yyval.opcode_) = new OpcodesJE(); result->opcode_ = (yyval.opcode_); }
#line 3246 "parser.cc"
    break;

  case 199: /* Opcode: _KW_JECXZ  */
#line 652 "nask.y"
              { (yyval.opcode_) = new OpcodesJECXZ(); result->opcode_ = (yyval.opcode_); }
#line 3252 "parser.cc"
    break;

  case 200: /* Opcode: _KW_JG  */
#line 653 "nask.y"
           { (yyval.opcode_) = new OpcodesJG(); result->opcode_ = (yyval.opcode_); }
#line 3258 "parser.cc"
    break;

  case 201: /* Opcode: _KW_JGE  */
#line 654 "nask.y"
            { (yyval.opcode_) = new OpcodesJGE(); result->opcode_ = (yyval.opcode_); }
#line 3264 "parser.cc"
    break;

  case 202: /* Opcode: _KW_JL  */
#line 655 "nask.y"
           { (yyval.opcode_) = new OpcodesJL(); result->opcode_ = (yyval.opcode_); }
#line 3270 "parser.cc"
    break;

  case 203: /* Opcode: _KW_JLE  */
#line 656 "nask.y"
            { (yyval.opcode_) = new OpcodesJLE(); result->opcode_ = (yyval.opcode_); }
#line 3276 "parser.cc"
    break;

  case 204: /* Opcode: _KW_JMP  */
#line 657 "nask.y"
            { (yyval.opcode_) = new OpcodesJMP(); result->opcode_ = (yyval.opcode_); }
#line 3282 "parser.cc"
    break;

  case 205: /* Opcode: _KW_JNA  */
#line 658 "nask.y"
            { (yyval.opcode_) = new OpcodesJNA(); result->opcode_ = (yyval.opcode_); }
#line 3288 "parser.cc"
    break;

  case 206: /* Opcode: _KW_JNAE  */
#line 659 "nask.y"
             { (yyval.opcode_) = new OpcodesJNAE(); result->opcode_ = (yyval.opcode_); }
#line 3294 "parser.cc"
    break;

  case 207: /* Opcode: _KW_JNB  */
#line 660 "nask.y"
            { (yyval.opcode_) = new OpcodesJNB(); result->opcode_ = (yyval.opcode_); }
#line 3300 "parser.cc"
    break;

  case 208: /* Opcode: _KW_JNBE  */
#line 661 "nask.y"
             { (yyval.opcode_) = new OpcodesJNBE(); result->opcode_ = (yyval.opcode_); }
#line 3306 "parser.cc"
    break;

  case 209: /* Opcode: _KW_JNC  */
#line 662 "nask.y"
            { (yyval.opcode_) = new OpcodesJNC(); result->opcode_ = (yyval.opcode_); }
#line 3312 "parser.cc"
    break;

  case 210: /* Opcode: _KW_JNE  */
#line 663 "nask.y"
            { (yyval.opcode_) = new OpcodesJNE(); result->opcode_ = (yyval.opcode_); }
#line 3318 "parser.cc"
    break;

  case 211: /* Opcode: _KW_JNG  */
#line 664 "nask.y"
            { (yyval.opcode_) = new OpcodesJNG(); result->opcode_ = (yyval.opcode_); }
#line 3324 "parser.cc"
    break;

  case 212: /* Opcode: _KW_JNGE  */
#line 665 "nask.y"
             { (yyval.opcode_) = new OpcodesJNGE(); result->opcode_ = (yyval.opcode_); }
#line 3330 "parser.cc"
    break;

  case 213: /* Opcode: _KW_JNL  */
#line 666 "nask.y"
            { (yyval.opcode_) = new OpcodesJNL(); result->opcode_ = (yyval.opcode_); }
#line 3336 "parser.cc"
    break;

  case 214: /* Opcode: _KW_JNLE  */
#line 667 "nask.y"
             { (yyval.opcode_) = new OpcodesJNLE(); result->opcode_ = (yyval.opcode_); }
#line 3342 "parser.cc"
    break;

  case 215: /* Opcode: _KW_JNO  */
#line 668 "nask.y"
            { (yyval.opcode_) = new OpcodesJNO(); result->opcode_ = (yyval.opcode_); }
#line 3348 "parser.cc"
    break;

  case 216: /* Opcode: _KW_JNP  */
#line 669 "nask.y"
            { (yyval.opcode_) = new OpcodesJNP(); result->opcode_ = (yyval.opcode_); }
#line 3354 "parser.cc"
    break;

  case 217: /* Opcode: _KW_JNS  */
#line 670 "nask.y"
            { (yyval.opcode_) = new OpcodesJNS(); result->opcode_ = (yyval.opcode_); }
#line 3360 "parser.cc"
    break;

  case 218: /* Opcode: _KW_JNZ  */
#line 671 "nask.y"
            { (yyval.opcode_) = new OpcodesJNZ(); result->opcode_ = (yyval.opcode_); }
#line 3366 "parser.cc"
    break;

  case 219: /* Opcode: _KW_JO  */
#line 672 "nask.y"
           { (yyval.opcode_) = new OpcodesJO(); result->opcode_ = (yyval.opcode_); }
#line 3372 "parser.cc"
    break;

  case 220: /* Opcode: _KW_JP  */
#line 673 "nask.y"
           { (yyval.opcode_) = new OpcodesJP(); result->opcode_ = (yyval.opcode_); }
#line 3378 "parser.cc"
    break;

  case 221: /* Opcode: _KW_JPE  */
#line 674 "nask.y"
            { (yyval.opcode_) = new OpcodesJPE(); result->opcode_ = (yyval.opcode_); }
#line 3384 "parser.cc"
    break;

  case 222: /* Opcode: _KW_JPO  */
#line 675 "nask.y"
            { (yyval.opcode_) = new OpcodesJPO(); result->opcode_ = (yyval.opcode_); }
#line 3390 "parser.cc"
    break;

  case 223: /* Opcode: _KW_JS  */
#line 676 "nask.y"
           { (yyval.opcode_) = new OpcodesJS(); result->opcode_ = (yyval.opcode_); }
#line 3396 "parser.cc"
    break;

  case 224: /* Opcode: _KW_JZ  */
#line 677 "nask.y"
           { (yyval.opcode_) = new OpcodesJZ(); result->opcode_ = (yyval.opcode_); }
#line 3402 "parser.cc"
    break;

  case 225: /* Opcode: _KW_LAHF  */
#line 678 "nask.y"
             { (yyval.opcode_) = new OpcodesLAHF(); result->opcode_ = (yyval.opcode_); }
#line 3408 "parser.cc"
    break;

  case 226: /* Opcode: _KW_LAR  */
#line 679 "nask.y"
            { (yyval.opcode_) = new OpcodesLAR(); result->opcode_ = (yyval.opcode_); }
#line 3414 "parser.cc"
    break;

  case 227: /* Opcode: _KW_LDS  */
#line 680 "nask.y"
            { (yyval.opcode_) = new OpcodesLDS(); result->opcode_ = (yyval.opcode_); }
#line 3420 "parser.cc"
    break;

  case 228: /* Opcode: _KW_LEA  */
#line 681 "nask.y"
            { (yyval.opcode_) = new OpcodesLEA(); result->opcode_ = (yyval.opcode_); }
#line 3426 "parser.cc"
    break;

  case 229: /* Opcode: _KW_LEAVE  */
#line 682 "nask.y"
              { (yyval.opcode_) = new OpcodesLEAVE(); result->opcode_ = (yyval.opcode_); }
#line 3432 "parser.cc"
    break;

  case 230: /* Opcode: _KW_LES  */
#line 683 "nask.y"
            { (yyval.opcode_) = new OpcodesLES(); result->opcode_ = (yyval.opcode_); }
#line 3438 "parser.cc"
    break;

  case 231: /* Opcode: _KW_LFS  */
#line 684 "nask.y"
            { (yyval.opcode_) = new OpcodesLFS(); result->opcode_ = (yyval.opcode_); }
#line 3444 "parser.cc"
    break;

  case 232: /* Opcode: _KW_LGDT  */
#line 685 "nask.y"
             { (yyval.opcode_) = new OpcodesLGDT(); result->opcode_ = (yyval.opcode_); }
#line 3450 "parser.cc"
    break;

  case 233: /* Opcode: _KW_LGS  */
#line 686 "nask.y"
            { (yyval.opcode_) = new OpcodesLGS(); result->opcode_ = (yyval.opcode_); }
#line 3456 "parser.cc"
    break;

  case 234: /* Opcode: _KW_LIDT  */
#line 687 "nask.y"
             { (yyval.opcode_) = new OpcodesLIDT(); result->opcode_ = (yyval.opcode_); }
#line 3462 "parser.cc"
    break;

  case 235: /* Opcode: _KW_LLDT  */
#line 688 "nask.y"
             { (yyval.opcode_) = new OpcodesLLDT(); result->opcode_ = (yyval.opcode_); }
#line 3468 "parser.cc"
    break;

  case 236: /* Opcode: _KW_LMSW  */
#line 689 "nask.y"
             { (yyval.opcode_) = new OpcodesLMSW(); result->opcode_ = (yyval.opcode_); }
#line 3474 "parser.cc"
    break;

  case 237: /* Opcode: _KW_LOCK  */
#line 690 "nask.y"
             { (yyval.opcode_) = new OpcodesLOCK(); result->opcode_ = (yyval.opcode_); }
#line 3480 "parser.cc"
    break;

  case 238: /* Opcode: _KW_LODSB  */
#line 691 "nask.y"
              { (yyval.opcode_) = new OpcodesLODSB(); result->opcode_ = (yyval.opcode_); }
#line 3486 "parser.cc"
    break;

  case 239: /* Opcode: _KW_LODSD  */
#line 692 "nask.y"
              { (yyval.opcode_) = new OpcodesLODSD(); result->opcode_ = (yyval.opcode_); }
#line 3492 "parser.cc"
    break;

  case 240: /* Opcode: _KW_LODSW  */
#line 693 "nask.y"
              { (yyval.opcode_) = new OpcodesLODSW(); result->opcode_ = (yyval.opcode_); }
#line 3498 "parser.cc"
    break;

  case 241: /* Opcode: _KW_LOOP  */
#line 694 "nask.y"
             { (yyval.opcode_) = new OpcodesLOOP(); result->opcode_ = (yyval.opcode_); }
#line 3504 "parser.cc"
    break;

  case 242: /* Opcode: _KW_LOOPE  */
#line 695 "nask.y"
              { (yyval.opcode_) = new OpcodesLOOPE(); result->opcode_ = (yyval.opcode_); }
#line 3510 "parser.cc"
    break;

  case 243: /* Opcode: _KW_LOOPNE  */
#line 696 "nask.y"
               { (yyval.opcode_) = new OpcodesLOOPNE(); result->opcode_ = (yyval.opcode_); }
#line 3516 "parser.cc"
    break;

  case 244: /* Opcode: _KW_LOOPNZ  */
#line 697 "nask.y"
               { (yyval.opcode_) = new OpcodesLOOPNZ(); result->opcode_ = (yyval.opcode_); }
#line 3522 "parser.cc"
    break;

  case 245: /* Opcode: _KW_LOOPZ  */
#line 698 "nask.y"
              { (yyval.opcode_) = new OpcodesLOOPZ(); result->opcode_ = (yyval.opcode_); }
#line 3528 "parser.cc"
    break;

  case 246: /* Opcode: _KW_LSL  */
#line 699 "nask.y"
            { (yyval.opcode_) = new OpcodesLSL(); result->opcode_ = (yyval.opcode_); }
#line 3534 "parser.cc"
    break;

  case 247: /* Opcode: _KW_LSS  */
#line 700 "nask.y"
            { (yyval.opcode_) = new OpcodesLSS(); result->opcode_ = (yyval.opcode_); }
#line 3540 "parser.cc"
    break;

  case 248: /* Opcode: _KW_LTR  */
#line 701 "nask.y"
            { (yyval.opcode_) = new OpcodesLTR(); result->opcode_ = (yyval.opcode_); }
#line 3546 "parser.cc"
    break;

  case 249: /* Opcode: _KW_MOV  */
#line 702 "nask.y"
            { (yyval.opcode_) = new OpcodesMOV(); result->opcode_ = (yyval.opcode_); }
#line 3552 "parser.cc"
    break;

  case 250: /* Opcode: _KW_MOVSB  */
#line 703 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSB(); result->opcode_ = (yyval.opcode_); }
#line 3558 "parser.cc"
    break;

  case 251: /* Opcode: _KW_MOVSD  */
#line 704 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSD(); result->opcode_ = (yyval.opcode_); }
#line 3564 "parser.cc"
    break;

  case 252: /* Opcode: _KW_MOVSW  */
#line 705 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSW(); result->opcode_ = (yyval.opcode_); }
#line 3570 "parser.cc"
    break;

  case 253: /* Opcode: _KW_MOVSX  */
#line 706 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVSX(); result->opcode_ = (yyval.opcode_); }
#line 3576 "parser.cc"
    break;

  case 254: /* Opcode: _KW_MOVZX  */
#line 707 "nask.y"
              { (yyval.opcode_) = new OpcodesMOVZX(); result->opcode_ = (yyval.opcode_); }
#line 3582 "parser.cc"
    break;

  case 255: /* Opcode: _KW_MUL  */
#line 708 "nask.y"
            { (yyval.opcode_) = new OpcodesMUL(); result->opcode_ = (yyval.opcode_); }
#line 3588 "parser.cc"
    break;

  case 256: /* Opcode: _KW_NEG  */
#line 709 "nask.y"
            { (yyval.opcode_) = new OpcodesNEG(); result->opcode_ = (yyval.opcode_); }
#line 3594 "parser.cc"
    break;

  case 257: /* Opcode: _KW_NOP  */
#line 710 "nask.y"
            { (yyval.opcode_) = new OpcodesNOP(); result->opcode_ = (yyval.opcode_); }
#line 3600 "parser.cc"
    break;

  case 258: /* Opcode: _KW_NOT  */
#line 711 "nask.y"
            { (yyval.opcode_) = new OpcodesNOT(); result->opcode_ = (yyval.opcode_); }
#line 3606 "parser.cc"
    break;

  case 259: /* Opcode: _KW_OR  */
#line 712 "nask.y"
           { (yyval.opcode_) = new OpcodesOR(); result->opcode_ = (yyval.opcode_); }
#line 3612 "parser.cc"
    break;

  case 260: /* Opcode: _KW_ORG  */
#line 713 "nask.y"
            { (yyval.opcode_) = new OpcodesORG(); result->opcode_ = (yyval.opcode_); }
#line 3618 "parser.cc"
    break;

  case 261: /* Opcode: _KW_OUT  */
#line 714 "nask.y"
            { (yyval.opcode_) = new OpcodesOUT(); result->opcode_ = (yyval.opcode_); }
#line 3624 "parser.cc"
    break;

  case 262: /* Opcode: _KW_OUTSB  */
#line 715 "nask.y"
              { (yyval.opcode_) = new OpcodesOUTSB(); result->opcode_ = (yyval.opcode_); }
#line 3630 "parser.cc"
    break;

  case 263: /* Opcode: _KW_OUTSD  */
#line 716 "nask.y"
              { (yyval.opcode_) = new OpcodesOUTSD(); result->opcode_ = (yyval.opcode_); }
#line 3636 "parser.cc"
    break;

  case 264: /* Opcode: _KW_OUTSW  */
#line 717 "nask.y"
              { (yyval.opcode_) = new OpcodesOUTSW(); result->opcode_ = (yyval.opcode_); }
#line 3642 "parser.cc"
    break;

  case 265: /* Opcode: _KW_POP  */
#line 718 "nask.y"
            { (yyval.opcode_) = new OpcodesPOP(); result->opcode_ = (yyval.opcode_); }
#line 3648 "parser.cc"
    break;

  case 266: /* Opcode: _KW_POPA  */
#line 719 "nask.y"
             { (yyval.opcode_) = new OpcodesPOPA(); result->opcode_ = (yyval.opcode_); }
#line 3654 "parser.cc"
    break;

  case 267: /* Opcode: _KW_POPAD  */
#line 720 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPAD(); result->opcode_ = (yyval.opcode_); }
#line 3660 "parser.cc"
    break;

  case 268: /* Opcode: _KW_POPAW  */
#line 721 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPAW(); result->opcode_ = (yyval.opcode_); }
#line 3666 "parser.cc"
    break;

  case 269: /* Opcode: _KW_POPF  */
#line 722 "nask.y"
             { (yyval.opcode_) = new OpcodesPOPF(); result->opcode_ = (yyval.opcode_); }
#line 3672 "parser.cc"
    break;

  case 270: /* Opcode: _KW_POPFD  */
#line 723 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPFD(); result->opcode_ = (yyval.opcode_); }
#line 3678 "parser.cc"
    break;

  case 271: /* Opcode: _KW_POPFW  */
#line 724 "nask.y"
              { (yyval.opcode_) = new OpcodesPOPFW(); result->opcode_ = (yyval.opcode_); }
#line 3684 "parser.cc"
    break;

  case 272: /* Opcode: _KW_PUSH  */
#line 725 "nask.y"
             { (yyval.opcode_) = new OpcodesPUSH(); result->opcode_ = (yyval.opcode_); }
#line 3690 "parser.cc"
    break;

  case 273: /* Opcode: _KW_PUSHA  */
#line 726 "nask.y"
              { (yyval.opcode_) = new OpcodesPUSHA(); result->opcode_ = (yyval.opcode_); }
#line 3696 "parser.cc"
    break;

  case 274: /* Opcode: _KW_PUSHD  */
#line 727 "nask.y"
              { (yyval.opcode_) = new OpcodesPUSHD(); result->opcode_ = (yyval.opcode_); }
#line 3702 "parser.cc"
    break;

  case 275: /* Opcode: _KW_PUSHAD  */
#line 728 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHAD(); result->opcode_ = (yyval.opcode_); }
#line 3708 "parser.cc"
    break;

  case 276: /* Opcode: _KW_PUSHAW  */
#line 729 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHAW(); result->opcode_ = (yyval.opcode_); }
#line 3714 "parser.cc"
    break;

  case 277: /* Opcode: _KW_PUSHF  */
#line 730 "nask.y"
              { (yyval.opcode_) = new OpcodesPUSHF(); result->opcode_ = (yyval.opcode_); }
#line 3720 "parser.cc"
    break;

  case 278: /* Opcode: _KW_PUSHFD  */
#line 731 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHFD(); result->opcode_ = (yyval.opcode_); }
#line 3726 "parser.cc"
    break;

  case 279: /* Opcode: _KW_PUSHFW  */
#line 732 "nask.y"
               { (yyval.opcode_) = new OpcodesPUSHFW(); result->opcode_ = (yyval.opcode_); }
#line 3732 "parser.cc"
    break;

  case 280: /* Opcode: _KW_RCL  */
#line 733 "nask.y"
            { (yyval.opcode_) = new OpcodesRCL(); result->opcode_ = (yyval.opcode_); }
#line 3738 "parser.cc"
    break;

  case 281: /* Opcode: _KW_RCR  */
#line 734 "nask.y"
            { (yyval.opcode_) = new OpcodesRCR(); result->opcode_ = (yyval.opcode_); }
#line 3744 "parser.cc"
    break;

  case 282: /* Opcode: _KW_RDMSR  */
#line 735 "nask.y"
              { (yyval.opcode_) = new OpcodesRDMSR(); result->opcode_ = (yyval.opcode_); }
#line 3750 "parser.cc"
    break;

  case 283: /* Opcode: _KW_RDPMC  */
#line 736 "nask.y"
              { (yyval.opcode_) = new OpcodesRDPMC(); result->opcode_ = (yyval.opcode_); }
#line 3756 "parser.cc"
    break;

  case 284: /* Opcode: _KW_REP  */
#line 737 "nask.y"
            { (yyval.opcode_) = new OpcodesREP(); result->opcode_ = (yyval.opcode_); }
#line 3762 "parser.cc"
    break;

  case 285: /* Opcode: _KW_REPE  */
#line 738 "nask.y"
             { (yyval.opcode_) = new OpcodesREPE(); result->opcode_ = (yyval.opcode_); }
#line 3768 "parser.cc"
    break;

  case 286: /* Opcode: _KW_REPNE  */
#line 739 "nask.y"
              { (yyval.opcode_) = new OpcodesREPNE(); result->opcode_ = (yyval.opcode_); }
#line 3774 "parser.cc"
    break;

  case 287: /* Opcode: _KW_REPNZ  */
#line 740 "nask.y"
              { (yyval.opcode_) = new OpcodesREPNZ(); result->opcode_ = (yyval.opcode_); }
#line 3780 "parser.cc"
    break;

  case 288: /* Opcode: _KW_REPZ  */
#line 741 "nask.y"
             { (yyval.opcode_) = new OpcodesREPZ(); result->opcode_ = (yyval.opcode_); }
#line 3786 "parser.cc"
    break;

  case 289: /* Opcode: _KW_RESB  */
#line 742 "nask.y"
             { (yyval.opcode_) = new OpcodesRESB(); result->opcode_ = (yyval.opcode_); }
#line 3792 "parser.cc"
    break;

  case 290: /* Opcode: _KW_RESD  */
#line 743 "nask.y"
             { (yyval.opcode_) = new OpcodesRESD(); result->opcode_ = (yyval.opcode_); }
#line 3798 "parser.cc"
    break;

  case 291: /* Opcode: _KW_RESQ  */
#line 744 "nask.y"
             { (yyval.opcode_) = new OpcodesRESQ(); result->opcode_ = (yyval.opcode_); }
#line 3804 "parser.cc"
    break;

  case 292: /* Opcode: _KW_REST  */
#line 745 "nask.y"
             { (yyval.opcode_) = new OpcodesREST(); result->opcode_ = (yyval.opcode_); }
#line 3810 "parser.cc"
    break;

  case 293: /* Opcode: _KW_RESW  */
#line 746 "nask.y"
             { (yyval.opcode_) = new OpcodesRESW(); result->opcode_ = (yyval.opcode_); }
#line 3816 "parser.cc"
    break;

  case 294: /* Opcode: _KW_RET  */
#line 747 "nask.y"
            { (yyval.opcode_) = new OpcodesRET(); result->opcode_ = (yyval.opcode_); }
#line 3822 "parser.cc"
    break;

  case 295: /* Opcode: _KW_RETF  */
#line 748 "nask.y"
             { (yyval.opcode_) = new OpcodesRETF(); result->opcode_ = (yyval.opcode_); }
#line 3828 "parser.cc"
    break;

  case 296: /* Opcode: _KW_RETN  */
#line 749 "nask.y"
             { (yyval.opcode_) = new OpcodesRETN(); result->opcode_ = (yyval.opcode_); }
#line 3834 "parser.cc"
    break;

  case 297: /* Opcode: _KW_ROL  */
#line 750 "nask.y"
            { (yyval.opcode_) = new OpcodesROL(); result->opcode_ = (yyval.opcode_); }
#line 3840 "parser.cc"
    break;

  case 298: /* Opcode: _KW_ROR  */
#line 751 "nask.y"
            { (yyval.opcode_) = new OpcodesROR(); result->opcode_ = (yyval.opcode_); }
#line 3846 "parser.cc"
    break;

  case 299: /* Opcode: _KW_RSM  */
#line 752 "nask.y"
            { (yyval.opcode_) = new OpcodesRSM(); result->opcode_ = (yyval.opcode_); }
#line 3852 "parser.cc"
    break;

  case 300: /* Opcode: _KW_SAHF  */
#line 753 "nask.y"
             { (yyval.opcode_) = new OpcodesSAHF(); result->opcode_ = (yyval.opcode_); }
#line 3858 "parser.cc"
    break;

  case 301: /* Opcode: _KW_SAL  */
#line 754 "nask.y"
            { (yyval.opcode_) = new OpcodesSAL(); result->opcode_ = (yyval.opcode_); }
#line 3864 "parser.cc"
    break;

  case 302: /* Opcode: _KW_SAR  */
#line 755 "nask.y"
            { (yyval.opcode_) = new OpcodesSAR(); result->opcode_ = (yyval.opcode_); }
#line 3870 "parser.cc"
    break;

  case 303: /* Opcode: _KW_SBB  */
#line 756 "nask.y"
            { (yyval.opcode_) = new OpcodesSBB(); result->opcode_ = (yyval.opcode_); }
#line 3876 "parser.cc"
    break;

  case 304: /* Opcode: _KW_SCASB  */
#line 757 "nask.y"
              { (yyval.opcode_) = new OpcodesSCASB(); result->opcode_ = (yyval.opcode_); }
#line 3882 "parser.cc"
    break;

  case 305: /* Opcode: _KW_SCASD  */
#line 758 "nask.y"
              { (yyval.opcode_) = new OpcodesSCASD(); result->opcode_ = (yyval.opcode_); }
#line 3888 "parser.cc"
    break;

  case 306: /* Opcode: _KW_SCASW  */
#line 759 "nask.y"
              { (yyval.opcode_) = new OpcodesSCASW(); result->opcode_ = (yyval.opcode_); }
#line 3894 "parser.cc"
    break;

  case 307: /* Opcode: _KW_SETA  */
#line 760 "nask.y"
             { (yyval.opcode_) = new OpcodesSETA(); result->opcode_ = (yyval.opcode_); }
#line 3900 "parser.cc"
    break;

  case 308: /* Opcode: _KW_SETAE  */
#line 761 "nask.y"
              { (yyval.opcode_) = new OpcodesSETAE(); result->opcode_ = (yyval.opcode_); }
#line 3906 "parser.cc"
    break;

  case 309: /* Opcode: _KW_SETB  */
#line 762 "nask.y"
             { (yyval.opcode_) = new OpcodesSETB(); result->opcode_ = (yyval.opcode_); }
#line 3912 "parser.cc"
    break;

  case 310: /* Opcode: _KW_SETBE  */
#line 763 "nask.y"
              { (yyval.opcode_) = new OpcodesSETBE(); result->opcode_ = (yyval.opcode_); }
#line 3918 "parser.cc"
    break;

  case 311: /* Opcode: _KW_SETC  */
#line 764 "nask.y"
             { (yyval.opcode_) = new OpcodesSETC(); result->opcode_ = (yyval.opcode_); }
#line 3924 "parser.cc"
    break;

  case 312: /* Opcode: _KW_SETE  */
#line 765 "nask.y"
             { (yyval.opcode_) = new OpcodesSETE(); result->opcode_ = (yyval.opcode_); }
#line 3930 "parser.cc"
    break;

  case 313: /* Opcode: _KW_SETG  */
#line 766 "nask.y"
             { (yyval.opcode_) = new OpcodesSETG(); result->opcode_ = (yyval.opcode_); }
#line 3936 "parser.cc"
    break;

  case 314: /* Opcode: _KW_SETGE  */
#line 767 "nask.y"
              { (yyval.opcode_) = new OpcodesSETGE(); result->opcode_ = (yyval.opcode_); }
#line 3942 "parser.cc"
    break;

  case 315: /* Opcode: _KW_SETL  */
#line 768 "nask.y"
             { (yyval.opcode_) = new OpcodesSETL(); result->opcode_ = (yyval.opcode_); }
#line 3948 "parser.cc"
    break;

  case 316: /* Opcode: _KW_SETLE  */
#line 769 "nask.y"
              { (yyval.opcode_) = new OpcodesSETLE(); result->opcode_ = (yyval.opcode_); }
#line 3954 "parser.cc"
    break;

  case 317: /* Opcode: _KW_SETNA  */
#line 770 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNA(); result->opcode_ = (yyval.opcode_); }
#line 3960 "parser.cc"
    break;

  case 318: /* Opcode: _KW_SETNAE  */
#line 771 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNAE(); result->opcode_ = (yyval.opcode_); }
#line 3966 "parser.cc"
    break;

  case 319: /* Opcode: _KW_SETNB  */
#line 772 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNB(); result->opcode_ = (yyval.opcode_); }
#line 3972 "parser.cc"
    break;

  case 320: /* Opcode: _KW_SETNBE  */
#line 773 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNBE(); result->opcode_ = (yyval.opcode_); }
#line 3978 "parser.cc"
    break;

  case 321: /* Opcode: _KW_SETNC  */
#line 774 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNC(); result->opcode_ = (yyval.opcode_); }
#line 3984 "parser.cc"
    break;

  case 322: /* Opcode: _KW_SETNE  */
#line 775 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNE(); result->opcode_ = (yyval.opcode_); }
#line 3990 "parser.cc"
    break;

  case 323: /* Opcode: _KW_SETNG  */
#line 776 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNG(); result->opcode_ = (yyval.opcode_); }
#line 3996 "parser.cc"
    break;

  case 324: /* Opcode: _KW_SETNGE  */
#line 777 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNGE(); result->opcode_ = (yyval.opcode_); }
#line 4002 "parser.cc"
    break;

  case 325: /* Opcode: _KW_SETNL  */
#line 778 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNL(); result->opcode_ = (yyval.opcode_); }
#line 4008 "parser.cc"
    break;

  case 326: /* Opcode: _KW_SETNLE  */
#line 779 "nask.y"
               { (yyval.opcode_) = new OpcodesSETNLE(); result->opcode_ = (yyval.opcode_); }
#line 4014 "parser.cc"
    break;

  case 327: /* Opcode: _KW_SETNO  */
#line 780 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNO(); result->opcode_ = (yyval.opcode_); }
#line 4020 "parser.cc"
    break;

  case 328: /* Opcode: _KW_SETNP  */
#line 781 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNP(); result->opcode_ = (yyval.opcode_); }
#line 4026 "parser.cc"
    break;

  case 329: /* Opcode: _KW_SETNS  */
#line 782 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNS(); result->opcode_ = (yyval.opcode_); }
#line 4032 "parser.cc"
    break;

  case 330: /* Opcode: _KW_SETNZ  */
#line 783 "nask.y"
              { (yyval.opcode_) = new OpcodesSETNZ(); result->opcode_ = (yyval.opcode_); }
#line 4038 "parser.cc"
    break;

  case 331: /* Opcode: _KW_SETO  */
#line 784 "nask.y"
             { (yyval.opcode_) = new OpcodesSETO(); result->opcode_ = (yyval.opcode_); }
#line 4044 "parser.cc"
    break;

  case 332: /* Opcode: _KW_SETP  */
#line 785 "nask.y"
             { (yyval.opcode_) = new OpcodesSETP(); result->opcode_ = (yyval.opcode_); }
#line 4050 "parser.cc"
    break;

  case 333: /* Opcode: _KW_SETPE  */
#line 786 "nask.y"
              { (yyval.opcode_) = new OpcodesSETPE(); result->opcode_ = (yyval.opcode_); }
#line 4056 "parser.cc"
    break;

  case 334: /* Opcode: _KW_SETPO  */
#line 787 "nask.y"
              { (yyval.opcode_) = new OpcodesSETPO(); result->opcode_ = (yyval.opcode_); }
#line 4062 "parser.cc"
    break;

  case 335: /* Opcode: _KW_SETS  */
#line 788 "nask.y"
             { (yyval.opcode_) = new OpcodesSETS(); result->opcode_ = (yyval.opcode_); }
#line 4068 "parser.cc"
    break;

  case 336: /* Opcode: _KW_SETZ  */
#line 789 "nask.y"
             { (yyval.opcode_) = new OpcodesSETZ(); result->opcode_ = (yyval.opcode_); }
#line 4074 "parser.cc"
    break;

  case 337: /* Opcode: _KW_SGDT  */
#line 790 "nask.y"
             { (yyval.opcode_) = new OpcodesSGDT(); result->opcode_ = (yyval.opcode_); }
#line 4080 "parser.cc"
    break;

  case 338: /* Opcode: _KW_SHL  */
#line 791 "nask.y"
            { (yyval.opcode_) = new OpcodesSHL(); result->opcode_ = (yyval.opcode_); }
#line 4086 "parser.cc"
    break;

  case 339: /* Opcode: _KW_SHLD  */
#line 792 "nask.y"
             { (yyval.opcode_) = new OpcodesSHLD(); result->opcode_ = (yyval.opcode_); }
#line 4092 "parser.cc"
    break;

  case 340: /* Opcode: _KW_SHR  */
#line 793 "nask.y"
            { (yyval.opcode_) = new OpcodesSHR(); result->opcode_ = (yyval.opcode_); }
#line 4098 "parser.cc"
    break;

  case 341: /* Opcode: _KW_SHRD  */
#line 794 "nask.y"
             { (yyval.opcode_) = new OpcodesSHRD(); result->opcode_ = (yyval.opcode_); }
#line 4104 "parser.cc"
    break;

  case 342: /* Opcode: _KW_SIDT  */
#line 795 "nask.y"
             { (yyval.opcode_) = new OpcodesSIDT(); result->opcode_ = (yyval.opcode_); }
#line 4110 "parser.cc"
    break;

  case 343: /* Opcode: _KW_SLDT  */
#line 796 "nask.y"
             { (yyval.opcode_) = new OpcodesSLDT(); result->opcode_ = (yyval.opcode_); }
#line 4116 "parser.cc"
    break;

  case 344: /* Opcode: _KW_SMSW  */
#line 797 "nask.y"
             { (yyval.opcode_) = new OpcodesSMSW(); result->opcode_ = (yyval.opcode_); }
#line 4122 "parser.cc"
    break;

  case 345: /* Opcode: _KW_STC  */
#line 798 "nask.y"
            { (yyval.opcode_) = new OpcodesSTC(); result->opcode_ = (yyval.opcode_); }
#line 4128 "parser.cc"
    break;

  case 346: /* Opcode: _KW_STD  */
#line 799 "nask.y"
            { (yyval.opcode_) = new OpcodesSTD(); result->opcode_ = (yyval.opcode_); }
#line 4134 "parser.cc"
    break;

  case 347: /* Opcode: _KW_STI  */
#line 800 "nask.y"
            { (yyval.opcode_) = new OpcodesSTI(); result->opcode_ = (yyval.opcode_); }
#line 4140 "parser.cc"
    break;

  case 348: /* Opcode: _KW_STOSB  */
#line 801 "nask.y"
              { (yyval.opcode_) = new OpcodesSTOSB(); result->opcode_ = (yyval.opcode_); }
#line 4146 "parser.cc"
    break;

  case 349: /* Opcode: _KW_STOSD  */
#line 802 "nask.y"
              { (yyval.opcode_) = new OpcodesSTOSD(); result->opcode_ = (yyval.opcode_); }
#line 4152 "parser.cc"
    break;

  case 350: /* Opcode: _KW_STOSW  */
#line 803 "nask.y"
              { (yyval.opcode_) = new OpcodesSTOSW(); result->opcode_ = (yyval.opcode_); }
#line 4158 "parser.cc"
    break;

  case 351: /* Opcode: _KW_STR  */
#line 804 "nask.y"
            { (yyval.opcode_) = new OpcodesSTR(); result->opcode_ = (yyval.opcode_); }
#line 4164 "parser.cc"
    break;

  case 352: /* Opcode: _KW_SUB  */
#line 805 "nask.y"
            { (yyval.opcode_) = new OpcodesSUB(); result->opcode_ = (yyval.opcode_); }
#line 4170 "parser.cc"
    break;

  case 353: /* Opcode: _KW_TEST  */
#line 806 "nask.y"
             { (yyval.opcode_) = new OpcodesTEST(); result->opcode_ = (yyval.opcode_); }
#line 4176 "parser.cc"
    break;

  case 354: /* Opcode: _KW_TIMES  */
#line 807 "nask.y"
              { (yyval.opcode_) = new OpcodesTIMES(); result->opcode_ = (yyval.opcode_); }
#line 4182 "parser.cc"
    break;

  case 355: /* Opcode: _KW_UD2  */
#line 808 "nask.y"
            { (yyval.opcode_) = new OpcodesUD2(); result->opcode_ = (yyval.opcode_); }
#line 4188 "parser.cc"
    break;

  case 356: /* Opcode: _KW_VERR  */
#line 809 "nask.y"
             { (yyval.opcode_) = new OpcodesVERR(); result->opcode_ = (yyval.opcode_); }
#line 4194 "parser.cc"
    break;

  case 357: /* Opcode: _KW_VERW  */
#line 810 "nask.y"
             { (yyval.opcode_) = new OpcodesVERW(); result->opcode_ = (yyval.opcode_); }
#line 4200 "parser.cc"
    break;

  case 358: /* Opcode: _KW_WAIT  */
#line 811 "nask.y"
             { (yyval.opcode_) = new OpcodesWAIT(); result->opcode_ = (yyval.opcode_); }
#line 4206 "parser.cc"
    break;

  case 359: /* Opcode: _KW_WBINVD  */
#line 812 "nask.y"
               { (yyval.opcode_) = new OpcodesWBINVD(); result->opcode_ = (yyval.opcode_); }
#line 4212 "parser.cc"
    break;

  case 360: /* Opcode: _KW_WRMSR  */
#line 813 "nask.y"
              { (yyval.opcode_) = new OpcodesWRMSR(); result->opcode_ = (yyval.opcode_); }
#line 4218 "parser.cc"
    break;

  case 361: /* Opcode: _KW_XADD  */
#line 814 "nask.y"
             { (yyval.opcode_) = new OpcodesXADD(); result->opcode_ = (yyval.opcode_); }
#line 4224 "parser.cc"
    break;

  case 362: /* Opcode: _KW_XCHG  */
#line 815 "nask.y"
             { (yyval.opcode_) = new OpcodesXCHG(); result->opcode_ = (yyval.opcode_); }
#line 4230 "parser.cc"
    break;

  case 363: /* Opcode: _KW_XLATB  */
#line 816 "nask.y"
              { (yyval.opcode_) = new OpcodesXLATB(); result->opcode_ = (yyval.opcode_); }
#line 4236 "parser.cc"
    break;

  case 364: /* Opcode: _KW_XOR  */
#line 817 "nask.y"
            { (yyval.opcode_) = new OpcodesXOR(); result->opcode_ = (yyval.opcode_); }
#line 4242 "parser.cc"
    break;


#line 4246 "parser.cc"

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

#line 820 "nask.y"



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

/* Entrypoint: parse DataType* from file. */
DataType* pDataType(FILE *inp)
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
    return result.datatype_;
  }
}

/* Entrypoint: parse DataType* from string. */
DataType* psDataType(const char *str)
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
    return result.datatype_;
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



