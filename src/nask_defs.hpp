#ifndef NASK_DEFS_HPP_
#define NASK_DEFS_HPP_

#include <cstdint>
#include <array>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <regex>
#include <tuple>
#include "spdlog/spdlog.h"

static const std::shared_ptr<spdlog::logger> log() {
     return spdlog::get("opennask");
}

// 80x86 Opcodes
// https://courses.engr.illinois.edu/ece390/resources/opcodes.html
const std::array<std::string, 24> REGISTERS {
     "AL", "BL", "CL", "DL", "EAX", "EBX", "ECX", "EDX", "AX", "BX", "CX", "DX",
     "AH", "BH", "CH", "DH", "ESP", "EDI", "EBP", "ESI", "SP", "DI", "BP", "SI"
};

const std::array<std::string, 6> SEGMENT_REGISTERS {
     "CS", // コード
     "DS", // データ
     "ES", // エクストラ
     "SS", // スタック
     "FS",
     "GS"
};

// PE file header
constexpr uint16_t I386MAGIC = 0x14c;

struct PIMAGE_FILE_HEADER {
     uint16_t  machine;
     uint16_t  numberOfSections;
     int32_t   timeDateStamp;
     int32_t   pointerToSymbolTable;
     int32_t   numberOfSymbols;
     uint16_t  sizeOfOptionalHeader;
     uint16_t  characteristics;
} __attribute__((packed));

// PE section header
constexpr size_t IMAGE_SIZEOF_SHORT_NAME = 8;

struct PIMAGE_SECTION_HEADER {
     uint8_t         name[IMAGE_SIZEOF_SHORT_NAME];
     union {
	  uint32_t   physicalAddress;
	  uint32_t   virtualSize;
     } misc;
     uint32_t        virtualAddress;
     uint32_t        sizeOfRawData;
     uint32_t        pointerToRawData;
     uint32_t        pointerToRelocations;
     uint32_t        pointerToLinenumbers;
     uint16_t        numberOfRelocations;
     uint16_t        numberOfLinenumbers;
     uint32_t        characteristics;
} __attribute__((packed));

constexpr uint16_t WCOFF_TEXT_FIELD = 0x0001;
constexpr uint16_t WCOFF_DATA_FIELD = 0x0002;
constexpr uint16_t WCOFF_BSS_FIELD  = 0x0003;

struct PIMAGE_SYMBOL {
     uint8_t    shortName[8];
     uint32_t   value;
     uint16_t   sectionNumber;
     uint16_t   type;
     uint8_t    storageClass;
     uint8_t    numberOfAuxSymbols;
} __attribute__((packed));

//
// The registers FS and GS are segment registers. They have no processor-defined purpose,
// but instead are given purpose by the OS's running them.
// In Windows 64-bit the GS register is used to point to operating system defined structures.
// FS and GS are commonly used by OS kernels to access thread-specific memory. In windows,
// the GS register is used to manage thread-specific memory.
// The linux kernel uses GS to access cpu-specific memory.
//
// https://stackoverflow.com/questions/10810203/what-is-the-fs-gs-register-intended-for/10810287#10810287

const std::array<std::string, 4> CONTROL_REGISTERS {
     "CR0", "CR2", "CR3", "CR4"
};

const std::array<std::string, 1> PRE_PROCESS_WORDS {
     "EQU"
};

const std::array<std::string, 4> PRE_PROCESS_OPERATORS {
     "*", "/", "+", "-"
};

const std::array<std::string, 10> DATA_TYPES {
     "BYTE", "WORD", "DWORD", "FWORD", "QWORD", "TBYTE", "OWORD", "REAL4", "REAL8", "REAL10"
};

// byte sized part of an opcode
constexpr size_t imm8  = 1;
constexpr size_t imm16 = 2;
constexpr size_t imm32 = 4;
constexpr size_t imm64 = 8;
constexpr size_t offs  = 12;

// 現在のnaskのモード
enum OPENNASK_MODES {
     ID_16BIT_MODE = 0,
     ID_32BIT_MODE,
     ID_63BIT_MODE
};

// オペランドの種類
// @see https://courses.engr.illinois.edu/ece390/resources/opcodes.html
enum OPERAND_KINDS {
     ID_Reg = 0,
     ID_Reg16,
     ID_Reg32,
     ID_Mem,
     ID_Mem16,
     ID_Mem32,
     ID_Acc,
     ID_Imm,
     ID_Imm8,
     ID_Rel8,
     ID_Rel16,
     ID_Rel32
};

constexpr uint8_t SUP_8086      = 0x000000ff; /* bit 0 */
constexpr uint8_t SUP_80186	= 0x000000fe; /* bit 1 */
constexpr uint8_t SUP_80286	= 0x000000fc; /* bit 2 */
constexpr uint8_t SUP_80286P	= 0x000000a8; /* bit 3 */
constexpr uint8_t SUP_i386	= 0x000000f0; /* bit 4 */
constexpr uint8_t SUP_i386P	= 0x000000a0; /* bit 5 */
constexpr uint8_t SUP_i486	= 0x000000c0; /* bit 6 */
constexpr uint8_t SUP_i486P	= 0x00000080; /* bit 7 */
constexpr uint8_t SUP_Pentium   = 0;
constexpr uint8_t SUP_Pentium2	= 0;
constexpr uint8_t SUP_Pentium3	= 0;
constexpr uint8_t SUP_Pentium4	= 0;

