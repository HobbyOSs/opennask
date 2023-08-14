#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "front_end.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "diff.hh"

class Day03Suite : public ::testing::Test {
protected:
    // 試験開始時に一回だけ実行
    Day03Suite() {
        spdlog::set_level(spdlog::level::debug);
    }

    // 試験終了時に一回だけ実行
    ~Day03Suite() override {
    }

    // 各テストケース実行前に実行
    void SetUp() override {
    }

    // 各テストケース実行後に実行
    void TearDown() override {
    }
};

TEST_F(Day03Suite, Harib00a) {

    std::stringstream ss;
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
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

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

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}

TEST_F(Day03Suite, Harib00b) {

    std::stringstream ss;
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
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

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

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}

TEST_F(Day03Suite, Harib00c) {

    std::stringstream ss;
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
    auto d = std::make_unique<FrontEnd>(true, true);
    ss << nask_statements;
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

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

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}

TEST_F(Day03Suite, Harib00d) {

    std::stringstream ss;
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
    auto d = std::make_unique<FrontEnd>(true, true);
    ss << nask_statements;
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

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

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}

TEST_F(Day03Suite, Harib00g) {

    std::stringstream ss;
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
		JB		readloop		; DH < 2 だったらreadloopへ
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop		; CH < CYLS だったらreadloopへ

; 読み終わったのでharibote.sysを実行だ！

		MOV		[0x0ff0],CH		; IPLがどこまで読んだのかをメモ
		JMP		0xc200

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
fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ
msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"load error"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);
    std::vector<uint8_t> padding(297, 0);

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
    expected.insert(expected.end(), {0x73, 0x32}); // JAE error
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

    // 読み終わったのでharibote.sysを実行だ！
    expected.insert(expected.end(), {0x88, 0x2e, 0xf0, 0x0f});
    expected.insert(expected.end(), {0xe9, 0x55, 0x45});

    expected.insert(expected.end(), {0xbe, 0xc7, 0x7c}); // MOV SI,msg
    expected.insert(expected.end(), {0x8a, 0x04});
    expected.insert(expected.end(), {0x83, 0xc6, 0x01});
    expected.insert(expected.end(), {0x3c, 0x00});
    expected.insert(expected.end(), {0x74, 0x09}); // JE fin
    expected.insert(expected.end(), {0xb4, 0x0e});
    expected.insert(expected.end(), {0xbb, 0x0f, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xeb, 0xee});

    expected.insert(expected.end(), {0xf4});
    expected.insert(expected.end(), {0xeb, 0xfd});

    expected.insert(expected.end(), {0x0a, 0x0a});
    expected.insert(expected.end(), {0x6c, 0x6f, 0x61, 0x64, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72});

    expected.insert(expected.end(), {0x0a});
    expected.insert(expected.end(), {0x00});
    expected.insert(expected.end(), std::begin(padding), std::end(padding));
    expected.insert(expected.end(), {0x55, 0xaa});

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}


TEST_F(Day03Suite, Harib00h) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; haribote-os
; TAB=4

; BOOT_INFO関係
CYLS	EQU		0x0ff0			; ブートセクタが設定する
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; 色数に関する情報。何ビットカラーか？
SCRNX	EQU		0x0ff4			; 解像度のX
SCRNY	EQU		0x0ff6			; 解像度のY
VRAM	EQU		0x0ff8			; グラフィックバッファの開始番地

		ORG		0xc200			; このプログラムがどこに読み込まれるのか

		MOV		AL,0x13			; VGAグラフィックス、320x200x8bitカラー
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; 画面モードをメモする
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; キーボードのLED状態をBIOSに教えてもらう

		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL

