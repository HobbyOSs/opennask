#include "driver.hh"
#include "demangle.hpp"
#include "tinyexpr.h"
#include "spdlog/spdlog.h"

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

auto logger = spdlog::stdout_logger_mt("opennask", "console");

TEST_GROUP(day01_suite)
{

};

TEST(day01_suite, helloos1) {

    const char nask_statements[] = R"(
	DB	0xeb, 0x4e, 0x90, 0x48, 0x45, 0x4c, 0x4c, 0x4f
	DB	0x49, 0x50, 0x4c, 0x00, 0x02, 0x01, 0x01, 0x00
	DB	0x02, 0xe0, 0x00, 0x40, 0x0b, 0xf0, 0x09, 0x00
	DB	0x12, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
	DB	0x40, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x29, 0xff
	DB	0xff, 0xff, 0xff, 0x48, 0x45, 0x4c, 0x4c, 0x4f
	DB	0x2d, 0x4f, 0x53, 0x20, 0x20, 0x20, 0x46, 0x41
	DB	0x54, 0x31, 0x32, 0x20, 0x20, 0x20, 0x00, 0x00
	RESB	16
	DB	0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
	DB	0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
	DB	0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
	DB	0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
	DB	0xee, 0xf4, 0xeb, 0xfd, 0x0a, 0x0a, 0x68, 0x65
	DB	0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72
	DB	0x6c, 0x64, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00
	RESB	368
	DB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
	DB	0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	RESB	4600
	DB	0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	RESB	1469432
)";

    std::unique_ptr<Driver> d(new Driver(true, true));
    d->Parse<Program>(nask_statements, "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb16(16, 0);
	std::vector<uint8_t> resb368(368, 0);
	std::vector<uint8_t> resb4600(4600, 0);
    std::vector<uint8_t> resb1469432(1469432, 0);

    expected.insert(expected.end(), {
            0xeb, 0x4e, 0x90, 0x48, 0x45, 0x4c, 0x4c, 0x4f
            , 0x49, 0x50, 0x4c, 0x00, 0x02, 0x01, 0x01, 0x00
            , 0x02, 0xe0, 0x00, 0x40, 0x0b, 0xf0, 0x09, 0x00
            , 0x12, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
            , 0x40, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x29, 0xff
            , 0xff, 0xff, 0xff, 0x48, 0x45, 0x4c, 0x4c, 0x4f
            , 0x2d, 0x4f, 0x53, 0x20, 0x20, 0x20, 0x46, 0x41
            , 0x54, 0x31, 0x32, 0x20, 0x20, 0x20, 0x00, 0x00});
    expected.insert(expected.end(), std::begin(resb16), std::end(resb16));
    expected.insert(expected.end(), {
            0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
            , 0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
            , 0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
            , 0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
            , 0xee, 0xf4, 0xeb, 0xfd, 0x0a, 0x0a, 0x68, 0x65
            , 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72
            , 0x6c, 0x64, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), std::begin(resb368), std::end(resb368));
    expected.insert(expected.end(), {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
            , 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), std::begin(resb4600), std::end(resb4600));
    expected.insert(expected.end(), {
            0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00});
    expected.insert(expected.end(), std::begin(resb1469432), std::end(resb1469432));

    CHECK_EQUAL(expected.size(), d->binout_container.size());
    CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}

TEST(day01_suite, helloos2) {

    const char nask_statements[] = R"(
; hello-os
; TAB=4

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		DB		0xeb, 0x4e, 0x90
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

		DB		0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
		DB		0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
		DB		0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
		DB		0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
		DB		0xee, 0xf4, 0xeb, 0xfd

; メッセージ部分

		DB		0x0a, 0x0a		; 改行を2つ
		DB		"hello, world"
		DB		0x0a			; 改行
		DB		0

		RESB	0x1fe-$			; 0x001feまでを0x00で埋める命令

		DB		0x55, 0xaa

; 以下はブートセクタ以外の部分の記述

		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	4600
		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	1469432
)";

    std::unique_ptr<Driver> d(new Driver(true, true));
    d->Parse<Program>(nask_statements, "test.img");

    std::vector<uint8_t> expected = {};
    std::vector<uint8_t> resb18(18, 0);
    std::vector<uint8_t> resb378(378, 0);
	std::vector<uint8_t> resb4600(4600, 0);
    std::vector<uint8_t> resb1469432(1469432, 0);

    expected.insert(expected.end(), {0xeb, 0x4e, 0x90});
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

    expected.insert(expected.end(), {0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c});
    expected.insert(expected.end(), {0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a});
    expected.insert(expected.end(), {0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09});
    expected.insert(expected.end(), {0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb});
    expected.insert(expected.end(), {0xee, 0xf4, 0xeb, 0xfd});

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

    CHECK_EQUAL(expected.size(), d->binout_container.size());
    CHECK_TRUE(std::equal(expected.begin(), expected.end(), d->binout_container.begin()));
}


int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    std::vector<const char*> args(argv, argv + argc); // Insert all arguments
    args.push_back("-v"); // Set verbose mode
    args.push_back("-c"); // Set color output (OPTIONAL)
    //args.push_back("TEST(day01_suite, helloos2)");

    // Run all tests
    int i = RUN_ALL_TESTS(args.size(), &args[0]);
    return i;
}
