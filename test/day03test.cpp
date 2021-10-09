#include "driver.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"
#include "diff.hh"

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

auto logger = spdlog::stdout_logger_mt("opennask", "console");

TEST_GROUP(day03_suite)
{

};

TEST(day03_suite, harib00a) {

    const char nask_statements[] = R"(
; haribote-ipl
; TAB=4

		ORG		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ブートセクタの名前を自由に書いてよい（8バイト）
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
		DB		"HARIBOTEOS "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ディスクを読む

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; シリンダ0
		MOV		DH,0			; ヘッド0
		MOV		CL,2			; セクタ2

		MOV		AH,0x02			; AH=0x02 : ディスク読み込み
		MOV		AL,1			; 1セクタ
		MOV		BX,0
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ディスクBIOS呼び出し
		JC		error

; 読み終わったけどとりあえずやることないので寝る

fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ

error:
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
msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"load error"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    std::unique_ptr<Driver> d(new Driver(true, true));
    d->Parse<Program>(nask_statements, "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);
    std::vector<uint8_t> resb358(358, 0);

    // 以下は標準的なFAT12フォーマットフロッピーディスクのための記述
    expected.insert(expected.end(), {0xeb, 0x4e});
    expected.insert(expected.end(), {0x90});
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45});
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
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45, 0x4f, 0x53, 0x20});
    expected.insert(expected.end(), {0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20});
    expected.insert(expected.end(), std::begin(resb18), std::end(resb18));

    // プログラム本体
    expected.insert(expected.end(), {0xb8, 0x00, 0x00});
    expected.insert(expected.end(), {0x8e, 0xd0});
    expected.insert(expected.end(), {0xbc, 0x00, 0x7c});
    expected.insert(expected.end(), {0x8e, 0xd8});
    // ディスクを読む
    expected.insert(expected.end(), {0xb8, 0x20, 0x08});
    expected.insert(expected.end(), {0x8e, 0xc0});
    expected.insert(expected.end(), {0xb5, 0x00});
    expected.insert(expected.end(), {0xb6, 0x00});
    expected.insert(expected.end(), {0xb1, 0x02});

    expected.insert(expected.end(), {0xb4, 0x02});
    expected.insert(expected.end(), {0xb0, 0x01});
    expected.insert(expected.end(), {0xbb, 0x00, 0x00});
    expected.insert(expected.end(), {0xb2, 0x00});
    expected.insert(expected.end(), {0xcd, 0x13});
    expected.insert(expected.end(), {0x72, 0x03});
    // 読み終わったけどとりあえずやることないので寝る
    expected.insert(expected.end(), {0xf4});
    expected.insert(expected.end(), {0xeb, 0xfd});
    expected.insert(expected.end(), {0xbe, 0x8a, 0x7c});
    expected.insert(expected.end(), {0x8a, 0x04});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x3c, 0x00});
    expected.insert(expected.end(), {0x74, 0xf1});
    expected.insert(expected.end(), {0xb4, 0x0e});
    expected.insert(expected.end(), {0xbb, 0x0f, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xeb, 0xee});

    expected.insert(expected.end(), {0x0a, 0x0a});
    expected.insert(expected.end(), {0x6c, 0x6f, 0x61, 0x64, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72});

    expected.insert(expected.end(), {0x0a});
    expected.insert(expected.end(), {0x00});
    expected.insert(expected.end(), std::begin(resb358), std::end(resb358));
    expected.insert(expected.end(), {0x55, 0xaa});

    CHECK_EQUAL(expected.size(), d->binout_container.size());
    CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}