fin:
		HLT
		JMP		fin
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

    std::vector<uint8_t> expected = {};

    // haribote.nas
    expected.insert(expected.end(), {0xb0, 0x13});
    expected.insert(expected.end(), {0xb4, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xc6, 0x06, 0xf2, 0x0f, 0x08}); // BYTE [VMODE],8
    expected.insert(expected.end(), {0xc7, 0x06, 0xf4, 0x0f, 0x40, 0x01}); // WORD [SCRNX],320
    expected.insert(expected.end(), {0xc7, 0x06, 0xf6, 0x0f, 0xc8, 0x00}); // WORD [SCRNY],200
    expected.insert(expected.end(), {0x66, 0xc7, 0x06, 0xf8, 0x0f, 0x00, 0x00, 0x0a, 0x00}); // DWORD [VRAM],0x000a0000

    expected.insert(expected.end(), {0xb4, 0x02});
    expected.insert(expected.end(), {0xcd, 0x16});

    expected.insert(expected.end(), {0xa2});
    expected.insert(expected.end(), {0xf1, 0x0f});
    expected.insert(expected.end(), {0xf4});
    expected.insert(expected.end(), {0xeb, 0xfd});

    // 作成したバイナリの差分assert & diff表示
    ASSERT_PRED_FORMAT2(checkTextF,expected,d->binout_container);
}

