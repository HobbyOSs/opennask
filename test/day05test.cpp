#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "front_end.hh"
#include "demangle.hpp"
#include "diff.hh"

class Day05Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Day05Suite() {
        spdlog::set_level(spdlog::level::debug);
    }

    // 試験終了時に一回だけ実行
    ~Day05Suite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};


TEST_F(Day05Suite, Harib02i) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; オブジェクトファイルを作るモード
[INSTRSET "i486p"]				; 486の命令まで使いたいという記述
[BITS 32]						; 32ビットモード用の機械語を作らせる
[FILE "naskfunc.nas"]			; ソースファイル名情報

		GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL	_io_load_eflags, _io_store_eflags
		GLOBAL	_load_gdtr, _load_idtr

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_io_cli:	; void io_cli(void);
		CLI
		RET

_io_sti:	; void io_sti(void);
		STI
		RET

_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AL,DX
		RET

_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; PUSH EFLAGS という意味
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP EFLAGS という意味
		RET

_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

    std::vector<uint8_t> expected = {};

    // COFFのヘッダー部分
    expected.insert(expected.end(), {
            0x4C, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
    });
    // COFFの各種section
    expected.insert(expected.end(), {
            0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x4F, 0x00, 0x00, 0x00, 0x8C, 0x00, 0x00, 0x00, 0xDB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x10, 0x60
        }); // .text
    expected.insert(expected.end(), {
            0x2E, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x10, 0xC0
        }); // .data
    expected.insert(expected.end(), {
            0x2E, 0x62, 0x73, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x10, 0xC0
        }); // .bss
    expected.insert(expected.end(), {
            0xF4, 0xC3, 0xFA, 0xC3, 0xFB, 0xC3, 0xFB, 0xF4, 0xC3, 0x8B, 0x54, 0x24, 0x04, 0xB8, 0x00, 0x00,
            0x00, 0x00, 0xEC, 0xC3, 0x8B, 0x54, 0x24, 0x04, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x66, 0xED, 0xC3,
            0x8B, 0x54, 0x24, 0x04, 0xED, 0xC3, 0x8B, 0x54, 0x24, 0x04, 0x8A, 0x44, 0x24, 0x08, 0xEE, 0xC3,
            0x8B, 0x54, 0x24, 0x04, 0x8B, 0x44, 0x24, 0x08, 0x66, 0xEF, 0xC3, 0x8B, 0x54, 0x24, 0x04, 0x8B,
            0x44, 0x24, 0x08, 0xEF, 0xC3, 0x9C, 0x58, 0xC3, 0x8B, 0x44, 0x24, 0x04, 0x50, 0x9D, 0xC3
        }); // rawdata
    expected.insert(expected.end(), {
            0x2E, 0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00,
            0x67, 0x01, 0x6E, 0x61, 0x73, 0x6B, 0x66, 0x75, 0x6E, 0x63, 0x2E, 0x6E, 0x61, 0x73, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        }); // .file
    expected.insert(expected.end(), {
            0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
            0x03, 0x01, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        }); // .text
    expected.insert(expected.end(), {
            0x2E, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
            0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        }); // .data
    expected.insert(expected.end(), {
            0x2E, 0x62, 0x73, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
            0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        }); // .bss
    expected.insert(expected.end(), {
            0x5F, 0x69, 0x6F, 0x5F, 0x68, 0x6C, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
            0x02, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x63, 0x6C, 0x69, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
            0x00, 0x00, 0x02, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x73, 0x74, 0x69, 0x00, 0x04, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x06, 0x00,
            0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x69, 0x6E, 0x38, 0x00,
            0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x69, 0x6E,
            0x31, 0x36, 0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5F, 0x69, 0x6F, 0x5F,
            0x69, 0x6E, 0x33, 0x32, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5F, 0x69,
            0x6F, 0x5F, 0x6F, 0x75, 0x74, 0x38, 0x26, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x00, 0x01, 0x00,
            0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x48, 0x00,
            0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x44, 0x00, 0x00, 0x00, 0x5F, 0x69, 0x6F, 0x5F,
            0x73, 0x74, 0x69, 0x68, 0x6C, 0x74, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x6F, 0x75, 0x74, 0x31, 0x36,
            0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x6F, 0x75, 0x74, 0x33, 0x32, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x6C,
            0x6F, 0x61, 0x64, 0x5F, 0x65, 0x66, 0x6C, 0x61, 0x67, 0x73, 0x00, 0x5F, 0x69, 0x6F, 0x5F, 0x73,
            0x74, 0x6F, 0x72, 0x65, 0x5F, 0x65, 0x66, 0x6C, 0x61, 0x67, 0x73, 0x00
        }); // シンボル情報

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}
