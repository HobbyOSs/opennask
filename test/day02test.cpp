#include "driver.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

auto logger = spdlog::stdout_logger_mt("opennask", "console");

TEST_GROUP(day02_suite)
{

};

TEST(day02_suite, helloos3) {

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

		;MOV		SI,msg
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
		;JMP		fin				; 無限ループ

;msg:
		;DB		0x0a, 0x0a		; 改行を2つ
		;DB		"hello, world"
		;DB		0x0a			; 改行
		;DB		0

		;RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		;DB		0x55, 0xaa

; 以下はブートセクタ以外の部分の記述

		;DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		;RESB	4600
		;DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		;RESB	1469432
)";

    std::unique_ptr<Driver> d(new Driver(true, true));
    d->Parse<Program>(nask_statements, "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);

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

    expected.insert(expected.end(), {0xb8, 0x00, 0x00});
    expected.insert(expected.end(), {0x8e, 0xd0});
    expected.insert(expected.end(), {0xbc, 0x00, 0x7c});
    expected.insert(expected.end(), {0x8e, 0xd8});
    expected.insert(expected.end(), {0x8e, 0xc0});

    expected.insert(expected.end(), {0x8a, 0x04});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x3c, 0x00});
    expected.insert(expected.end(), {0x74, 0x00});
    expected.insert(expected.end(), {0xb4, 0x0e});
    expected.insert(expected.end(), {0xbb, 0x0f, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xeb, 0xee});
    expected.insert(expected.end(), {0xf4});


    CHECK_EQUAL(expected.size(), d->binout_container.size());
    //CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)
    //args.push_back("TEST(day02_suite, helloos2)");

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
