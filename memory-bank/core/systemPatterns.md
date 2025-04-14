# System Patterns

## システムアーキテクチャ
- CMake を利用した階層的なビルドシステムを採用しています。
- `projects/` ディレクトリ内の各サンプル（日ごとのディレクトリ `XX_day` とその下の `haribXXX` や `helloosX`）が個別のサブプロジェクトとして構成されます。

## 主要な技術的決定
- **CMakeLists.txt の自動生成**: `projects/cmake.sh` スクリプトが、`projects/` ディレクトリ内の `.nas` ファイルを探索し、その結果に基づいて階層的な `CMakeLists.txt` ファイル群を自動生成します。
    - トップレベルの `CMakeLists.txt` (`projects/CMakeLists.txt`) には、各 `XX_day` ディレクトリを追加するための `add_subdirectory()` が記述されるべきことを示唆します (スクリプト自体は直接編集せず、指示を出力します)。
    - 各 `XX_day` ディレクトリ内の `CMakeLists.txt` は、`cmake.sh` によって生成され、さらにその下のサンプルディレクトリ (`haribXXX` など) を `add_subdirectory()` で追加します。
- **詳細なビルドルールの分離と条件分岐**: 個々の `.nas` ファイルをアセンブルし、リンクして実行可能イメージを作成するための具体的な CMake コマンド (`add_custom_command`, `add_custom_target` など) は、`cmake.sh` から呼び出される専用のシェルスクリプト (`cmake_helloos.sh`, `cmake_02day.sh`, `cmake_until_20day.sh`, `cmake_from_21day.sh`) によって、各サンプルディレクトリの `CMakeLists.txt` に生成されます。
    - 特に `cmake_until_20day.sh` は、サンプルディレクトリ内のファイル構成（例: `ipl.nas` のみ存在するか、`ipl10.nas`, `asmhead.nas`, `*.c` が全て存在するか）をチェックし、その構成に応じた適切なビルドルール（IPL の直接実行ターゲット、またはフル OS イメージ生成ターゲット）を生成するロジックを持っています。

## 標準実装パターン
- **CMake の階層構造**: `add_subdirectory()` を利用して、プロジェクト全体をトップレベルと日ごとのサブディレクトリ、さらに個別のサンプルディレクトリという階層で管理します。
- **シェルスクリプトによる自動生成**: `find` と `sed`、ファイル存在チェック (`[ -e ... ]`, `ls`) を駆使してファイル構造を解析し、定型的な CMakeLists.txt を生成するパターンを採用しています。これにより、新しいサンプルディレクトリを追加する際の手間を軽減しています。
- **責務分離**: 全体の構造定義 (`cmake.sh`) と、個別のビルドルール生成 (専用の `cmake_*.sh` スクリプト群) を分離しています。
- **条件付きビルドルール生成**: `cmake_until_20day.sh` において、サンプルディレクトリの内容に応じて生成する CMake ターゲットを切り替えるパターンを採用しています。

## 使用している設計パターン
- （現時点では特筆すべき設計パターンは見当たりません。ビルドスクリプトの自動生成が特徴的です。）

## プロジェクトのディレクトリ構成
```
opennask/
├── .clinerules           # Cline のプロジェクト固有ルール
├── .gitignore            # Git の無視ファイル設定
├── CMakeLists.txt        # プロジェクト全体の CMake 設定 (トップレベル)
├── README.md             # プロジェクトの説明
├── golibc/               # OS 開発用の簡易 C ライブラリ
│   ├── *.c               # C ソースファイル
│   ├── *.h               # ヘッダファイル
│   └── CMakeLists.txt    # golibc の CMake 設定
├── memory-bank/          # Cline の記憶用ディレクトリ
│   ├── core/             # プロジェクトの中核情報
│   ├── details/          # 詳細な実装情報
│   ├── archives/         # 過去の記録
│   └── docs/             # プロジェクト文書
├── objconv/              # オブジェクトファイルコンバータ (外部ツール)
│   └── ...
├── projects/             # 「30日でできる！OS自作入門」のサンプルプロジェクト群
│   ├── XX_day/           # 日ごとのサンプルディレクトリ (例: 01_day)
│   │   ├── haribXXX/     # 個別のサンプル (例: harib00a)
│   │   │   ├── *.nas     # アセンブリソース
│   │   │   ├── *.c       # C ソース (一部)
│   │   │   └── CMakeLists.txt # 個別ビルドルール (自動生成)
│   │   └── CMakeLists.txt  # 日ごとの CMake 設定 (自動生成)
│   ├── CMakeLists.txt    # projects ディレクトリの CMake 設定 (手動編集が必要)
│   ├── cmake.sh          # CMakeLists.txt 自動生成スクリプト (メイン)
│   └── cmake_*.sh        # 詳細ビルドルール生成スクリプト
└── ...                   # その他の設定ファイル、スクリプトなど
```

## コンポーネント間の関係
- `cmake.sh` は、ファイルシステム (`projects/` 内の `.nas` ファイルの存在) を元に、CMake のビルド構造 (ディレクトリ階層) を定義する `CMakeLists.txt` を生成します。
- `cmake.sh` は、詳細なビルドコマンドの生成を、専用の `cmake_*.sh` スクリプト群に委譲します。
- 各 `cmake_*.sh` スクリプトは、対応するサンプルディレクトリ内の `CMakeLists.txt` に、アセンブル、コンパイル、リンクのための具体的な `add_custom_command` や `add_custom_target` を書き込みます。
- 最終的に、`cmake` コマンドがこれらの `CMakeLists.txt` を解釈し、Ninja や Make などのビルドシステム用のファイルを生成します。