TEST(day03_suite, harib00b) {

    const char nask_statements[] = R"(
; haribote-ipl
; TAB=4

		ORG		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ブートセクタの名前を自由に書いてよい（8バイト）
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
		DB		"HARIBOTEOS "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ディスクを読む

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; シリンダ0
		MOV		DH,0			; ヘッド0
		MOV		CL,2			; セクタ2

		MOV		SI,0			; 失敗回数を数えるレジスタ
retry:
		MOV		AH,0x02			; AH=0x02 : ディスク読み込み
		MOV		AL,1			; 1セクタ
		MOV		BX,0
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ディスクBIOS呼び出し
		JNC		fin				; エラーがおきなければfinへ
		ADD		SI,1			; SIに1を足す
		CMP		SI,5			; SIと5を比較
		JAE		error			; SI >= 5 だったらerrorへ
		MOV		AH,0x00
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ドライブのリセット
		JMP		retry

; 読み終わったけどとりあえずやることないので寝る

fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ

error:
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
msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"load error"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    std::unique_ptr<Driver> d(new Driver(true, true));
    d->Parse<Program>(nask_statements, "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);
    std::vector<uint8_t> resb339(339, 0);

    // 以下は標準的なFAT12フォーマットフロッピーディスクのための記述
    expected.insert(expected.end(), {0xeb, 0x4e});
    expected.insert(expected.end(), {0x90});
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45});
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
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45, 0x4f, 0x53, 0x20});
    expected.insert(expected.end(), {0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20});
    expected.insert(expected.end(), std::begin(resb18), std::end(resb18));

    // プログラム本体
    expected.insert(expected.end(), {0xb8, 0x00, 0x00});
    expected.insert(expected.end(), {0x8e, 0xd0});
    expected.insert(expected.end(), {0xbc, 0x00, 0x7c});
    expected.insert(expected.end(), {0x8e, 0xd8});
    // ディスクを読む
    expected.insert(expected.end(), {0xb8, 0x20, 0x08});
    expected.insert(expected.end(), {0x8e, 0xc0});
    expected.insert(expected.end(), {0xb5, 0x00});
    expected.insert(expected.end(), {0xb6, 0x00});
    expected.insert(expected.end(), {0xb1, 0x02});
    expected.insert(expected.end(), {0xbe, 0x00, 0x00});

    expected.insert(expected.end(), {0xb4, 0x02});
    expected.insert(expected.end(), {0xb0, 0x01});
    expected.insert(expected.end(), {0xbb, 0x00, 0x00});
    expected.insert(expected.end(), {0xb2, 0x00});
    expected.insert(expected.end(), {0xcd, 0x13});
    expected.insert(expected.end(), {0x73, 0x10});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x83, 0xfe, 0x05});
    expected.insert(expected.end(), {0x73, 0x0b});
    expected.insert(expected.end(), {0xb4, 0x00});
    expected.insert(expected.end(), {0xb2, 0x00});
    expected.insert(expected.end(), {0xcd, 0x13});
    expected.insert(expected.end(), {0xeb, 0xe3});
    expected.insert(expected.end(), {0xf4});
    expected.insert(expected.end(), {0xeb, 0xfd});
    expected.insert(expected.end(), {0xbe, 0x9d, 0x7c});
    expected.insert(expected.end(), {0x8a, 0x04});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x3c, 0x00});
    expected.insert(expected.end(), {0x74, 0xf1});
    expected.insert(expected.end(), {0xb4, 0x0e});
    expected.insert(expected.end(), {0xbb, 0x0f, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xeb, 0xee});

    expected.insert(expected.end(), {0x0a, 0x0a});
    expected.insert(expected.end(), {0x6c, 0x6f, 0x61, 0x64, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72});

    expected.insert(expected.end(), {0x0a});
    expected.insert(expected.end(), {0x00});
    expected.insert(expected.end(), std::begin(resb339), std::end(resb339));
    expected.insert(expected.end(), {0x55, 0xaa});

    CHECK_EQUAL(expected.size(), d->binout_container.size());
    CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}

