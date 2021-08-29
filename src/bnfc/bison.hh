/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_NASK_BISON_HH_INCLUDED
# define YY_NASK_BISON_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int nask_debug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    _ERROR_ = 258,                 /* _ERROR_  */
    _BANGEQ = 259,                 /* _BANGEQ  */
    _PERCENT = 260,                /* _PERCENT  */
    _STAR = 261,                   /* _STAR  */
    _PLUS = 262,                   /* _PLUS  */
    _COMMA = 263,                  /* _COMMA  */
    _MINUS = 264,                  /* _MINUS  */
    _SLASH = 265,                  /* _SLASH  */
    _COLON = 266,                  /* _COLON  */
    _LT = 267,                     /* _LT  */
    _LDARROW = 268,                /* _LDARROW  */
    _DEQ = 269,                    /* _DEQ  */
    _GT = 270,                     /* _GT  */
    _GTEQ = 271,                   /* _GTEQ  */
    _KW_AAA = 272,                 /* _KW_AAA  */
    _KW_AAD = 273,                 /* _KW_AAD  */
    _KW_AAM = 274,                 /* _KW_AAM  */
    _KW_AAS = 275,                 /* _KW_AAS  */
    _KW_ABSOLUTE = 276,            /* _KW_ABSOLUTE  */
    _KW_ADC = 277,                 /* _KW_ADC  */
    _KW_ADD = 278,                 /* _KW_ADD  */
    _KW_ALIGN = 279,               /* _KW_ALIGN  */
    _KW_ALIGNB = 280,              /* _KW_ALIGNB  */
    _KW_AND = 281,                 /* _KW_AND  */
    _KW_ARPL = 282,                /* _KW_ARPL  */
    _KW_BITS = 283,                /* _KW_BITS  */
    _KW_BOUND = 284,               /* _KW_BOUND  */
    _KW_BSF = 285,                 /* _KW_BSF  */
    _KW_BSR = 286,                 /* _KW_BSR  */
    _KW_BSWAP = 287,               /* _KW_BSWAP  */
    _KW_BT = 288,                  /* _KW_BT  */
    _KW_BTC = 289,                 /* _KW_BTC  */
    _KW_BTR = 290,                 /* _KW_BTR  */
    _KW_BTS = 291,                 /* _KW_BTS  */
    _KW_BYTE = 292,                /* _KW_BYTE  */
    _KW_CALL = 293,                /* _KW_CALL  */
    _KW_CBW = 294,                 /* _KW_CBW  */
    _KW_CDQ = 295,                 /* _KW_CDQ  */
    _KW_CLC = 296,                 /* _KW_CLC  */
    _KW_CLD = 297,                 /* _KW_CLD  */
    _KW_CLI = 298,                 /* _KW_CLI  */
    _KW_CLTS = 299,                /* _KW_CLTS  */
    _KW_CMC = 300,                 /* _KW_CMC  */
    _KW_CMP = 301,                 /* _KW_CMP  */
    _KW_CMPSB = 302,               /* _KW_CMPSB  */
    _KW_CMPSD = 303,               /* _KW_CMPSD  */
    _KW_CMPSW = 304,               /* _KW_CMPSW  */
    _KW_CMPXCHG = 305,             /* _KW_CMPXCHG  */
    _KW_CPUID = 306,               /* _KW_CPUID  */
    _KW_CWD = 307,                 /* _KW_CWD  */
    _KW_CWDE = 308,                /* _KW_CWDE  */
    _KW_DAA = 309,                 /* _KW_DAA  */
    _KW_DAS = 310,                 /* _KW_DAS  */
    _KW_DB = 311,                  /* _KW_DB  */
    _KW_DD = 312,                  /* _KW_DD  */
    _KW_DEC = 313,                 /* _KW_DEC  */
    _KW_DIV = 314,                 /* _KW_DIV  */
    _KW_DQ = 315,                  /* _KW_DQ  */
    _KW_DT = 316,                  /* _KW_DT  */
    _KW_DW = 317,                  /* _KW_DW  */
    _KW_DWORD = 318,               /* _KW_DWORD  */
    _KW_END = 319,                 /* _KW_END  */
    _KW_ENTER = 320,               /* _KW_ENTER  */
    _KW_EQU = 321,                 /* _KW_EQU  */
    _KW_EXTERN = 322,              /* _KW_EXTERN  */
    _KW_F2XM1 = 323,               /* _KW_F2XM1  */
    _KW_FABS = 324,                /* _KW_FABS  */
    _KW_FADD = 325,                /* _KW_FADD  */
    _KW_FADDP = 326,               /* _KW_FADDP  */
    _KW_FBLD = 327,                /* _KW_FBLD  */
    _KW_FBSTP = 328,               /* _KW_FBSTP  */
    _KW_FCHS = 329,                /* _KW_FCHS  */
    _KW_FCLEX = 330,               /* _KW_FCLEX  */
    _KW_FCOM = 331,                /* _KW_FCOM  */
    _KW_FCOMP = 332,               /* _KW_FCOMP  */
    _KW_FCOMPP = 333,              /* _KW_FCOMPP  */
    _KW_FCOS = 334,                /* _KW_FCOS  */
    _KW_FDECSTP = 335,             /* _KW_FDECSTP  */
    _KW_FDISI = 336,               /* _KW_FDISI  */
    _KW_FDIV = 337,                /* _KW_FDIV  */
    _KW_FDIVP = 338,               /* _KW_FDIVP  */
    _KW_FDIVR = 339,               /* _KW_FDIVR  */
    _KW_FDIVRP = 340,              /* _KW_FDIVRP  */
    _KW_FENI = 341,                /* _KW_FENI  */
    _KW_FFREE = 342,               /* _KW_FFREE  */
    _KW_FIADD = 343,               /* _KW_FIADD  */
    _KW_FICOM = 344,               /* _KW_FICOM  */
    _KW_FICOMP = 345,              /* _KW_FICOMP  */
    _KW_FIDIV = 346,               /* _KW_FIDIV  */
    _KW_FIDIVR = 347,              /* _KW_FIDIVR  */
    _KW_FILD = 348,                /* _KW_FILD  */
    _KW_FILE = 349,                /* _KW_FILE  */
    _KW_FIMUL = 350,               /* _KW_FIMUL  */
    _KW_FINCSTP = 351,             /* _KW_FINCSTP  */
    _KW_FINIT = 352,               /* _KW_FINIT  */
    _KW_FIST = 353,                /* _KW_FIST  */
    _KW_FISTP = 354,               /* _KW_FISTP  */
    _KW_FISUB = 355,               /* _KW_FISUB  */
    _KW_FISUBR = 356,              /* _KW_FISUBR  */
    _KW_FLD = 357,                 /* _KW_FLD  */
    _KW_FLD1 = 358,                /* _KW_FLD1  */
    _KW_FLDCW = 359,               /* _KW_FLDCW  */
    _KW_FLDENV = 360,              /* _KW_FLDENV  */
    _KW_FLDL2E = 361,              /* _KW_FLDL2E  */
    _KW_FLDL2T = 362,              /* _KW_FLDL2T  */
    _KW_FLDLG2 = 363,              /* _KW_FLDLG2  */
    _KW_FLDLN2 = 364,              /* _KW_FLDLN2  */
    _KW_FLDPI = 365,               /* _KW_FLDPI  */
    _KW_FLDZ = 366,                /* _KW_FLDZ  */
    _KW_FMUL = 367,                /* _KW_FMUL  */
    _KW_FMULP = 368,               /* _KW_FMULP  */
    _KW_FNCLEX = 369,              /* _KW_FNCLEX  */
    _KW_FNDISI = 370,              /* _KW_FNDISI  */
    _KW_FNENI = 371,               /* _KW_FNENI  */
    _KW_FNINIT = 372,              /* _KW_FNINIT  */
    _KW_FNOP = 373,                /* _KW_FNOP  */
    _KW_FNSAVE = 374,              /* _KW_FNSAVE  */
    _KW_FNSTCW = 375,              /* _KW_FNSTCW  */
    _KW_FNSTENV = 376,             /* _KW_FNSTENV  */
    _KW_FNSTSW = 377,              /* _KW_FNSTSW  */
    _KW_FORMAT = 378,              /* _KW_FORMAT  */
    _KW_FPATAN = 379,              /* _KW_FPATAN  */
    _KW_FPREM = 380,               /* _KW_FPREM  */
    _KW_FPREM1 = 381,              /* _KW_FPREM1  */
    _KW_FPTAN = 382,               /* _KW_FPTAN  */
    _KW_FRNDINT = 383,             /* _KW_FRNDINT  */
    _KW_FRSTOR = 384,              /* _KW_FRSTOR  */
    _KW_FSAVE = 385,               /* _KW_FSAVE  */
    _KW_FSCALE = 386,              /* _KW_FSCALE  */
    _KW_FSETPM = 387,              /* _KW_FSETPM  */
    _KW_FSIN = 388,                /* _KW_FSIN  */
    _KW_FSINCOS = 389,             /* _KW_FSINCOS  */
    _KW_FSQRT = 390,               /* _KW_FSQRT  */
    _KW_FST = 391,                 /* _KW_FST  */
    _KW_FSTCW = 392,               /* _KW_FSTCW  */
    _KW_FSTENV = 393,              /* _KW_FSTENV  */
    _KW_FSTP = 394,                /* _KW_FSTP  */
    _KW_FSTSW = 395,               /* _KW_FSTSW  */
    _KW_FSUB = 396,                /* _KW_FSUB  */
    _KW_FSUBP = 397,               /* _KW_FSUBP  */
    _KW_FSUBR = 398,               /* _KW_FSUBR  */
    _KW_FSUBRP = 399,              /* _KW_FSUBRP  */
    _KW_FTST = 400,                /* _KW_FTST  */
    _KW_FUCOM = 401,               /* _KW_FUCOM  */
    _KW_FUCOMP = 402,              /* _KW_FUCOMP  */
    _KW_FUCOMPP = 403,             /* _KW_FUCOMPP  */
    _KW_FXAM = 404,                /* _KW_FXAM  */
    _KW_FXCH = 405,                /* _KW_FXCH  */
    _KW_FXTRACT = 406,             /* _KW_FXTRACT  */
    _KW_FYL2X = 407,               /* _KW_FYL2X  */
    _KW_FYL2XP1 = 408,             /* _KW_FYL2XP1  */
    _KW_HLT = 409,                 /* _KW_HLT  */
    _KW_IDIV = 410,                /* _KW_IDIV  */
    _KW_IMUL = 411,                /* _KW_IMUL  */
    _KW_IN = 412,                  /* _KW_IN  */
    _KW_INC = 413,                 /* _KW_INC  */
    _KW_INCO = 414,                /* _KW_INCO  */
    _KW_INSB = 415,                /* _KW_INSB  */
    _KW_INSD = 416,                /* _KW_INSD  */
    _KW_INSTRSET = 417,            /* _KW_INSTRSET  */
    _KW_INSW = 418,                /* _KW_INSW  */
    _KW_INT = 419,                 /* _KW_INT  */
    _KW_INT3 = 420,                /* _KW_INT3  */
    _KW_INTO = 421,                /* _KW_INTO  */
    _KW_INVD = 422,                /* _KW_INVD  */
    _KW_INVLPG = 423,              /* _KW_INVLPG  */
    _KW_IRET = 424,                /* _KW_IRET  */
    _KW_IRETD = 425,               /* _KW_IRETD  */
    _KW_IRETW = 426,               /* _KW_IRETW  */
    _KW_JA = 427,                  /* _KW_JA  */
    _KW_JAE = 428,                 /* _KW_JAE  */
    _KW_JB = 429,                  /* _KW_JB  */
    _KW_JBE = 430,                 /* _KW_JBE  */
    _KW_JC = 431,                  /* _KW_JC  */
    _KW_JCXZ = 432,                /* _KW_JCXZ  */
    _KW_JE = 433,                  /* _KW_JE  */
    _KW_JECXZ = 434,               /* _KW_JECXZ  */
    _KW_JG = 435,                  /* _KW_JG  */
    _KW_JGE = 436,                 /* _KW_JGE  */
    _KW_JL = 437,                  /* _KW_JL  */
    _KW_JLE = 438,                 /* _KW_JLE  */
    _KW_JMP = 439,                 /* _KW_JMP  */
    _KW_JNA = 440,                 /* _KW_JNA  */
    _KW_JNAE = 441,                /* _KW_JNAE  */
    _KW_JNB = 442,                 /* _KW_JNB  */
    _KW_JNBE = 443,                /* _KW_JNBE  */
    _KW_JNC = 444,                 /* _KW_JNC  */
    _KW_JNE = 445,                 /* _KW_JNE  */
    _KW_JNG = 446,                 /* _KW_JNG  */
    _KW_JNGE = 447,                /* _KW_JNGE  */
    _KW_JNL = 448,                 /* _KW_JNL  */
    _KW_JNLE = 449,                /* _KW_JNLE  */
    _KW_JNO = 450,                 /* _KW_JNO  */
    _KW_JNP = 451,                 /* _KW_JNP  */
    _KW_JNS = 452,                 /* _KW_JNS  */
    _KW_JNZ = 453,                 /* _KW_JNZ  */
    _KW_JO = 454,                  /* _KW_JO  */
    _KW_JP = 455,                  /* _KW_JP  */
    _KW_JPE = 456,                 /* _KW_JPE  */
    _KW_JPO = 457,                 /* _KW_JPO  */
    _KW_JS = 458,                  /* _KW_JS  */
    _KW_JZ = 459,                  /* _KW_JZ  */
    _KW_LAHF = 460,                /* _KW_LAHF  */
    _KW_LAR = 461,                 /* _KW_LAR  */
    _KW_LDS = 462,                 /* _KW_LDS  */
    _KW_LEA = 463,                 /* _KW_LEA  */
    _KW_LEAVE = 464,               /* _KW_LEAVE  */
    _KW_LES = 465,                 /* _KW_LES  */
    _KW_LFS = 466,                 /* _KW_LFS  */
    _KW_LGDT = 467,                /* _KW_LGDT  */
    _KW_LGS = 468,                 /* _KW_LGS  */
    _KW_LIDT = 469,                /* _KW_LIDT  */
    _KW_LLDT = 470,                /* _KW_LLDT  */
    _KW_LMSW = 471,                /* _KW_LMSW  */
    _KW_LOCK = 472,                /* _KW_LOCK  */
    _KW_LODSB = 473,               /* _KW_LODSB  */
    _KW_LODSD = 474,               /* _KW_LODSD  */
    _KW_LODSW = 475,               /* _KW_LODSW  */
    _KW_LOOP = 476,                /* _KW_LOOP  */
    _KW_LOOPE = 477,               /* _KW_LOOPE  */
    _KW_LOOPNE = 478,              /* _KW_LOOPNE  */
    _KW_LOOPNZ = 479,              /* _KW_LOOPNZ  */
    _KW_LOOPZ = 480,               /* _KW_LOOPZ  */
    _KW_LSL = 481,                 /* _KW_LSL  */
    _KW_LSS = 482,                 /* _KW_LSS  */
    _KW_LTR = 483,                 /* _KW_LTR  */
    _KW_MOV = 484,                 /* _KW_MOV  */
    _KW_MOVSB = 485,               /* _KW_MOVSB  */
    _KW_MOVSD = 486,               /* _KW_MOVSD  */
    _KW_MOVSW = 487,               /* _KW_MOVSW  */
    _KW_MOVSX = 488,               /* _KW_MOVSX  */
    _KW_MOVZX = 489,               /* _KW_MOVZX  */
    _KW_MUL = 490,                 /* _KW_MUL  */
    _KW_NEG = 491,                 /* _KW_NEG  */
    _KW_NOP = 492,                 /* _KW_NOP  */
    _KW_NOT = 493,                 /* _KW_NOT  */
    _KW_OPTIMIZE = 494,            /* _KW_OPTIMIZE  */
    _KW_OR = 495,                  /* _KW_OR  */
    _KW_ORG = 496,                 /* _KW_ORG  */
    _KW_OUT = 497,                 /* _KW_OUT  */
    _KW_OUTSB = 498,               /* _KW_OUTSB  */
    _KW_OUTSD = 499,               /* _KW_OUTSD  */
    _KW_OUTSW = 500,               /* _KW_OUTSW  */
    _KW_PADDING = 501,             /* _KW_PADDING  */
    _KW_PADSET = 502,              /* _KW_PADSET  */
    _KW_POP = 503,                 /* _KW_POP  */
    _KW_POPA = 504,                /* _KW_POPA  */
    _KW_POPAD = 505,               /* _KW_POPAD  */
    _KW_POPAW = 506,               /* _KW_POPAW  */
    _KW_POPF = 507,                /* _KW_POPF  */
    _KW_POPFD = 508,               /* _KW_POPFD  */
    _KW_POPFW = 509,               /* _KW_POPFW  */
    _KW_PUSH = 510,                /* _KW_PUSH  */
    _KW_PUSHA = 511,               /* _KW_PUSHA  */
    _KW_PUSHAD = 512,              /* _KW_PUSHAD  */
    _KW_PUSHAW = 513,              /* _KW_PUSHAW  */
    _KW_PUSHD = 514,               /* _KW_PUSHD  */
    _KW_PUSHF = 515,               /* _KW_PUSHF  */
    _KW_PUSHFD = 516,              /* _KW_PUSHFD  */
    _KW_PUSHFW = 517,              /* _KW_PUSHFW  */
    _KW_RCL = 518,                 /* _KW_RCL  */
    _KW_RCR = 519,                 /* _KW_RCR  */
    _KW_RDMSR = 520,               /* _KW_RDMSR  */
    _KW_RDPMC = 521,               /* _KW_RDPMC  */
    _KW_REP = 522,                 /* _KW_REP  */
    _KW_REPE = 523,                /* _KW_REPE  */
    _KW_REPNE = 524,               /* _KW_REPNE  */
    _KW_REPNZ = 525,               /* _KW_REPNZ  */
    _KW_REPZ = 526,                /* _KW_REPZ  */
    _KW_RESB = 527,                /* _KW_RESB  */
    _KW_RESD = 528,                /* _KW_RESD  */
    _KW_RESQ = 529,                /* _KW_RESQ  */
    _KW_REST = 530,                /* _KW_REST  */
    _KW_RESW = 531,                /* _KW_RESW  */
    _KW_RET = 532,                 /* _KW_RET  */
    _KW_RETF = 533,                /* _KW_RETF  */
    _KW_RETN = 534,                /* _KW_RETN  */
    _KW_ROL = 535,                 /* _KW_ROL  */
    _KW_ROR = 536,                 /* _KW_ROR  */
    _KW_RSM = 537,                 /* _KW_RSM  */
    _KW_SAHF = 538,                /* _KW_SAHF  */
    _KW_SAL = 539,                 /* _KW_SAL  */
    _KW_SAR = 540,                 /* _KW_SAR  */
    _KW_SBB = 541,                 /* _KW_SBB  */
    _KW_SCASB = 542,               /* _KW_SCASB  */
    _KW_SCASD = 543,               /* _KW_SCASD  */
    _KW_SCASW = 544,               /* _KW_SCASW  */
    _KW_SECTION = 545,             /* _KW_SECTION  */
    _KW_SETA = 546,                /* _KW_SETA  */
    _KW_SETAE = 547,               /* _KW_SETAE  */
    _KW_SETB = 548,                /* _KW_SETB  */
    _KW_SETBE = 549,               /* _KW_SETBE  */
    _KW_SETC = 550,                /* _KW_SETC  */
    _KW_SETE = 551,                /* _KW_SETE  */
    _KW_SETG = 552,                /* _KW_SETG  */
    _KW_SETGE = 553,               /* _KW_SETGE  */
    _KW_SETL = 554,                /* _KW_SETL  */
    _KW_SETLE = 555,               /* _KW_SETLE  */
    _KW_SETNA = 556,               /* _KW_SETNA  */
    _KW_SETNAE = 557,              /* _KW_SETNAE  */
    _KW_SETNB = 558,               /* _KW_SETNB  */
    _KW_SETNBE = 559,              /* _KW_SETNBE  */
    _KW_SETNC = 560,               /* _KW_SETNC  */
    _KW_SETNE = 561,               /* _KW_SETNE  */
    _KW_SETNG = 562,               /* _KW_SETNG  */
    _KW_SETNGE = 563,              /* _KW_SETNGE  */
    _KW_SETNL = 564,               /* _KW_SETNL  */
    _KW_SETNLE = 565,              /* _KW_SETNLE  */
    _KW_SETNO = 566,               /* _KW_SETNO  */
    _KW_SETNP = 567,               /* _KW_SETNP  */
    _KW_SETNS = 568,               /* _KW_SETNS  */
    _KW_SETNZ = 569,               /* _KW_SETNZ  */
    _KW_SETO = 570,                /* _KW_SETO  */
    _KW_SETP = 571,                /* _KW_SETP  */
    _KW_SETPE = 572,               /* _KW_SETPE  */
    _KW_SETPO = 573,               /* _KW_SETPO  */
    _KW_SETS = 574,                /* _KW_SETS  */
    _KW_SETZ = 575,                /* _KW_SETZ  */
    _KW_SGDT = 576,                /* _KW_SGDT  */
    _KW_SHL = 577,                 /* _KW_SHL  */
    _KW_SHLD = 578,                /* _KW_SHLD  */
    _KW_SHR = 579,                 /* _KW_SHR  */
    _KW_SHRD = 580,                /* _KW_SHRD  */
    _KW_SIDT = 581,                /* _KW_SIDT  */
    _KW_SLDT = 582,                /* _KW_SLDT  */
    _KW_SMSW = 583,                /* _KW_SMSW  */
    _KW_STC = 584,                 /* _KW_STC  */
    _KW_STD = 585,                 /* _KW_STD  */
    _KW_STI = 586,                 /* _KW_STI  */
    _KW_STOSB = 587,               /* _KW_STOSB  */
    _KW_STOSD = 588,               /* _KW_STOSD  */
    _KW_STOSW = 589,               /* _KW_STOSW  */
    _KW_STR = 590,                 /* _KW_STR  */
    _KW_SUB = 591,                 /* _KW_SUB  */
    _KW_TEST = 592,                /* _KW_TEST  */
    _KW_TIMES = 593,               /* _KW_TIMES  */
    _KW_UD2 = 594,                 /* _KW_UD2  */
    _KW_VERR = 595,                /* _KW_VERR  */
    _KW_VERW = 596,                /* _KW_VERW  */
    _KW_WAIT = 597,                /* _KW_WAIT  */
    _KW_WBINVD = 598,              /* _KW_WBINVD  */
    _KW_WORD = 599,                /* _KW_WORD  */
    _KW_WRMSR = 600,               /* _KW_WRMSR  */
    _KW_XADD = 601,                /* _KW_XADD  */
    _KW_XCHG = 602,                /* _KW_XCHG  */
    _KW_XLATB = 603,               /* _KW_XLATB  */
    _KW_XOR = 604,                 /* _KW_XOR  */
    _LBRACK = 605,                 /* _LBRACK  */
    _RBRACK = 606,                 /* _RBRACK  */
    T_Hex = 607,                   /* T_Hex  */
    T_Label = 608,                 /* T_Label  */
    _STRING_ = 609,                /* _STRING_  */
    _INTEGER_ = 610,               /* _INTEGER_  */
    _IDENT_ = 611                  /* _IDENT_  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 47 "nask.y"

  int    _int;
  char   _char;
  double _double;
  char*  _string;
  Program* program_;
  ListStatement* liststatement_;
  Statement* statement_;
  ListMnemonicArgs* listmnemonicargs_;
  MnemonicArgs* mnemonicargs_;
  Exp* exp_;
  Factor* factor_;
  ConfigType* configtype_;
  DataType* datatype_;
  Opcode* opcode_;

#line 437 "bison.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int nask_parse (yyscan_t scanner, YYSTYPE *result);

#endif /* !YY_NASK_BISON_HH_INCLUDED  */