TEST_F(Day03Suite, Harib00i) {

    std::stringstream ss;
    const char nask_statements[] = R"(
; haribote-os boot asm
; TAB=4

BOTPAK	EQU		0x00280000		; bootpackのロード先
DSKCAC	EQU		0x00100000		; ディスクキャッシュの場所
DSKCAC0	EQU		0x00008000		; ディスクキャッシュの場所（リアルモード）

; BOOT_INFO関係
CYLS	EQU		0x0ff0			; ブートセクタが設定する
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; 色数に関する情報。何ビットカラーか？
SCRNX	EQU		0x0ff4			; 解像度のX
SCRNY	EQU		0x0ff6			; 解像度のY
VRAM	EQU		0x0ff8			; グラフィックバッファの開始番地

		ORG		0xc200			; このプログラムがどこに読み込まれるのか

; 画面モードを設定

		MOV		AL,0x13			; VGAグラフィックス、320x200x8bitカラー
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; 画面モードをメモする（C言語が参照する）
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; キーボードのLED状態をBIOSに教えてもらう

		MOV		AH,0x02
		INT		0x16			; keyboard BIOS
		MOV		[LEDS],AL

; PICが一切の割り込みを受け付けないようにする
;	AT互換機の仕様では、PICの初期化をするなら、
;	こいつをCLI前にやっておかないと、たまにハングアップする
;	PICの初期化はあとでやる

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						; OUT命令を連続させるとうまくいかない機種があるらしいので
		OUT		0xa1,AL

		CLI						; さらにCPUレベルでも割り込み禁止

; CPUから1MB以上のメモリにアクセスできるように、A20GATEを設定

		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL
		CALL	waitkbdout

; プロテクトモード移行

[INSTRSET "i486p"]				; 486の命令まで使いたいという記述

		LGDT	[GDTR0]			; 暫定GDTを設定
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	; bit31を0にする（ページング禁止のため）
		OR		EAX,0x00000001	; bit0を1にする（プロテクトモード移行のため）
		MOV		CR0,EAX
		JMP		pipelineflush
pipelineflush:
		MOV		AX,1*8			;  読み書き可能セグメント32bit
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; bootpackの転送

		MOV		ESI,bootpack	; 転送元
		MOV		EDI,BOTPAK		; 転送先
		MOV		ECX,512*1024/4
		CALL	memcpy

; ついでにディスクデータも本来の位置へ転送

; まずはブートセクタから

		MOV		ESI,0x7c00		; 転送元
		MOV		EDI,DSKCAC		; 転送先
		MOV		ECX,512/4
		CALL	memcpy

; 残り全部

		MOV		ESI,DSKCAC0+512	; 転送元
		MOV		EDI,DSKCAC+512	; 転送先
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; シリンダ数からバイト数/4に変換
		SUB		ECX,512/4		; IPLの分だけ差し引く
		CALL	memcpy

; asmheadでしなければいけないことは全部し終わったので、
;	あとはbootpackに任せる

; bootpackの起動

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; 転送するべきものがない
		MOV		ESI,[EBX+20]	; 転送元
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; 転送先
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; スタック初期値
		JMP		DWORD 2*8:0x0000001b

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; ANDの結果が0でなければwaitkbdoutへ
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		;JNZ		memcpy			; 引き算した結果が0でなければmemcpyへ
		;RET
; memcpyはアドレスサイズプリフィクスを入れ忘れなければ、ストリング命令でも書ける

		;ALIGNB	16
GDT0:
		;RESB	8				; ヌルセレクタ
		;DW		0xffff,0x0000,0x9200,0x00cf	; 読み書き可能セグメント32bit
		;DW		0xffff,0x0000,0x9a28,0x0047	; 実行可能セグメント32bit（bootpack用）

		;DW		0
GDTR0:
		;DW		8*3-1
		;DD		GDT0

		;ALIGNB	16
bootpack:
)";

    // od形式で出力する際は `od -t x1 test/test.img > test_img.txt`
    ss << nask_statements;
    auto d = std::make_unique<FrontEnd>(true, true);
    auto pt = d->Parse<Program>(ss);
    d->Eval<Program>(pt.get(), "test.img");

    // 差分がわかりやすいよう調整
    std::vector<uint8_t> expected = {};

    // haribote.nas
    expected.insert(expected.end(), {0xb0, 0x13});
    expected.insert(expected.end(), {0xb4, 0x00});
    expected.insert(expected.end(), {0xcd, 0x10});
    expected.insert(expected.end(), {0xc6, 0x06, 0xf2, 0x0f, 0x08}); // BYTE [VMODE],8
    expected.insert(expected.end(), {0xc7, 0x06, 0xf4, 0x0f, 0x40, 0x01}); // WORD [SCRNX],320
    expected.insert(expected.end(), {0xc7, 0x06, 0xf6, 0x0f, 0xc8, 0x00}); // WORD [SCRNY],200
    expected.insert(expected.end(), {0x66, 0xc7, 0x06, 0xf8, 0x0f, 0x00, 0x00, 0x0a, 0x00}); // DWORD [VRAM],0x000a0000

    expected.insert(expected.end(), {0xb4, 0x02});
    expected.insert(expected.end(), {0xcd, 0x16});
    expected.insert(expected.end(), {0xa2, 0xf1, 0x0f});

    expected.insert(expected.end(), {0xb0, 0xff});
    expected.insert(expected.end(), {0xe6, 0x21});
    expected.insert(expected.end(), {0x90});
    expected.insert(expected.end(), {0xe6, 0xa1});
    expected.insert(expected.end(), {0xfa});
    expected.insert(expected.end(), {0xe8, 0xb5, 0x00}); // CALL waitkbdout
    expected.insert(expected.end(), {0xb0, 0xd1});
    expected.insert(expected.end(), {0xe6, 0x64});
    expected.insert(expected.end(), {0xe8, 0xae, 0x00}); // CALL waitkbdout
    expected.insert(expected.end(), {0xb0, 0xdf});
    expected.insert(expected.end(), {0xe6, 0x60});
    expected.insert(expected.end(), {0xe8, 0xa7, 0x00}); // CALL waitkbdout

    expected.insert(expected.end(), {0x0f, 0x01, 0x16, 0x2a, 0xc3}); // LGDT[GDTR0]
    expected.insert(expected.end(), {0x0f, 0x20, 0xc0});
    expected.insert(expected.end(), {0x66, 0x25, 0xff, 0xff, 0xff, 0x7f});
    expected.insert(expected.end(), {0x66, 0x83, 0xc8, 0x01});
    expected.insert(expected.end(), {0x0f, 0x22, 0xc0});
    expected.insert(expected.end(), {0xeb, 0x00});

    // pipelineflush:
    expected.insert(expected.end(), {0xb8, 0x08, 0x00});
    expected.insert(expected.end(), {0x8e, 0xd8});
    expected.insert(expected.end(), {0x8e, 0xc0});
    expected.insert(expected.end(), {0x8e, 0xe0});
    expected.insert(expected.end(), {0x8e, 0xe8});
    expected.insert(expected.end(), {0x8e, 0xd0});

    // bootpackの転送
    expected.insert(expected.end(), {0x66, 0xbe, 0x30, 0xc3, 0x00, 0x00});
    expected.insert(expected.end(), {0x66, 0xbf, 0x00, 0x00, 0x28, 0x00});
    expected.insert(expected.end(), {0x66, 0xb9, 0x00, 0x00, 0x02, 0x00});
    expected.insert(expected.end(), {0xe8, 0x60, 0x00});

    // まずはブートセクタから
    expected.insert(expected.end(), {0x66, 0xbe, 0x00, 0x7c, 0x00, 0x00});
    expected.insert(expected.end(), {0x66, 0xbf, 0x00, 0x00, 0x10, 0x00});
    expected.insert(expected.end(), {0x66, 0xb9, 0x80, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), {0xe8, 0x60, 0x00});

    // 残り全部
    expected.insert(expected.end(), {0x66, 0xbe, 0x00, 0x82, 0x00, 0x00});
    expected.insert(expected.end(), {0x66, 0xbf, 0x00, 0x02, 0x10, 0x00});
    expected.insert(expected.end(), {0x66, 0xb9, 0x00, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), {0x8a, 0x0e, 0xf0, 0x0f});
    expected.insert(expected.end(), {0x66, 0x69, 0xc9, 0x00, 0x12, 0x00, 0x00});
    expected.insert(expected.end(), {0x66, 0x81, 0xe9, 0x80, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), {0xe8, 0x39, 0x00});

    // bootpackの起動
    expected.insert(expected.end(), {0x66, 0xbb, 0x00, 0x00, 0x28, 0x00});
    expected.insert(expected.end(), {0x67, 0x66, 0x8b, 0x4b, 0x10});
    expected.insert(expected.end(), {0x66, 0x83, 0xc1, 0x03});
    expected.insert(expected.end(), {0x66, 0xc1, 0xe9, 0x02});
    expected.insert(expected.end(), {0x74, 0x10});
    expected.insert(expected.end(), {0x67, 0x66, 0x8b, 0x73, 0x14});
    expected.insert(expected.end(), {0x66, 0x01, 0xde});
    expected.insert(expected.end(), {0x67, 0x66, 0x8b, 0x7b, 0x0c});
    expected.insert(expected.end(), {0xe8, 0x14, 0x00});
    // skip:
    expected.insert(expected.end(), {0x67, 0x66, 0x8b, 0x63, 0x0c});
    expected.insert(expected.end(), {0x66, 0xea, 0x1b, 0x00, 0x00, 0x00, 0x10, 0x00});

    // waitkbdout:
    expected.insert(expected.end(), {0xe4, 0x64});
    expected.insert(expected.end(), {0x24, 0x02});
    expected.insert(expected.end(), {0x75, 0xfa});
    expected.insert(expected.end(), {0xc3});

    // memcpy:
    expected.insert(expected.end(), {0x67, 0x66, 0x8b, 0x06});
    expected.insert(expected.end(), {0x66, 0x83, 0xc6, 0x04});
    expected.insert(expected.end(), {0x67, 0x66, 0x89, 0x07});
    expected.insert(expected.end(), {0x66, 0x83, 0xc7, 0x04});
    expected.insert(expected.end(), {0x66, 0x83, 0xe9, 0x01});

    // 作成したバイナリの差分assert & diff表示
    //GTEST_SKIP(); // TODO: まだ機能しない
    ASSERT_PRED_FORMAT2(checkTextF, expected, d->binout_container);
}
