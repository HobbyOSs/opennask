# 現在の状況 (Active Context)

## 現在の作業焦点
- `rakusk` の移行および `opennask` のビルド環境整備の完了確認。
- `master` ブランチへの統合完了に伴う、ブランチ構成の整理と維持。
- デイリーサンプルのビルド・実行確認の継続。

## 直近の変更点
- **master への統合とブランチ整理**: 作業ブランチ `issues/99_introduce_cline_and_replace` を `master` にマージし、リモートの `master` へプッシュを完了。不要になった古い作業ブランチ (`issues/85_*` 等) を削除し、リポジトリ環境をクリーンアップした。
- **rakusk の更新**: `rakusk` 側で Shift-JIS エンコーディングの自動検知および `MOV` 命令等のパース問題が修正され、バージョン 2.1.0 がリリースされた。これを受け、環境内の `rakusk` を `zef` で更新した。
- **補助ツールの復元と統合**: `master` ブランチから `src/makefont.c`, `src/bin2obj.c`, `src/CMakeLists.txt` を復元し、プロジェクト内でビルド・使用するように `CMake` 構成を修正した。
- **objconv の CMake 統合**: `objconv` を `add_subdirectory` に追加し、ビルドディレクトリ内でビルドされるように修正した。
- **ビルドシステムの修正**: 各プロジェクトの `CMakeLists.txt` を生成するスクリプト (`projects/cmake_*.sh`) を修正し、`rakusk` の使用および最新のツールパスに対応させた。
- **全プロジェクトのビルド成功**: `ninja -C build images` により、Day 1 から Day 21 までの全サンプルプロジェクトの `.img` ファイル生成を確認した。

## 次のステップ
- **動作確認の継続**: 生成された OS イメージが QEMU で期待通りに動作するか、必要に応じて個別に検証する。
- **Day 22 以降の対応**: Day 22 から Day 30 までのビルド環境を整備する。現状、`cmake.sh` によって生成された `CMakeLists.txt` が `apilib` などのディレクトリ不足によりエラーになるため、構成の修正が必要。

## 持ち越し課題
- **Day 22-30 の CMake 構成修正**: 後半のプロジェクトで導入される共通ライブラリ（apilib）やディレクトリ構造の変化に対応した `CMakeLists.txt` の生成・整備。
