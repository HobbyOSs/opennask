#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "front_end.hh"
#include "pass1_strategy.hh"
#include "demangle.hpp"
#include "spdlog/spdlog.h"
#include "diff.hh"

class Day02Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Day02Suite() {
        spdlog::set_level(spdlog::level::debug);
    }

    // 試験終了時に一回だけ実行
    ~Day02Suite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(Day02Suite, Helloos3) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; hello-os
; TAB=4

		ORG		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; ブートセクタの名前を自由に書いてよい（8バイト）
		DW		512				; 1セクタの大きさ（512にしなければいけない）
		DB		1				; クラスタの大きさ（1セクタにしなければいけない）
		DW		1				; FATがどこから始まるか（普通は1セクタ目からにする）
		DB		2				; FATの個数（2にしなければいけない）
		DW		224				; ルートディレクトリ領域の大きさ（普通は224エントリにする）
		DW		2880			; このドライブの大きさ（2880セクタにしなければいけない）
		DB		0xf0			; メディアのタイプ（0xf0にしなければいけない）
		DW		9				; FAT領域の長さ（9セクタにしなければいけない）
		DW		18				; 1トラックにいくつのセクタがあるか（18にしなければいけない）
		DW		2				; ヘッドの数（2にしなければいけない）
		DD		0				; パーティションを使ってないのでここは必ず0
		DD		2880			; このドライブ大きさをもう一度書く
		DB		0,0,0x29		; よくわからないけどこの値にしておくといいらしい
		DD		0xffffffff		; たぶんボリュームシリアル番号
		DB		"HELLO-OS   "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		ES,AX

		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; SIに1を足す
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 一文字表示ファンクション
		MOV		BX,15			; カラーコード
		INT		0x10			; ビデオBIOS呼び出し
		JMP		putloop
fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ

msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"hello, world"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa

; 以下はブートセクタ以外の部分の記述

		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	4600
		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	1469432
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);
    std::vector<uint8_t> resb378(378, 0);
    std::vector<uint8_t> resb4600(4600, 0);
    std::vector<uint8_t> resb1469432(1469432, 0);

    // 以下は標準的なFAT12フォーマットフロッピーディスクのための記述
    expected.insert(expected.end(), {0xeb, 0x4e});
    expected.insert(expected.end(), {0x90});
    expected.insert(expected.end(), {0x48, 0x45, 0x4c, 0x4c, 0x4f, 0x49, 0x50, 0x4c});
    expected.insert(expected.end(), {0x00, 0x02});
	expected.insert(expected.end(), {0x01});
	expected.insert(expected.end(), {0x01, 0x00});
	expected.insert(expected.end(), {0x02});
    expected.insert(expected.end(), {0xe0, 0x00});
	expected.insert(expected.end(), {0x40, 0x0b});
    expected.insert(expected.end(), {0xf0});
	expected.insert(expected.end(), {0x09, 0x00});
	expected.insert(expected.end(), {0x12, 0x00});
	expected.insert(expected.end(), {0x02, 0x00});
    expected.insert(expected.end(), {0x00, 0x00, 0x00, 0x00});
	expected.insert(expected.end(), {0x40, 0x0b, 0x00, 0x00});
	expected.insert(expected.end(), {0x00, 0x00, 0x29});
	expected.insert(expected.end(), {0xff, 0xff, 0xff, 0xff});
    expected.insert(expected.end(), {0x48, 0x45, 0x4c, 0x4c, 0x4f, 0x2d, 0x4f, 0x53, 0x20, 0x20, 0x20});
    expected.insert(expected.end(), {0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20});
	expected.insert(expected.end(), std::begin(resb18), std::end(resb18));

    // プログラム本体
    expected.insert(expected.end(), {0xb8, 0x00, 0x00});
    expected.insert(expected.end(), {0x8e, 0xd0});
    expected.insert(expected.end(), {0xbc, 0x00, 0x7c});
    expected.insert(expected.end(), {0x8e, 0xd8});
    expected.insert(expected.end(), {0x8e, 0xc0});
    // MOV命令からラベルまでの距離をWORD単位で格納(リトルエンディアン)
    // 0x74(=116), 0x74は全体のバイト数の累積となる
    expected.insert(expected.end(), {0xbe, 0x74, 0x7c});
    expected.insert(expected.end(), {0x8a, 0x04});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x3c, 0x00});
    expected.insert(expected.end(), {0x74, 0x09});
    expected.insert(expected.end(), {0xb4, 0x0e});
    expected.insert(expected.end(), {0xbb, 0x0f, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xeb, 0xee});
    expected.insert(expected.end(), {0xf4});
    expected.insert(expected.end(), {0xeb, 0xfd});

    expected.insert(expected.end(), {0x0a, 0x0a});
    expected.insert(expected.end(), {0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64});
    expected.insert(expected.end(), {0x0a});
    expected.insert(expected.end(), {0x00});
    expected.insert(expected.end(), std::begin(resb378), std::end(resb378));

    expected.insert(expected.end(), {0x55, 0xaa});
    expected.insert(expected.end(), {0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), std::begin(resb4600), std::end(resb4600));
    expected.insert(expected.end(), {0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), std::begin(resb1469432), std::end(resb1469432));

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}
