# Technical Notes

## projects/ ディレクトリ構造

```
projects/
├── 01_day/             # ブートセクタ作成の基本
│   ├── CMakeLists.txt
│   ├── helloos0/       # (空のディレクトリ)
│   ├── helloos1/       # 最も単純なブートセクタ ("hello, world" 表示)
│   └── helloos2/       # FAT12ヘッダ情報を含むブートセクタ ("hello, world" 表示)
├── 02_day/             # BIOS を利用した文字表示
│   ├── CMakeLists.txt
│   ├── helloos3/       # BIOS INT 0x10 を使って "hello, world" を表示
│   ├── helloos4/       # helloos3 とほぼ同内容 (ファイル名が ipl.nas に)
│   └── helloos5/       # helloos4 とほぼ同内容
├── 03_day/             # ディスク読み込みとOS本体へのジャンプ、C言語導入
│   ├── CMakeLists.txt
│   ├── harib00a/       # IPL: 1セクタ読み込み、エラー処理
│   ├── harib00b/       # IPL: ディスク読み込みリトライ処理追加
│   ├── harib00c/       # IPL: 複数セクタ読み込み (1ヘッド分)
│   ├── harib00d/       # IPL: 複数シリンダ読み込み
│   ├── harib00e/       # IPL: harib00d と同等 (リファクタリング?)
│   ├── harib00f/       # IPL: OS本体(haribote.nas)をロードし 0xc200 へジャンプ。OSはHLTのみ
│   ├── harib00g/       # IPL: 読み込みシリンダ数を記録。OS: VGAグラフィックモードへ移行
│   ├── harib00h/       # OS: キーボードLED状態取得、画面情報(解像度, VRAMアドレス)記録
│   ├── harib00i/       # asmhead.nas導入: プロテクトモード移行、GDT設定、メモリコピー後、C言語(bootpack.c)へジャンプ。bootpack.cはHLTのみ
│   └── harib00j/       # naskfunc.nas導入: C言語から呼び出すアセンブラ関数(_io_hlt)を提供。bootpack.cから_io_hltを呼び出す
├── 04_day/             # C言語からのVRAM直接アクセス、パレット設定、基本的な図形描画
│   ├── CMakeLists.txt
│   ├── harib01a/       # C言語からアセンブラ関数(`write_mem8`)経由でVRAMを単色(白)で塗りつぶす
│   ├── harib01b/       # C言語のポインタ演算でVRAMにカラーパターンを描画 (`write_mem8`使用せず)
│   ├── harib01c/       # harib01bと同様だが、`naskfunc.nas`から`_write_mem8`を削除
│   ├── harib01d/       # harib01cと同等 (チェックポイント)
│   ├── harib01e/       # harib01dと同等 (チェックポイント)
│   ├── harib01f/       # パレット設定関数(`init_palette`, `set_palette`)を導入 (I/Oポート操作用のアセンブラ関数追加)
│   ├── harib01g/       # 矩形描画関数(`boxfill8`)を導入し、画面に矩形を描画
│   └── harib01h/       # `boxfill8`を使って簡単なウィンドウフレームを描画
├── 05_day/             # 構造体、文字表示、GDT/IDT初期化
│   ├── CMakeLists.txt
│   ├── harib02a/       # BOOTINFO をメモリから読み取り画面初期化 (`init_screen`)
│   ├── harib02b/       # C言語で `struct BOOTINFO` を定義してアクセス
│   ├── harib02c/       # harib02b と同等 (リファクタリング)
│   ├── harib02d/       # フォント描画関数 (`putfont8`) を導入 (文字'A'を描画)
│   ├── harib02e/       # 外部フォントデータ (`hankaku.txt`) を利用して文字を描画
│   ├── harib02f/       # 文字列描画関数 (`putfonts8_asc`) を導入
│   ├── harib02g/       # `sprintf` を利用して画面解像度を表示
│   ├── harib02h/       # マウスカーソル描画関数 (`init_mouse_cursor8`, `putblock8_8`) を導入
│   └── harib02i/       # GDT (Global Descriptor Table) と IDT (Interrupt Descriptor Table) の初期化処理を追加 (`dsctbl.c`, `naskfunc.nas` に関連関数追加)
├── 06_day/             # 割り込み処理 (PIC初期化、IDT設定)
│   ├── CMakeLists.txt
│   ├── harib03a/       # GDT/IDT初期化関数を別ファイル(`dsctbl.c`)に分離
│   ├── harib03b/       # harib03a と同等 (チェックポイント)
│   ├── harib03c/       # ヘッダファイル(`bootpack.h`)導入によるコード整理
│   ├── harib03d/       # PIC (Programmable Interrupt Controller) の初期化 (`int.c`導入)
│   └── harib03e/       # IDTに割り込みハンドラを設定し、PIC経由での割り込み受付を有効化 (キーボード、マウス割り込み)
├── 07_day/             # キーボード/マウス割り込み処理、FIFOバッファ導入
│   ├── CMakeLists.txt
│   ├── harib04a/       # キーボード割り込みハンドラ(`inthandler21`)でキーコードを取得し画面表示
│   ├── harib04b/       # キーボードバッファ(`keybuf`)導入 (1バイトのみ)
│   ├── harib04c/       # キーボードバッファを32バイトに拡張 (簡易FIFO実装)
│   ├── harib04d/       # キーボードバッファをリングバッファ(FIFO)として実装 (`len`, `next_r`, `next_w`)
│   ├── harib04e/       # 汎用FIFO構造体(`struct FIFO8`)と関数群を導入 (`fifo.c`)
│   ├── harib04f/       # キーボードコントローラ初期化(`init_keyboard`)、マウス有効化(`enable_mouse`)
│   └── harib04g/       # マウス用FIFO(`mousefifo`)を追加し、マウス割り込み(`inthandler2c`)でデータ格納
├── 08_day/             # マウス入力処理とカーソル移動
│   ├── CMakeLists.txt
│   ├── harib05a/       # マウスからの3バイトデータ受信処理 (ステートマシン導入) と画面表示
│   ├── harib05b/       # マウスデータ解析構造体(`struct MOUSE_DEC`)と解析関数(`mouse_decode`)導入
│   ├── harib05c/       # マウスデータ解析を改善 (ボタン状態、xy移動量の符号処理)
│   └── harib05d/       # マウスの移動量に応じて画面上のカーソルを移動させる
├── 09_day/             # メモリ管理
│   ├── CMakeLists.txt
│   ├── harib06a/       # キーボード(`keyboard.c`)とマウス(`mouse.c`)関連のコードを分離
│   ├── harib06b/       # メモリ容量チェック機能(`memtest`)を追加
│   ├── harib06c/       # メモリ管理構造体(`struct MEMMAN`)と初期化・空き容量計算関数を導入
│   └── harib06d/       # メモリ確保(`memman_alloc`)・解放(`memman_free`)関数を実装し、メモリ管理機能を分離(`memory.c`)
├── 10_day/             # シート(レイヤー)機能の導入
│   ├── CMakeLists.txt
│   ├── harib07a/       # シート構造体(`struct SHEET`)と管理構造体(`struct SHTCTL`)定義、初期化関数 (`sheet.c` はまだない)
│   ├── harib07b/       # シート確保(`sheet_alloc`)、設定(`sheet_setbuf`)、上下関係設定(`sheet_updown`)関数 (`sheet.c`導入)
│   ├── harib07c/       # シート表示リフレッシュ(`sheet_refreshsub`, `sheet_refresh`)、移動(`sheet_slide`)関数
│   └── harib07d/       # シート解放(`sheet_free`)関数、リフレッシュ範囲最適化
├── 11_day/             # シート機能の改善、ウィンドウ描画
│   ├── CMakeLists.txt
│   ├── harib08a/       # 背景シートとマウスシートを作成し重ね合わせ表示
│   ├── harib08b/       # `sheet_refresh` の範囲指定改善 (クリッピング)
│   ├── harib08c/       # `sheet_refresh` のバグ修正 (マイナス座標対応)
│   ├── harib08d/       # `sheet_slide` のバグ修正 (マイナス座標対応)
│   ├── harib08e/       # `sheet_refresh` の高速化 (矩形範囲計算の最適化)
│   ├── harib08f/       # `putfonts8_asc` をシートに対応 (`sheet_refresh` を利用)
│   ├── harib08g/       # ウィンドウ描画関数 (`make_window8`) を導入
│   └── harib08h/       # ウィンドウにタイトルを描画する機能を追加
├── 12_day/             # タイマー機能の導入と改善
│   ├── CMakeLists.txt
│   ├── harib09a/       # PIT (Programmable Interval Timer) を初期化 (`timer.c`導入、`init_pit`)
│   ├── harib09b/       # タイマー割り込みハンドラ (`inthandler20`) でカウンタをインクリメント (`timerctl.count`)
│   ├── harib09c/       # 指定時間後にFIFOへ通知する機能 (`settimer`, `timerctl.timeout`)
│   ├── harib09d/       # 複数タイマー管理構造 (`struct TIMER`, `timer_alloc`, `timer_init`, `timer_settime`, `timer_free`) 導入 (単純配列版)
│   ├── harib09e/       # タイマー設定を絶対時刻に変更 (`timer_settime`, `inthandler20` 修正)
│   ├── harib09f/       # 次回タイムアウト時刻 (`timerctl.next`) を管理し `inthandler20` を効率化
│   └── harib09g/       # タイマーをタイムアウト時刻順にソートして管理 (`timerctl.timers`, `timerctl.using`) し、割り込み処理を高速化
├── 13_day/             # FIFO統合、文字直接描画、タイマー応用
│   ├── CMakeLists.txt
│   ├── harib10a/       # シート直接描画関数 (`putfonts8_asc_sht`) 導入
│   ├── harib10b/       # 複数のタイマー用FIFOを一つに統合 (`timerfifo`)、データでタイマー種別を区別
│   ├── harib10c/       # タイマーを利用したカウンター表示 (10秒毎更新、3秒毎リセット)
│   ├── harib10d/       # (bootpack.c に大きな変更なし、リファクタリング等か)
│   ├── harib10e/       # (bootpack.c に大きな変更なし、リファクタリング等か)
│   ├── harib10f/       # (bootpack.c に大きな変更なし、リファクタリング等か)
│   ├── harib10g/       # 32bit FIFO (`struct FIFO32`) 導入、キーボード・マウス・タイマーのFIFOを統合
│   ├── harib10h/       # (bootpack.c に大きな変更なし、リファクタリング等か)
│   └── harib10i/       # (bootpack.c に大きな変更なし、リファクタリング等か)
├── 14_day/             # ウィンドウ操作、文字入力、CPUアイドル
│   ├── CMakeLists.txt
│   ├── harib11a/       # タイマー割り当てテスト用関数 `set490` 追加 (未使用)
│   ├── harib11b/       # (bootpack.c に大きな変更なし)
│   ├── harib11c/       # (bootpack.c に大きな変更なし)
│   ├── harib11d/       # (bootpack.c に大きな変更なし)
│   ├── harib11e/       # (bootpack.c に大きな変更なし)
│   ├── harib11f/       # CPUアイドル命令 `io_stihlt` 導入、ウィンドウへの文字表示テスト ('A'キー)
│   ├── harib11g/       # キーコード変換テーブル (`keytable`) 導入、ウィンドウへの文字入力実装
│   ├── harib11h/       # テキスト入力ボックス描画 (`make_textbox8`)、点滅カーソル、バックスペース実装
│   └── harib11i/       # マウス左ボタンでのウィンドウ移動機能実装
├── 15_day/             # マルチタスク機能の導入
│   ├── CMakeLists.txt
│   ├── harib12a/       # TSS (Task State Segment) 構造体定義、GDTにTSSディスクリプタ設定、TRレジスタロード、タスクB用TSS設定
│   ├── harib12b/       # タスクB (`task_b_main`) 実装 (独立FIFO、タイマー、タスクスイッチ処理)
│   ├── harib12c/       # タスクスイッチ用タイマー (`timer_ts`) 導入、FIFO値2でタスクスイッチ実行
│   ├── harib12d/       # タスクBから背景シートへのアクセス方法変更 (アドレス渡し)
│   ├── harib12e/       # タスクBにカウンター表示機能追加
│   ├── harib12f/       # タスクBに1秒タイマー追加 (カウンター速度表示用)
│   └── harib12g/       # マルチタスク管理 (`mtask.c`) 導入 (`mt_init`, `mt_taskswitch`)
├── 16_day/             # マルチタスク改善 (スリープ、複数タスク、優先度、レベル)
│   ├── CMakeLists.txt
│   ├── harib13a/       # マルチタスク管理構造 (`struct TASKCTL`, `struct TASK`) と基本関数 (`task_init`, `task_alloc`, `task_run`, `task_switch`) を `mtask.c` に実装
│   ├── harib13b/       # タスクスリープ機能 (`task_sleep`) 導入
│   ├── harib13c/       # 複数タスク (task_b * 3) 生成、ウィンドウのアクティブ/非アクティブ表示
│   ├── harib13d/       # タスク優先度 (`priority`) 導入
│   └── harib13e/       # タスクレベル (`level`) 導入、レベルベーススケジューリング実装
├── 17_day/             # アイドルタスク、コンソールタスク、キー入力改善
│   ├── CMakeLists.txt
│   ├── harib14a/       # アイドルタスク (`task_idle`) 導入
│   ├── harib14b/       # コンソールタスク (`console_task`) とウィンドウ (`sht_cons`) 実装
│   ├── harib14c/       # ウィンドウタイトルバーのアクティブ/非アクティブ表示、Tabキーでの入力ウィンドウ切り替え (`key_to`)
│   ├── harib14d/       # キー入力をアクティブなウィンドウ/コンソールに送信
│   ├── harib14e/       # Shiftキー対応 (`key_shift`, `keytable1`)
│   ├── harib14f/       # CapsLock, NumLock, ScrollLock 対応 (`key_leds`, `KEYCMD_LED`, `keycmd` FIFO)
│   └── harib14g/       # (bootpack.c, mtask.c に大きな変更なし)
├── 18_day/             # コンソール機能の実装とコマンド追加
│   ├── CMakeLists.txt
│   ├── harib15a/       # コンソールタスク (`console.c`) 導入、カーソル表示
│   ├── harib15b/       # コンソールでのバックスペース処理
│   ├── harib15c/       # コンソールでのEnterキー処理 (改行)
│   ├── harib15d/       # コンソールでの画面スクロール (`cons_newline`)
│   ├── harib15e/       # `mem` コマンド実装
│   ├── harib15f/       # `cls` コマンド実装
│   └── harib15g/       # `dir` コマンド実装
├── 19_day/             # ファイルシステム、アプリケーション実行
│   ├── CMakeLists.txt
│   ├── harib16a/       # (bootpack.c, console.c に大きな変更なし)
│   ├── harib16b/       # (bootpack.c, console.c に大きな変更なし)
│   ├── harib16c/       # (bootpack.c, console.c に大きな変更なし)
│   ├── harib16d/       # `type` コマンド実装、ファイル/ウィンドウ関連関数を分離 (`file.c`, `window.c`)
│   └── harib16e/       # `hlt` コマンド実装 (外部アプリ `hlt.hrb` 実行)
├── 20_day/             # アプリケーション用API実装
│   ├── CMakeLists.txt
│   ├── harib17a/       # コンソール処理を `console.c` に分離、ファイル検索関数 `file_search` を `file.c` に追加
│   ├── harib17b/       # `cons_putchar` でタブ、改行などに対応
│   ├── harib17c/       # コマンド実行部を `cons_runcmd` に分離、アプリケーション実行関数 `cmd_app` 追加
│   ├── harib17d/       # `cmd_app` で拡張子 `.hrb` を自動補完
│   ├── harib17e/       # 文字列出力関数 `cons_putstr0`, `cons_putstr1` 追加
│   ├── harib17f/       # アプリケーション用API呼び出し関数 `hrb_api` (putchar, putstr0, putstr1) 実装
│   ├── harib17g/       # API利用アプリ `hello.nas` 追加
│   └── harib17h/       # API利用アプリ `hello2.nas` 追加、APIに終了処理追加
├── 21_day/             # アプリケーション実行環境の整備、例外処理
│   ├── CMakeLists.txt
│   ├── harib18a/       # `hrb_api` 実装 (暫定)
│   ├── harib18b/       # アプリケーション起動関数 `start_app` (スタック設定、farjmp) 導入、`hrb_api` 呼び出し規約変更
│   ├── harib18c/       # C言語アプリ `a.c`, アセンブラアプリ `a_nask.nas` 追加
│   ├── harib18d/       # アプリケーション用データセグメント設定、C言語アプリ `hello3.c` 追加
│   ├── harib18e/       # 一般保護例外 (INT 0x0d) ハンドラ実装
│   ├── harib18f/       # 例外発生テスト用アプリ `crack1.c`, `crack2.nas` 追加
│   └── harib18g/       # アプリケーション権限レベル(CPL=3)変更、`hrb_api`/例外ハンドラで `esp0` を返すように修正
├── 22_day/             # アプリケーションAPI拡張、ウィンドウ表示
│   ├── CMakeLists.txt
│   ├── harib19a/       # `hrb_api` にデバッグ用不正コード追加、テスト用アプリ `crack3-6.nas` 追加
│   ├── harib19b/       # スタック例外 (INT 0x0c) ハンドラ実装、テスト用アプリ `bug1.c` 追加
│   ├── harib19c/       # `hrb_api` 引数に `ds_base` 追加、テスト用アプリ `bug2.c`, `bug3.c` 追加
│   ├── harib19d/       # `start_app` で `eip` を直接指定するように変更、C言語アプリ `hello4.c` 追加
│   ├── harib19e/       # `.hrb` ヘッダ情報読み取り (`cmd_app` 修正)、アセンブラアプリ `hello5.nas` 追加
│   ├── harib19f/       # `hrb_api` にウィンドウ関連API (edx=5,6,7) 追加、C言語アプリ `winhelo.c` 追加
│   └── harib19g/       # `hrb_api` ウィンドウAPI改善、C言語アプリ `winhelo2.c` 追加
├── 23_day/             # アプリケーションAPI拡張 (メモリ、描画、キー入力)
│   ├── CMakeLists.txt
│   ├── harib20a/       # `hrb_api` にメモリ確保/解放API (edx=8,9,10) 追加、アプリ `star1.c` 追加
│   ├── harib20b/       # `hrb_api` に点描画API (edx=11) 追加、アプリ `stars.c` 追加
│   ├── harib20c/       # `hrb_api` にウィンドウリフレッシュAPI (edx=12) 追加、アプリ `stars2.c` 追加
│   ├── harib20d/       # `hrb_api` に線描画API (edx=13) 追加、アプリ `lines.c` 追加
│   ├── harib20e/       # `hrb_api` にウィンドウ閉じるAPI (edx=14) 追加、アプリ `winhelo3.c` 追加
│   ├── harib20f/       # `hrb_api` にキー入力API (edx=15) 追加、コンソールタイマーを構造体に格納
│   ├── harib20g/       # キー入力API (edx=15) でタイマー/カーソル処理追加、アプリ `walk.c` 追加
│   └── harib20h/       # アプリ終了時にウィンドウを自動で閉じるように修正 (`cmd_app`)
├── 24_day/             # ウィンドウ操作改善、タイマーAPI
│   ├── CMakeLists.txt
│   ├── harib21a/       # マウスクリックでウィンドウを前面に移動 (`sheet_updown`)、アクティブウィンドウのタイトル色変更 (`change_wtitle8`)
│   ├── harib21b/       # ウィンドウドラッグ移動機能実装
│   ├── harib21c/       # ウィンドウクローズボタン実装 (アプリウィンドウのみ終了可能)
│   ├── harib21d/       # コンソールとTaskAウィンドウはクローズ不可に
│   ├── harib21e/       # クリックで入力ウィンドウ切り替え (`keywin_on`, `keywin_off`)
│   ├── harib21f/       # `hrb_api` にタイマーAPI (edx=16,17,18,19) 追加、アプリ `noodle.c` 追加
│   ├── harib21g/       # アプリ終了時にタイマーを自動キャンセル (`cmd_app`)
│   └── harib21h/       # (bootpack.c, console.c, window.c, naskfunc.nas に大きな変更なし)
├── 25_day/
├── 26_day/
├── 27_day/
├── 28_day/
├── 29_day/
└── 30_day/
