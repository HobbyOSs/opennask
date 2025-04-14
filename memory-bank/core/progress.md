# Progress

## 実装済み
- CMake ビルドシステムを外部ツール `gosk` を使用するように変更。
- `projects/cmake_*.sh` スクリプトを `gosk` に対応するように修正。
- `projects/cmake_*.sh` の依存関係を修正 (`asmhead.bin` 生成)。
- `projects/cmake_until_20day.sh` を修正し、サンプルディレクトリ内のファイル構成 (`ipl.nas` のみ、`ipl10.nas` + `asmhead.nas` + `*.c` など) に応じて適切な CMake ビルドルールを生成するように変更。

## まだ必要な実装
- (なし)

## 関連情報 / 既知の問題
- (解決済み) **CMake ビルドエラー (03_day):** `projects/cmake_until_20day.sh` の修正により、以下の問題が解決されました。
    - `03_day/harib00a` (IPLのみ) で `os.img` が見つからないエラー。
    - `03_day/harib00i` (フルOSビルド) で `_od` ターゲットの依存関係 (`_asmhead`, `_ipl10`) が見つからないエラー。
    - `03_day/harib00a` (IPLのみ) で QEMU 実行コマンドの `-fda` オプションが重複するエラー。
- (解決済み) **`gosk` のアセンブル問題:** 以前のバージョン (`v0.0.0-20250406052136-a15b20e3eece`) では `projects/03_day/harib00i/asmhead.nas` のアセンブルに失敗していましたが、最新版 (`v0.0.0-20250408132134-d5f8380224fc`) で修正されました。
