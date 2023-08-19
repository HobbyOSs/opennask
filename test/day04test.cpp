#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "front_end.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "diff.hh"

class Day04Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Day04Suite() {
        spdlog::set_level(spdlog::level::debug);
    }

    // 試験終了時に一回だけ実行
    ~Day04Suite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};


TEST_F(Day04Suite, Harib01a) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; オブジェクトファイルを作るモード
[INSTRSET "i486p"]				; 486の命令まで使いたいという記述
[BITS 32]						; 32ビットモード用の機械語を作らせる
[FILE "naskfunc.nas"]			; ソースファイル名情報

		GLOBAL	_io_hlt,_write_mem8

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_write_mem8:	; void write_mem8(int addr, int data);
		MOV		ECX,[ESP+4]		; [ESP+4]にaddrが入っているのでそれをECXに読み込む
		MOV		AL,[ESP+8]		; [ESP+8]にdataが入っているのでそれをALに読み込む
		MOV		[ECX],AL
		RET
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    GTEST_SKIP(); // TODO: 実装
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

    std::vector<uint8_t> expected = {};

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}
