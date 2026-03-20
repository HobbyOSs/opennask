# Progress

## 実装済み
- **rakusk の導入**: `zef` によるインストール、CMake からの検出。
- **CMake 環境整備**: 3.10 以上への要求バージョン引き上げ、全 `CMakeLists.txt` の修正。
- **golibc の GCC 14 対応**: `atof.c` および `atoi.c` の型不一致エラーの修正と `libgo.a` のビルド成功。
- **アセンブラの置き換え (gosk → rakusk)**: 各プロジェクトのビルドルールの修正。

## まだ必要な実装 / 既知の課題
- **rakusk の Shift-JIS 対応**: 現状では UTF-8 前提のため、既存の `.nas` ファイル（Shift-JIS）の読み込みに失敗する（[詳細](../details/rakusk_encoding_issue.md)）。
- **ビルドの完遂**: アセンブラ側の対応完了後、全サンプルの `.img` 生成を確認。
- **動作検証**: QEMU での OS 起動確認。

## 解決済み
- (以前の記録) `gosk` に関するビルドルールの修正や依存関係の修正。