TEST(day03_suite, harib00c) {

    const char nask_statements[] = R"(
; haribote-ipl
; TAB=4

		ORG		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ブートセクタの名前を自由に書いてよい（8バイト）
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
		DB		"HARIBOTEOS "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ディスクを読む

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; シリンダ0
		MOV		DH,0			; ヘッド0
		MOV		CL,2			; セクタ2
readloop:
		MOV		SI,0			; 失敗回数を数えるレジスタ
retry:
		MOV		AH,0x02			; AH=0x02 : ディスク読み込み
		MOV		AL,1			; 1セクタ
		MOV		BX,0
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ディスクBIOS呼び出し
		JNC		next			; エラーがおきなければnextへ
		ADD		SI,1			; SIに1を足す
		CMP		SI,5			; SIと5を比較
		JAE		error			; SI >= 5 だったらerrorへ
		MOV		AH,0x00
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ドライブのリセット
		JMP		retry
next:
		MOV		AX,ES			; アドレスを0x200進める
		ADD		AX,0x0020
		MOV		ES,AX			; ADD ES,0x020 という命令がないのでこうしている
		ADD		CL,1			; CLに1を足す
		CMP		CL,18			; CLと18を比較
		JBE		readloop		; CL <= 18 だったらreadloopへ

; 読み終わったけどとりあえずやることないので寝る

fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ

error:
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
msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"load error"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    std::unique_ptr<Driver> d(new Driver(true, true));
    d->Parse<Program>(nask_statements, "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);
    std::vector<uint8_t> resb324(324, 0);

    // 以下は標準的なFAT12フォーマットフロッピーディスクのための記述
    expected.insert(expected.end(), {0xeb, 0x4e});
    expected.insert(expected.end(), {0x90});
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45});
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
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45, 0x4f, 0x53, 0x20});
    expected.insert(expected.end(), {0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20});
    expected.insert(expected.end(), std::begin(resb18), std::end(resb18));

    // プログラム本体
    expected.insert(expected.end(), {0xb8, 0x00, 0x00});
    expected.insert(expected.end(), {0x8e, 0xd0});
    expected.insert(expected.end(), {0xbc, 0x00, 0x7c});
    expected.insert(expected.end(), {0x8e, 0xd8});
    // ディスクを読む
    expected.insert(expected.end(), {0xb8, 0x20, 0x08});
    expected.insert(expected.end(), {0x8e, 0xc0});
    expected.insert(expected.end(), {0xb5, 0x00});
    expected.insert(expected.end(), {0xb6, 0x00});
    expected.insert(expected.end(), {0xb1, 0x02});
    expected.insert(expected.end(), {0xbe, 0x00, 0x00});

    expected.insert(expected.end(), {0xb4, 0x02});
    expected.insert(expected.end(), {0xb0, 0x01});
    expected.insert(expected.end(), {0xbb, 0x00, 0x00});
    expected.insert(expected.end(), {0xb2, 0x00});
    expected.insert(expected.end(), {0xcd, 0x13});
    expected.insert(expected.end(), {0x73, 0x10});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x83, 0xfe, 0x05});
    expected.insert(expected.end(), {0x73, 0x1a});
    expected.insert(expected.end(), {0xb4, 0x00});
    expected.insert(expected.end(), {0xb2, 0x00});
    expected.insert(expected.end(), {0xcd, 0x13});
    expected.insert(expected.end(), {0xeb, 0xe3});
    expected.insert(expected.end(), {0x8c, 0xc0});
    expected.insert(expected.end(), {0x05, 0x20, 0x00});
    expected.insert(expected.end(), {0x8e, 0xc0});
    expected.insert(expected.end(), {0x80, 0xc1, 0x01});
    expected.insert(expected.end(), {0x80, 0xf9, 0x12});
    expected.insert(expected.end(), {0x76, 0xd1});

    // 読み終わったけどとりあえずやることないので寝る
    expected.insert(expected.end(), {0xf4});
    expected.insert(expected.end(), {0xeb, 0xfd});
    expected.insert(expected.end(), {0xbe, 0xac, 0x7c}); // 0xbeac7c
    expected.insert(expected.end(), {0x8a, 0x04});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x3c, 0x00});
    expected.insert(expected.end(), {0x74, 0xf1});
    expected.insert(expected.end(), {0xb4, 0x0e});
    expected.insert(expected.end(), {0xbb, 0x0f, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xeb, 0xee});

    expected.insert(expected.end(), {0x0a, 0x0a});
    expected.insert(expected.end(), {0x6c, 0x6f, 0x61, 0x64, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72});

    expected.insert(expected.end(), {0x0a});
    expected.insert(expected.end(), {0x00});
    expected.insert(expected.end(), std::begin(resb324), std::end(resb324));
    expected.insert(expected.end(), {0x55, 0xaa});

    CHECK_EQUAL(expected.size(), d->binout_container.size());
    CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}

