# 現在の状況 (Active Context)

## 現在のタスク
- CMake ビルドシステムで、内部ビルドのアセンブラ `opennask` の代わりに、外部ツール `gosk` を使用するように変更する。

## 次のステップ
- (なし)

## 持ち越し課題
- (なし)

## このセッションで完了した作業
- トップレベルの `CMakeLists.txt` を修正:
    - `find_program` を使用して外部ツール `gosk` を検索するように変更。
    - 不要になった `add_subdirectory(src)` (opennask のビルド) を削除。
    - `src` に関連するテスト設定 (`googletest`, `test`, `backward-cpp`) を削除。
- `projects/cmake_*.sh` (cmake_02day.sh, cmake_from_21day.sh, cmake_helloos.sh, cmake_until_20day.sh) を修正:
    - `opennask` の代わりにトップレベルで定義された `GOSK_EXECUTABLE` 変数を使用するように変更。
    - `gosk` のコマンド形式 (`gosk <source> <output>`) に合わせて `add_custom_target` および `add_custom_command` 内の `COMMAND` を修正。
    - `COMMAND` にソースファイルへの `DEPENDS` を追加。
- `gosk -h` を実行し、正しいコマンドライン引数を確認した。
- (以前のセッション) `projects/cmake.sh` および `projects/cmake_*.sh` 内の `tee` コマンドをファイルリダイレクト (`>` および `>>`) に置換した。
- (以前のセッション) 修正後の `projects/cmake.sh` を実行し、`projects/XX_day/CMakeLists.txt` ファイル群を更新した。
- (以前のセッション) `memory-bank/core/rules_extras.md` に `cmake.sh` の `tee` 削除に関する説明を追記した。
- (以前のセッション) `git diff` で変更内容を確認した。
