# 現在の状況 (Active Context)

## 現在のタスク
- (進行中) `rakusk` の成果と Memory Bank の仕組みを `opennask` へ移植・統合する (Issue #99)
- (完了) `opennask` の `.clinerules` を `rakusk` のベストプラクティスに基づき更新。
- (完了) `rakusk` の `META6.json` を修正し、`zef` でインストール可能に。変更をリモートへプッシュ済み。
- (完了) `memory-bank/archives/` ディレクトリを作成し、アーカイブ運用を導入。
- (完了) `memory-bank/core/rules_extras.md` に回帰テスト義務付けと Raku コーディング規約を追加。

## 次のステップ
- `opennask` 内で `rakusk` を `zef` を通じて利用可能にするセットアップスクリプトまたはドキュメントの作成。
- `rakusk` を使用して `opennask` のアセンブルを試行し、`gosk` との置き換え可能性を検証する。

## 持ち越し課題
- `rakusk` による `opennask` プロジェクトのビルド統合（CMakeLists.txt への反映など）。

## このセッションで完了した作業
- `opennask` で `issues/99_introduce_cline_and_replace` ブランチへ切り替え。
- `rakusk` の `.clinerules` から絶対パス設定、回帰テストルール、一括読み込み設定を `opennask` に移植。
- `rakusk` の `META6.json` に `provides` セクションを全モジュール分追加し、`zef install` に対応。リモート (`main` ブランチ) へプッシュ。
- `opennask` に `memory-bank/archives/` を作成。
- `rules_extras.md` を更新し、テストと品質管理、Raku 規約のセクションを追加。