TEST(day03_suite, harib00d) {

    const char nask_statements[] = R"(
; haribote-ipl
; TAB=4

CYLS	EQU		10				; どこまで読み込むか

		ORG		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ブートセクタの名前を自由に書いてよい（8バイト）
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
		DB		"HARIBOTEOS "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ディスクを読む

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; シリンダ0
		MOV		DH,0			; ヘッド0
		MOV		CL,2			; セクタ2
readloop:
		MOV		SI,0			; 失敗回数を数えるレジスタ
retry:
		MOV		AH,0x02			; AH=0x02 : ディスク読み込み
		MOV		AL,1			; 1セクタ
		MOV		BX,0
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ディスクBIOS呼び出し
		JNC		next			; エラーがおきなければnextへ
		ADD		SI,1			; SIに1を足す
		CMP		SI,5			; SIと5を比較
		JAE		error			; SI >= 5 だったらerrorへ
		MOV		AH,0x00
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ドライブのリセット
		JMP		retry
next:
		MOV		AX,ES			; アドレスを0x200進める
		ADD		AX,0x0020
		MOV		ES,AX			; ADD ES,0x020 という命令がないのでこうしている
		ADD		CL,1			; CLに1を足す
		CMP		CL,18			; CLと18を比較
		JBE		readloop		; CL <= 18 だったらreadloopへ
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB			readloop		; DH < 2 だったらreadloopへ
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB			readloop		; CH < CYLS だったらreadloopへ

; 読み終わったけどとりあえずやることないので寝る

fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ

error:
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
msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"load error"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    std::unique_ptr<Driver> d(new Driver(true, true));
    d->Parse<Program>(nask_statements, "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);
    std::vector<uint8_t> resb304(304, 0);

    // 以下は標準的なFAT12フォーマットフロッピーディスクのための記述
    expected.insert(expected.end(), {0xeb, 0x4e});
    expected.insert(expected.end(), {0x90});
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45});
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
    expected.insert(expected.end(), {0x48, 0x41, 0x52, 0x49, 0x42, 0x4f, 0x54, 0x45, 0x4f, 0x53, 0x20});
    expected.insert(expected.end(), {0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20});
    expected.insert(expected.end(), std::begin(resb18), std::end(resb18));

    // プログラム本体
    expected.insert(expected.end(), {0xb8, 0x00, 0x00});
    expected.insert(expected.end(), {0x8e, 0xd0});
    expected.insert(expected.end(), {0xbc, 0x00, 0x7c});
    expected.insert(expected.end(), {0x8e, 0xd8});
    // ディスクを読む
    expected.insert(expected.end(), {0xb8, 0x20, 0x08});
    expected.insert(expected.end(), {0x8e, 0xc0});
    expected.insert(expected.end(), {0xb5, 0x00});
    expected.insert(expected.end(), {0xb6, 0x00});
    expected.insert(expected.end(), {0xb1, 0x02});
    expected.insert(expected.end(), {0xbe, 0x00, 0x00});

    expected.insert(expected.end(), {0xb4, 0x02});
    expected.insert(expected.end(), {0xb0, 0x01});
    expected.insert(expected.end(), {0xbb, 0x00, 0x00});
    expected.insert(expected.end(), {0xb2, 0x00});
    expected.insert(expected.end(), {0xcd, 0x13});
    expected.insert(expected.end(), {0x73, 0x10});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x83, 0xfe, 0x05});
    expected.insert(expected.end(), {0x73, 0x2e});
    expected.insert(expected.end(), {0xb4, 0x00});
    expected.insert(expected.end(), {0xb2, 0x00});
    expected.insert(expected.end(), {0xcd, 0x13});
    expected.insert(expected.end(), {0xeb, 0xe3});
    expected.insert(expected.end(), {0x8c, 0xc0});
    expected.insert(expected.end(), {0x05, 0x20, 0x00});
    expected.insert(expected.end(), {0x8e, 0xc0});
    expected.insert(expected.end(), {0x80, 0xc1, 0x01});
    expected.insert(expected.end(), {0x80, 0xf9, 0x12});
    expected.insert(expected.end(), {0x76, 0xd1});
    expected.insert(expected.end(), {0xb1, 0x01});
    expected.insert(expected.end(), {0x80, 0xc6, 0x01});
    expected.insert(expected.end(), {0x80, 0xfe, 0x02});
    expected.insert(expected.end(), {0x72, 0xc7});
    expected.insert(expected.end(), {0xb6, 0x00});
    expected.insert(expected.end(), {0x80, 0xc5, 0x01});
    expected.insert(expected.end(), {0x80, 0xfd, 0x0a});
    expected.insert(expected.end(), {0x72, 0xbd});

    // 読み終わったけどとりあえずやることないので寝る
    expected.insert(expected.end(), {0xf4});
    expected.insert(expected.end(), {0xeb, 0xfd});
    expected.insert(expected.end(), {0xbe, 0xc0, 0x7c}); // 0xbeac7c
    expected.insert(expected.end(), {0x8a, 0x04});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x3c, 0x00});
    expected.insert(expected.end(), {0x74, 0xf1});
    expected.insert(expected.end(), {0xb4, 0x0e});
    expected.insert(expected.end(), {0xbb, 0x0f, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xeb, 0xee});

    expected.insert(expected.end(), {0x0a, 0x0a});
    expected.insert(expected.end(), {0x6c, 0x6f, 0x61, 0x64, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72});

    expected.insert(expected.end(), {0x0a});
    expected.insert(expected.end(), {0x00});
    expected.insert(expected.end(), std::begin(resb304), std::end(resb304));
    expected.insert(expected.end(), {0x55, 0xaa});

    CHECK_EQUAL(expected.size(), d->binout_container.size());

    std::string msg = "[diff]\n" + diff(expected, d->binout_container);

    CHECK_TEXT(
        std::equal(expected.begin(), expected.end(), d->binout_container.begin()), msg.c_str()
    );
}


int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)
    //args.push_back("TEST(day03_suite, harib00d)");

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
