# 現在の状況 (Active Context)

## 現在の作業焦点
- `rakusk` による `opennask` のビルド完遂に向けた課題解決。
- モダン GCC 環境での `golibc` ビルドエラーの修正。
- `rakusk` の Shift-JIS エンコーディング対応に関する課題特定。

## 直近の変更点
- **golibc の修正**: `atof.c` および `atoi.c` において、`strtod`/`strtol` 呼び出し時の不適切な型キャスト `(const char **) NULL` を削除し、`NULL` を渡すように変更。これにより GCC 14 でのビルドエラーが解消。
- **golibc のビルド成功**: `libgo.a` の生成を確認。
- **rakusk の課題特定**: 「OS自作入門」のソースコード（Shift-JIS）を読み込む際に `Malformed UTF-8` エラーが発生することを確認。

## 次のステップ
- **rakusk の改修待ち**: Shift-JIS エンコーディングへの対応を `rakusk` 開発側に依頼（要望を `memory-bank/details/rakusk_encoding_issue.md` に集約）。
- **ビルドの再開**: アセンブラ側の対応完了後、全プロジェクトのビルドおよび QEMU での動作確認を実施する。

## 持ち越し課題
- `rakusk` による `.nas` ファイル（Shift-JIS）のアセンブル。
- プロジェクト全体の `ninja_rebuild.sh` の完遂。