constexpr uint8_t PREFIX        = 0x01;       /* param[1]がプリフィックス番号 */
constexpr uint8_t NO_PARAM	= 0x02;       /* param[1]の下位4bitがオペコードバイト数 */
constexpr uint8_t OPE_MR	= 0x03;       /* mem/reg,reg型 */ /* [1]:datawidth, [2]:len */
constexpr uint8_t OPE_RM	= 0x04;       /* reg,mem/reg型 */
constexpr uint8_t OPE_M		= 0x05;       /* mem/reg型 */
constexpr uint8_t OPE_SHIFT     = 0x06;	      /* ROL, ROR, RCL, RCR, SHL, SAL, SHR, SAR */
constexpr uint8_t OPE_RET	= 0x07;	      /* RET, RETF, RETN */
constexpr uint8_t OPE_AAMD	= 0x08;	      /* AAM, AAD */
constexpr uint8_t OPE_INT	= 0x09;       /* INT */
constexpr uint8_t OPE_PUSH	= 0x0a;       /* INC, DEC, PUSH, POP */
constexpr uint8_t OPE_MOV	= 0x0b;       /* MOV */
constexpr uint8_t OPE_ADD	= 0x0c;       /* ADD, OR, ADC, SBB, AND, SUB, XOR, CMP */
constexpr uint8_t OPE_XCHG	= 0x0d;       /* XCHG */
constexpr uint8_t OPE_INOUT	= 0x0e;       /* IN, OUT */
constexpr uint8_t OPE_IMUL	= 0x0f;       /* IMUL */
constexpr uint8_t OPE_TEST	= 0x10;       /* TEST */
constexpr uint8_t OPE_MOVZX	= 0x11;       /* MOVSX, MOVZX */
constexpr uint8_t OPE_SHLD	= 0x12;       /* SHLD, SHRD */
constexpr uint8_t OPE_LOOP	= 0x13;       /* LOOPcc, JCXZ */
constexpr uint8_t OPE_JCC	= 0x14;       /* Jcc */
constexpr uint8_t OPE_BT	= 0x15;       /* BT, BTC, BTR, BTS */
constexpr uint8_t OPE_ENTER	= 0x16;       /* ENTER */
constexpr uint8_t OPE_ALIGN	= 0x17;       /* ALIGN, ALIGNB */
constexpr uint8_t OPE_FPU	= 0x30;
constexpr uint8_t OPE_FPUP	= 0x31;
constexpr uint8_t OPE_FSTSW	= 0x32;
constexpr uint8_t OPE_FXCH	= 0x33;
constexpr uint8_t OPE_ORG	= 0x3d;       /* ORG */
constexpr uint8_t OPE_RESB	= 0x3e;       /* RESB, RESW, RESD, RESQ, REST */
constexpr uint8_t OPE_EQU	= 0x3f;

constexpr uint8_t OPE_JMP	= 0x40;       /* CALL, JMP */
constexpr uint8_t OPE_GLOBAL	= 0x44;	      /* GLOBAL, EXTERN */
constexpr uint8_t OPE_TIMES	= 0x47;	      /* TIMES */
constexpr uint8_t OPE_DB	= 0x48;	      /* DB, DW, DD, DQ, DT */
constexpr uint8_t OPE_END	= 0x49;

/* NO_PARAM用 */
constexpr uint8_t OPE16         = 0x10;
constexpr uint8_t OPE32		= 0x20;
constexpr uint8_t DEF_DS	= 0x40;
/* param[1]のbit4 : ope32 */
/* param[1]のbit5 : ope16 */
/* param[1]のbit6 : デフォルトプリフィックスDS */
/* param[1]のbit7 : デフォルトプリフィックスSS */

static constexpr uint32_t nask_LABELBUFSIZ = 256 * 1024;
static constexpr uint32_t OPCLENMAX    = 12; /* 足りなくなったら12にしてください */
static constexpr uint32_t MAX_SECTIONS = 8;
static constexpr uint32_t E_LABEL0     = 16;
static constexpr int nask_L_LABEL0     = 16384; /* externラベルは16300個程度使える */
static constexpr uint32_t MAX_LISTLEN  = 32;
static constexpr uint32_t MAX_SUPPORTED_OPECODE = 321;

template <class T, size_t dim>
static void append_buf(std::array<T, dim>& arr, std::stringstream& buf) {
     for (int i = 0; i < dim; i++) {
	  buf << "0x";
	  buf << std::setfill('0') << std::setw(2) << std::hex;
	  buf << static_cast<uint32_t>(arr[i]);
	  if (i+1 != dim) { buf << ',' << ' '; }
     }
}

template <class T, size_t dim>
static void append_buf_pretty(const char* name, std::array<T, dim>& arr, std::stringstream& buf) {
     buf << name << ": [ ";
     append_buf(arr, buf);
     buf << " ]" << std::endl;
}

struct INST_TABLE {
     std::string opecode;
     uint32_t support;
     std::array<uint8_t, 8> param;
     std::string to_string() {
	  std::stringstream buf;
	  //append_buf_pretty("opecode", opecode, buf);
	  buf << "support: " << support << std::endl;
	  append_buf_pretty("param", param, buf);
	  return buf.str();
     }
};

static std::array<INST_TABLE, MAX_SUPPORTED_OPECODE> instruction = {{
     { "AAA",		SUP_8086,	NO_PARAM, 1, 0x37 },
     { "AAD",		SUP_8086,	OPE_AAMD, 0xd5, 10 },
     { "AAS",		SUP_8086,	NO_PARAM, 1, 0x3f },
     { "AAM",		SUP_8086,	OPE_AAMD, 0xd4, 10 },
     { "ADC",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x02 << 3 },
     { "ADD",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x00 << 3 },
     { "AND",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x04 << 3 },
     { "ALIGN",		SUP_8086,	OPE_ALIGN, 0x90 },
     { "ALIGNB",	SUP_8086,	OPE_ALIGN, 0x00 },
     { "ARPL",		SUP_80286P,	OPE_MR, 0xa2, 0x21, 0x63 }, /* w,ws no-w no-o16/o32 */
     { "BOUND",		SUP_80186,	OPE_RM, 0x86, 0x01, 0x62 }, /* dw,s no-w */
     { "BSF",		SUP_i386,	OPE_RM, 0x86, 0x02, 0x0f, 0xbc }, /* dw,s no-w */
     { "BSR",		SUP_i386,	OPE_RM, 0x86, 0x02, 0x0f, 0xbd }, /* dw,s no-w */
     { "BSWAP",		SUP_i486,	OPE_M, 0x14, 0xa1, 0x0f }, /* d no-w no-o16/o32 no-mem */
     { "BT",		SUP_i386,	OPE_BT, 0x04 << 3 },
     { "BTC",		SUP_i386,	OPE_BT, 0x07 << 3 },
     { "BTR",		SUP_i386,	OPE_BT, 0x06 << 3 },
     { "BTS",		SUP_i386,	OPE_BT, 0x05 << 3 },
     { "CALL",		SUP_8086,	OPE_JMP, 0x02 << 3, 0xe8, 0x9a, 0x00 },
     { "CBW",		SUP_8086,	NO_PARAM, 1 | OPE16, 0x98 },
     { "CDQ",		SUP_i386,	NO_PARAM, 1 | OPE32, 0x99 },
     { "CLC",		SUP_8086,	NO_PARAM, 1, 0xf8 },
     { "CLD",		SUP_8086,	NO_PARAM, 1, 0xfc },
     { "CLI",		SUP_8086,	NO_PARAM, 1, 0xfa },
     { "CLTS",		SUP_80286P,	NO_PARAM, 2, 0x0f, 0x06 },
     { "CMC",		SUP_8086,	NO_PARAM, 1, 0xf5 },
     { "CMP",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x07 << 3 },
     { "CMPSB",		SUP_8086,	NO_PARAM, 1 | DEF_DS, 0xa6 },
     { "CMPSD",		SUP_i386,	NO_PARAM, 1 | OPE32 | DEF_DS, 0xa7 },
     { "CMPSW",		SUP_8086,	NO_PARAM, 1 | OPE16 | DEF_DS, 0xa7 },
     { "CMPXCHG",	SUP_i486,	OPE_MR, 0x87, 0x12, 0x0f, 0xb0 }, /* bwd,s w0 */
     { "CS",		SUP_8086,	PREFIX,	0x06 },
     { "CWD",		SUP_8086,	NO_PARAM, 1 | OPE16, 0x99 },
     { "CWDE",		SUP_i386,	NO_PARAM, 1 | OPE32, 0x98 },
     { "DAA",		SUP_8086,	NO_PARAM, 1, 0x27 },
     { "DAS",		SUP_8086,	NO_PARAM, 1, 0x2f },
     { "DB",		SUP_8086,	OPE_DB, 1, 0x9e }, /* DB - Define Byte. 8 bits */
     { "DD",		SUP_8086,	OPE_DB,	4, 0x9d }, /* Define double word. Generally 4 bytes on a typical x86 32-bit system */
     { "DEC",		SUP_8086,	OPE_PUSH, 0x01, 0x48, 0xfe },
     { "DIV",		SUP_8086,	OPE_M, 0x67, 0x11, 0xf6 }, /* bwd w0 */
     { "DQ",		SUP_8086,	OPE_DB, 8 },
     { "DS",		SUP_8086,	PREFIX, 0x08 },
     { "DT",		SUP_8086,	OPE_DB, 10 },
     { "DW",		SUP_8086,	OPE_DB, 2, 0x9b }, /* Define Word. Generally 2 bytes on a typical x86 32-bit system */
     { "END",		SUP_8086,	OPE_END },
     { "ENTER",		SUP_80186,	OPE_ENTER },
     { "EQU",		SUP_8086,	OPE_EQU },
     { "ES",		SUP_8086,	PREFIX, 0x05 },
     { "EXTERN",	SUP_8086,	OPE_GLOBAL, 2 },
     { "F2XM1",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xf0 },
     { "FABS",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xE1 },
     { "FADD",		SUP_8086,	OPE_FPU, 0, 0, 0x80 /* D8 /0 */, 0x84 /* DC /0 */, 0, 0x80 /* D8 /0 */, 0x84 /* DC /0 */ },
     { "FADDP",		SUP_8086,	OPE_FPUP, 0x06 /* DE /0 */ },
     { "FBLD",		SUP_8086,	OPE_FPU, 0x07 /* mem10 */, 0, 0, 0, 0xa7 /* DF /4 */ },
     { "FBSTP",		SUP_8086,	OPE_FPU, 0x07 /* mem10 */, 0, 0, 0, 0xb7 /* DF /6 */ },
     { "FCHS",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xe0 },
     { "FCLEX",		SUP_8086,	NO_PARAM, 3, 0x9b, 0xdb, 0xe2 },
     { "FCOM",		SUP_8086,	OPE_FPU, 0, 0, 0x90 /* D8 /2 */, 0x94 /* DC /2 */, 0, 0x90 /* D8 /2 */, 0 },
     { "FCOMP",		SUP_8086,	OPE_FPU, 0, 0, 0x98 /* D8 /3 */, 0x9c /* DC /3 */, 0, 0x98 /* D8 /3 */, 0 },
     { "FCOMPP",	SUP_8086,	NO_PARAM, 2, 0xde, 0xd9 },
     { "FCOS",		SUP_i386,	NO_PARAM, 2, 0xd9, 0xff },
     { "FDECSTP",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xf6 },
     { "FDISI",		SUP_8086,	NO_PARAM, 3, 0x9b, 0xdb, 0xe1 },
     { "FDIV",		SUP_8086,	OPE_FPU, 0, 0, 0xb0 /* D8 /6 */, 0xb4 /* DC /6 */, 0, 0xb0 /* D8 /6 */, 0xbc /* DC /7 */ },
     { "FDIVP",		SUP_8086,	OPE_FPUP, 0x3e /* DE /7 */ },
     { "FDIVR",		SUP_8086,	OPE_FPU, 0, 0, 0xb8 /* D8 /7 */, 0xbc /* DC /7 */, 0, 0xb8 /* D8 /7 */, 0xb4 /* DC /6 */ },
     { "FDIVRP",	SUP_8086,	OPE_FPUP, 0x36 /* DE /6 */ },
     { "FENI",		SUP_8086,	NO_PARAM, 3, 0x9b, 0xdb, 0xe0 },
     { "FFREE",		SUP_8086,	OPE_FPU, 0x08, 0, 0, 0, 0, 0x85 /* DD /0 */ },
     { "FIADD",		SUP_8086,	OPE_FPU, 0, 0x86 /* DE /0 */, 0x82 /* DA /0 */ },
     { "FICOM",		SUP_8086,	OPE_FPU, 0, 0x96 /* DE /2 */, 0x92 /* DA /2 */ },
     { "FICOMP",	SUP_8086,	OPE_FPU, 0, 0x9e /* DE /3 */, 0x9a /* DA /3 */ },
     { "FIDIV",		SUP_8086,	OPE_FPU, 0, 0xb6 /* DE /6 */, 0xb2 /* DA /6 */ },
     { "FIDIVR",	SUP_8086,	OPE_FPU, 0, 0xbe /* DE /7 */, 0xba /* DA /7 */ },
     { "FILD",		SUP_8086,	OPE_FPU, 0, 0x87 /* DF /0 */, 0x83 /* DB /0 */, 0xaf /* DF /5 */ },
     { "FIMUL",		SUP_8086,	OPE_FPU, 0, 0x8e /* DE /1 */, 0x8a /* DA /1 */ },
     { "FINCSTP",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xf7 },
     { "FINIT",		SUP_8086,	NO_PARAM, 3, 0x9b, 0xdb, 0xe3 },
     { "FIST",		SUP_8086,	OPE_FPU, 0, 0x97 /* DF /2 */, 0x93 /* DB /2 */ },
     { "FISTP",		SUP_8086,	OPE_FPU, 0, 0x9f /* DF /3 */, 0x9b /* DB /3 */, 0xbf /* DF /7 */ },
     { "FISUB",		SUP_8086,	OPE_FPU, 0, 0xa6 /* DE /4 */, 0xa2 /* DA /4 */ },
     { "FISUBR",	SUP_8086,	OPE_FPU, 0, 0xae /* DE /5 */, 0xaa /* DA /5 */ },
     { "FLD",		SUP_8086,	OPE_FPU, 0x08, 0, 0x81 /* D9 /0 */, 0x85 /* DD /0 */, 0xab /* DB /5 */, 0x81 /* D9 /0 */ },
     { "FLD1",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xe8 },
     { "FLDCW",		SUP_8086,	OPE_FPU, 0x04 /* mem2 */, 0xa9 /* D9 /5 */ },
     { "FLDENV",	SUP_8086,	OPE_M, 0x48, 0x61, 0xd9 }, /* s no-w no-o16/o32 no-reg */
     { "FLDL2E",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xea },
     { "FLDL2T",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xe9 },
     { "FLDLG2",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xec },
     { "FLDLN2",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xed },
     { "FLDPI",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xeb },
     { "FLDZ",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xee },
     { "FMUL",		SUP_8086,	OPE_FPU, 0, 0, 0x88 /* D8 /1 */, 0x8c /* DC /1 */, 0, 0x88 /* D8 /1 */, 0x8c /* DC /1 */ },
     { "FMULP",		SUP_8086,	OPE_FPUP, 0x0e /* DE /1 */ },
     { "FNCLEX",	SUP_8086,	NO_PARAM, 2, 0xdb, 0xe2 },
     { "FNDISI",	SUP_8086,	NO_PARAM, 2, 0xdb, 0xe1 },
     { "FNENI",		SUP_8086,	NO_PARAM, 2, 0xdb, 0xe0 },
     { "FNINIT",	SUP_8086,	NO_PARAM, 2, 0xdb, 0xe3 },
     { "FNOP",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xd0 },
     { "FNSAVE",	SUP_8086,	OPE_M, 0x68, 0x61, 0xdd }, /* s no-w no-o16/o32 no-reg */
     { "FNSTCW",	SUP_8086,	OPE_M, 0x7a, 0x61, 0xd9 }, /* ws no-w no-o16/o32 no-reg */
     { "FNSTENV",	SUP_8086,	OPE_M, 0x68, 0x61, 0xd9 }, /* s no-w no-o16/o32 no-reg */
     { "FNSTSW",	SUP_8086,	OPE_FSTSW, 0x0a, 0x61, 0xdd, 0, 0xdf, 0xe0 }, /* ws no-w no-o16/o32 no-reg */
     { "FPATAN",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xf3 },
     { "FPTAN",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xf2 },
     { "FPREM",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xf8 },
     { "FPREM1",	SUP_i386,	NO_PARAM, 2, 0xd9, 0xf5 },
     { "FRNDINT",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xfc },
     { "FRSTOR",	SUP_8086,	OPE_M, 0x48, 0x61, 0xdd }, /* s no-w no-o16/o32 no-reg */
     { "FS",		SUP_i386,	PREFIX, 0x09 },
     { "FSAVE",		SUP_8086,	OPE_M, 0x68, 0x62, 0x9b, 0xdd }, /* s no-w no-o16/o32 no-reg */
     { "FSCALE",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xfd },
     { "FSETPM",	SUP_80286P,	NO_PARAM, 2, 0xdb, 0xe4 },
     { "FSIN",		SUP_i386,	NO_PARAM, 2, 0xd9, 0xfe },
     { "FSINCOS",	SUP_i386,	NO_PARAM, 2, 0xd9, 0xfb },
     { "FSQRT",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xfa },
     { "FST",		SUP_8086,	OPE_FPU, 8, 0, 0x91 /* D9 /2 */, 0x95 /* DD /2 */, 0, 0x95 /* DD /2 */ },
     { "FSTCW",		SUP_8086,	OPE_M, 0x7a, 0x62, 0x9b, 0xd9 }, /* ws no-w no-o16/o32 no-reg */
     { "FSTENV",	SUP_8086,	OPE_M, 0x68, 0x62, 0x9b, 0xd9 }, /* s no-w no-o16/o32 no-reg */
     { "FSTP",		SUP_8086,	OPE_FPU, 8, 0, 0x99 /* D9 /3 */, 0x9d /* DD /3 */, 0xbb /* DB /7 */, 0x9d /* DD /3 */ },
     { "FSTSW",		SUP_8086,	OPE_FSTSW, 0x0a, 0x62, 0x9b, 0xdd, 0x9b, 0xdf, 0xe0 }, /* ws no-w no-o16/o32 no-reg */
     { "FSUB",		SUP_8086,	OPE_FPU, 0, 0, 0xa0 /* D8 /4 */, 0xa4 /* DC /4 */, 0, 0xa0 /* D8 /4 */, 0xac /* DC /5 */ },
     { "FSUBP",		SUP_8086,	OPE_FPUP, 0x2e /* DE /5 */ },
     { "FSUBR",		SUP_8086,	OPE_FPU, 0, 0, 0xa8 /* D8 /5 */, 0xac /* DC /5 */, 0, 0xa8 /* D8 /5 */, 0xa4 /* DC /4 */ },
     { "FSUBRP",	SUP_8086,	OPE_FPUP, 0x26 /* DE /4 */ },
     { "FTST",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xe4 },
     { "FUCOM",		SUP_i386,	OPE_FPU, 0, 0, 0, 0, 0, 0xa5 /* DD /4 */ },
     { "FUCOMP",	SUP_i386,	OPE_FPU, 0, 0, 0, 0, 0, 0xad /* DD /5 */ },
     { "FUCOMPP",	SUP_i386,	NO_PARAM, 2, 0xda, 0xe9 },
     { "FXAM",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xe5 },
     { "FXCH",		SUP_8086,	OPE_FXCH },
     { "FXTRACT",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xf4 },
     { "FYL2X",		SUP_8086,	NO_PARAM, 2, 0xd9, 0xf1 },
     { "FYL2XP1",	SUP_8086,	NO_PARAM, 2, 0xd9, 0xf9 },
     { "GLOBAL",	SUP_8086,	OPE_GLOBAL, 1 },
     { "GS",		SUP_i386,	PREFIX, 0x0a },
     { "HLT",		SUP_8086,	NO_PARAM, 1, 0xf4 },
     { "IDIV",		SUP_8086,	OPE_M, 0x77, 0x11, 0xf6 }, /* bwd w0 */
     { "IMUL",		SUP_8086,	OPE_IMUL, 0x57, 0x11, 0xf6  }, /* bwd w0 */
     { "IN",		SUP_8086,	OPE_INOUT, 0xe4 },
     { "INC",		SUP_8086,	OPE_PUSH, 0x00, 0x40, 0xfe },
     { "INSB",		SUP_8086,	NO_PARAM, 1, 0x6c },
     { "INSD",		SUP_i386,	NO_PARAM, 1 | OPE32, 0x6d },
     { "INSW",		SUP_8086,	NO_PARAM, 1 | OPE16, 0x6d },
     { "INT",		SUP_8086,	OPE_INT },
     { "INT3",		SUP_8086,	NO_PARAM, 1, 0xcc },
     { "INTO",		SUP_8086,	NO_PARAM, 1, 0xce },
     { "INVD",		SUP_i486,	NO_PARAM, 2, 0x0f, 0x08 },
     { "INVLPG",	SUP_i486,	OPE_M, 0x7e, 0x62, 0x0f, 0x01 }, /* wds no-w no-o16/o32 no-reg */
     { "IRET",		SUP_8086,	NO_PARAM, 1, 0xcf },
     { "IRETD",		SUP_i386,	NO_PARAM, 1 | OPE32, 0xcf },
     { "IRETW",		SUP_8086,	NO_PARAM, 1 | OPE16, 0xcf },
     { "JA",		SUP_8086,	OPE_JCC, 0x07 },
     { "JAE",		SUP_8086,	OPE_JCC, 0x03 },
     { "JB",		SUP_8086,	OPE_JCC, 0x02 },
     { "JBE",		SUP_8086,	OPE_JCC, 0x06 },
     { "JC",		SUP_8086,	OPE_JCC, 0x02 },
     { "JCXZ",		SUP_8086,	OPE_LOOP, 0xe3, 16 },
     { "JE",		SUP_8086,	OPE_JCC, 0x04 },
     { "JECXZ",		SUP_i386,	OPE_LOOP, 0xe3, 32 },
     { "JG",		SUP_8086,	OPE_JCC, 0x0f },
     { "JGE",		SUP_8086,	OPE_JCC, 0x0d },
     { "JL",		SUP_8086,	OPE_JCC, 0x0c },
     { "JLE",		SUP_8086,	OPE_JCC, 0x0e },
     { "JMP",		SUP_8086,	OPE_JMP, 0x04 << 3, 0xe9, 0xea, 0xeb },
     { "JNA",		SUP_8086,	OPE_JCC, 0x06 },
     { "JNAE",		SUP_8086,	OPE_JCC, 0x02 },
     { "JNB",		SUP_8086,	OPE_JCC, 0x03 },
     { "JNBE",		SUP_8086,	OPE_JCC, 0x07 },
     { "JNC",		SUP_8086,	OPE_JCC, 0x03 },
     { "JNE",		SUP_8086,	OPE_JCC, 0x05 },
     { "JNG",		SUP_8086,	OPE_JCC, 0x0e },
     { "JNGE",		SUP_8086,	OPE_JCC, 0x0c },
     { "JNL",		SUP_8086,	OPE_JCC, 0x0d },
     { "JNLE",		SUP_8086,	OPE_JCC, 0x0f },
     { "JNO",		SUP_8086,	OPE_JCC, 0x01 },
     { "JNP",		SUP_8086,	OPE_JCC, 0x0b },
     { "JNS",		SUP_8086,	OPE_JCC, 0x09 },
     { "JNZ",		SUP_8086,	OPE_JCC, 0x05 },
     { "JO",		SUP_8086,	OPE_JCC, 0x00 },
     { "JP",		SUP_8086,	OPE_JCC, 0x0a },
     { "JPE",		SUP_8086,	OPE_JCC, 0x0a },
     { "JPO",		SUP_8086,	OPE_JCC, 0x0b },
     { "JS",		SUP_8086,	OPE_JCC, 0x08 },
     { "JZ",		SUP_8086,	OPE_JCC, 0x04 },
     { "LAHF",		SUP_8086,	NO_PARAM, 1, 0x9f },
     { "LAR",		SUP_80286P,	OPE_RM, 0xa6, 0x02, 0x0f, 0x02 }, /* wd,ws no-w */
     { "LDS",		SUP_8086,	OPE_RM, 0x86, 0x41, 0xc5 }, /* wd,s no-w no-reg */
     { "LEA",		SUP_8086,	OPE_RM, 0x06, 0x41, 0x8d }, /* wd,all no-w no-reg (cancel seg:) */
     { "LEAVE",		SUP_80186,	NO_PARAM, 1, 0xc9 },
     { "LES",		SUP_8086,	OPE_RM, 0x86, 0x41, 0xc4 }, /* wd,s no-w no-reg */
     { "LFS",		SUP_i386,	OPE_RM, 0x86, 0x42, 0x0f, 0xb4 }, /* wd,s no-w no-reg */
     { "LGDT",		SUP_80286P,	OPE_M, 0x2e, 0x62, 0x0f, 0x01 }, /* wds no-w no-o16/o32 no-reg */
     { "LGS",		SUP_i386,	OPE_RM, 0x86, 0x42, 0x0f, 0xb5 }, /* wd,s no-w no-reg */
     { "LIDT",		SUP_80286P,	OPE_M, 0x3e, 0x62, 0x0f, 0x01 }, /* wds no-w no-o16/o32 no-reg */
     { "LLDT",		SUP_80286P,	OPE_M, 0x2a, 0x22, 0x0f, 0x00 }, /* ws no-w no-o16/o32 */
     { "LMSW",		SUP_80286P, 	OPE_M, 0x6a, 0x22, 0x0f, 0x01 }, /* ws no-w no-o16/o32 */
     { "LOCK",		SUP_8086,	PREFIX, 0x02 },
     { "LODSB",		SUP_8086,	NO_PARAM, 1 | DEF_DS, 0xac },
     { "LODSD",		SUP_i386,	NO_PARAM, 1 | OPE32 | DEF_DS, 0xad },
     { "LODSW",		SUP_8086,	NO_PARAM, 1 | OPE16 | DEF_DS, 0xad },
     { "LOOP",		SUP_8086,	OPE_LOOP, 0xe2, 0 },
     { "LOOPE",		SUP_8086,	OPE_LOOP, 0xe1, 0 },
     { "LOOPNE",	SUP_8086,	OPE_LOOP, 0xe0, 0 },
     { "LOOPNZ",	SUP_8086,	OPE_LOOP, 0xe0, 0 },
     { "LOOPZ",		SUP_8086,	OPE_LOOP, 0xe1, 0 },
     { "LSL",		SUP_80286P,	OPE_RM, 0xa6, 0x02, 0x0f, 0x03 }, /* wd,ws no-w */
     { "LSS",		SUP_8086,	OPE_RM, 0x86, 0x42, 0x0f, 0xb2 }, /* wd,s no-w no-reg */
     { "LTR",		SUP_80286P,	OPE_M, 0x3a, 0x22, 0x0f, 0x00 }, /* ws no-w no-o16/o32 */
     { "MOV",		SUP_8086,	OPE_MOV },
     { "MOVSB",		SUP_8086,	NO_PARAM, 1 | DEF_DS, 0xa4 },
     { "MOVSD",		SUP_i386,	NO_PARAM, 1 | OPE32 | DEF_DS, 0xa5 },
     { "MOVSW",		SUP_8086,	NO_PARAM, 1 | OPE16 | DEF_DS, 0xa5 },
     { "MOVSX",		SUP_i386,	OPE_MOVZX, 0xbf },
     { "MOVZX",		SUP_i386,	OPE_MOVZX, 0xb7 },
     { "MUL",		SUP_8086,	OPE_M, 0x47, 0x11, 0xf6 }, /* bwd w0 */
     { "NEG",		SUP_8086,	OPE_M, 0x37, 0x11, 0xf6 }, /* bwd w0 */
     { "NOP",		SUP_8086,	NO_PARAM, 1, 0x90 },
     { "NOT",		SUP_8086,	OPE_M, 0x27, 0x11, 0xf6 }, /* bwd w0 */
     { "OR",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x01 << 3 },
     { "ORG",		SUP_8086,	OPE_ORG },
     { "OUT",		SUP_8086,	OPE_INOUT, 0xe6 },
     { "OUTSB",		SUP_8086,	NO_PARAM, 1 | DEF_DS, 0x6e },
     { "OUTSD",		SUP_i386,	NO_PARAM, 1 | OPE32 | DEF_DS, 0x6f },
     { "OUTSW",		SUP_8086,	NO_PARAM, 1 | OPE16 | DEF_DS, 0x6f },

     { "POP",		SUP_8086,	OPE_PUSH, 0x08, 0x58, 0x8f, 0x07, 0xa1 },
     { "POPA",		SUP_80186,	NO_PARAM, 1, 0x61 },
     { "POPAD",		SUP_i386,	NO_PARAM, 1 | OPE32, 0x61 },
     { "POPAW",		SUP_80186,	NO_PARAM, 1 | OPE16, 0x61 },
     { "POPF",		SUP_8086,	NO_PARAM, 1, 0x9d },
     { "POPFD",		SUP_i386,	NO_PARAM, 1 | OPE32, 0x9d },
     { "POPFW",		SUP_8086,	NO_PARAM, 1 | OPE16, 0x9d },
     { "PUSH",		SUP_8086,	OPE_PUSH, 0x1e, 0x50, 0xff, 0x06, 0xa0 },
     { "PUSHA",		SUP_80186,	NO_PARAM, 1, 0x60 },
     { "PUSHAD",	SUP_i386,	NO_PARAM, 1 | OPE32, 0x60 },
     { "PUSHAW",	SUP_80186,	NO_PARAM, 1 | OPE16, 0x60 },
     { "PUSHF",		SUP_8086,	NO_PARAM, 1, 0x9c },
     { "PUSHFD",	SUP_i386,	NO_PARAM, 1 | OPE32, 0x9c },
     { "PUSHFW",	SUP_8086,	NO_PARAM, 1 | OPE16, 0x9c },

     { "RCL",		SUP_8086,	OPE_SHIFT, 0x02 },
     { "RCR",		SUP_8086,	OPE_SHIFT, 0x03 },
     { "REP",		SUP_8086,	PREFIX, 0x00 },
     { "REPE",		SUP_8086,	PREFIX, 0x00 },
     { "REPNE",		SUP_8086,	PREFIX, 0x01 },
     { "REPNZ",		SUP_8086,	PREFIX, 0x01 },
     { "REPZ",		SUP_8086,	PREFIX, 0x00 },
     { "RESB",		SUP_8086,	OPE_RESB, 1 },
     { "RESD",		SUP_8086,	OPE_RESB, 4 },
     { "RESQ",		SUP_8086,	OPE_RESB, 8 },
     { "REST",		SUP_8086,	OPE_RESB, 10 },
     { "RESW",		SUP_8086,	OPE_RESB, 2 },
     { "RET",		SUP_8086,	OPE_RET, 0xc2 },
     { "RETF",		SUP_8086,	OPE_RET, 0xca },
     { "RETN",		SUP_8086,	OPE_RET, 0xc2 },
     { "ROL",		SUP_8086,	OPE_SHIFT, 0x00 },
     { "ROR",		SUP_8086,	OPE_SHIFT, 0x01 },
     { "SAHF",		SUP_8086,	NO_PARAM, 1, 0x9e },
     { "SAL",		SUP_8086,	OPE_SHIFT, 0x04 },
     { "SAR",		SUP_8086,	OPE_SHIFT, 0x07 },
     { "SBB",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x03 << 3 },
     { "SCASB",		SUP_8086,	NO_PARAM, 1, 0xae },
     { "SCASD",		SUP_i386,	NO_PARAM, 1 | OPE32, 0xaf },
     { "SCASW",		SUP_8086,	NO_PARAM, 1 | OPE16, 0xaf },
     { "SETA",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x97 }, /* bs no-w no-o16/o32 */
     { "SETAE",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x93 }, /* bs no-w no-o16/o32 */
     { "SETB",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x92 }, /* bs no-w no-o16/o32 */
     { "SETBE",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x96 }, /* bs no-w no-o16/o32 */
     { "SETC",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x92 }, /* bs no-w no-o16/o32 */
     { "SETE",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x94 }, /* bs no-w no-o16/o32 */
     { "SETG",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9f }, /* bs no-w no-o16/o32 */
     { "SETGE",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9d }, /* bs no-w no-o16/o32 */
     { "SETL",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9c }, /* bs no-w no-o16/o32 */
     { "SETLE",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9e }, /* bs no-w no-o16/o32 */
     { "SETNA",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x96 }, /* bs no-w no-o16/o32 */
     { "SETNAE",	SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x92 }, /* bs no-w no-o16/o32 */
     { "SETNB",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x93 }, /* bs no-w no-o16/o32 */
     { "SETNBE",	SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x97 }, /* bs no-w no-o16/o32 */
     { "SETNC",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x93 }, /* bs no-w no-o16/o32 */
     { "SETNE",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x95 }, /* bs no-w no-o16/o32 */
     { "SETNG",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9e }, /* bs no-w no-o16/o32 */
     { "SETNGE",	SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9c }, /* bs no-w no-o16/o32 */
     { "SETNL",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9d }, /* bs no-w no-o16/o32 */
     { "SETNLE",	SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9f }, /* bs no-w no-o16/o32 */
     { "SETNO",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x91 }, /* bs no-w no-o16/o32 */
     { "SETNP",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9b }, /* bs no-w no-o16/o32 */
     { "SETNS",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x99 }, /* bs no-w no-o16/o32 */
     { "SETNZ",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x95 }, /* bs no-w no-o16/o32 */
     { "SETO",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x90 }, /* bs no-w no-o16/o32 */
     { "SETP",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9a }, /* bs no-w no-o16/o32 */
     { "SETPE",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9a }, /* bs no-w no-o16/o32 */
     { "SETPO",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x9b }, /* bs no-w no-o16/o32 */
     { "SETS",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x98 }, /* bs no-w no-o16/o32 */
     { "SETZ",		SUP_i386,	OPE_M, 0x09, 0x32, 0x0f, 0x94 }, /* bs no-w no-o16/o32 */
     { "SGDT",		SUP_80286P,	OPE_M, 0x0e, 0x62, 0x0f, 0x01 }, /* wds no-w no-o16/o32 no-reg */
     { "SHL",		SUP_8086,	OPE_SHIFT, 0x04 },
     { "SHLD",		SUP_i386,	OPE_SHLD, 0xa4 },
     { "SHR",		SUP_8086,	OPE_SHIFT, 0x05 },
     { "SHRD",		SUP_i386,	OPE_SHLD, 0xac },
     { "SIDT",		SUP_80286P,	OPE_M, 0x1e, 0x62, 0x0f, 0x01 }, /* wds no-w no-o16/o32 no-reg */
     { "SLDT",		SUP_80286P,	OPE_M, 0x0a, 0x22, 0x0f, 0x00 }, /* ws no-w no-o16/o32 */
     { "SMSW",		SUP_80286P,	OPE_M, 0x4a, 0x22, 0x0f, 0x01 }, /* ws no-w no-o16/o32 */
     { "SS",		SUP_8086,	PREFIX, 0x07 },
     { "STC",		SUP_8086,	NO_PARAM, 1, 0xf9 },
     { "STD",		SUP_8086,	NO_PARAM, 1, 0xfd },
     { "STI",		SUP_8086,	NO_PARAM, 1, 0xfb },
     { "STOSB",		SUP_8086,	NO_PARAM, 1, 0xaa },
     { "STOSD",		SUP_i386,	NO_PARAM, 1 | OPE32, 0xab },
     { "STOSW",		SUP_8086,	NO_PARAM, 1 | OPE16, 0xab },
     { "STR",		SUP_80286P,	OPE_M, 0x1a, 0x22, 0x0f, 0x00 }, /* ws no-w no-o16/o32 */
     { "SUB",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x05 << 3 },
     { "TEST",		SUP_8086,	OPE_TEST },
     { "TIMES",		SUP_8086,	OPE_TIMES },
     { "VERR",		SUP_80286P,	OPE_M, 0x4a, 0x22, 0x0f, 0x00 }, /* ws no-w no-o16/o32 */
     { "VERW",		SUP_80286P, OPE_M, 0x5a, 0x22, 0x0f, 0x00 }, /* ws no-w no-o16/o32 */
     { "WAIT",		SUP_8086,	NO_PARAM, 1, 0x9b },
     { "WBINVD",	SUP_i486,	NO_PARAM, 1, 0x0f, 0x09 },
     { "XADD",		SUP_i486,	OPE_MR, 0x87, 0x12, 0x0f, 0xc0 }, /* bwd,s w0 */
     { "XCHG",		SUP_8086,	OPE_XCHG, 0x87, 0x11, 0x86 }, /* bwd,s w0 */
     { "XLATB",		SUP_8086,	NO_PARAM, 1 | DEF_DS, 0xd7 },
     { "XOR",		SUP_8086,	OPE_ADD, 0x87, 0x11, 0x06 << 3 },
     { "", 0, 0 }
}};

#endif /* NASK_DEFS_HPP_ */
