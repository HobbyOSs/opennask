#!/bin/bash

SCRIPT_DIR=$(cd $(dirname $0); pwd)
BUILD_DIR="${SCRIPT_DIR}/build"

# build ディレクトリから *_od ターゲットを抽出
targets_from_build=""
if [ -d "${BUILD_DIR}" ]; then
    # ninja -t targets の出力から '_od: ' を含む行を抽出し、':' より前の部分を取得
    targets_from_build=$(cd "${BUILD_DIR}" && ninja -t targets 2>/dev/null | grep '_od: ' | awk -F': ' '{print $1}' | sort -u)
fi

# ターゲットが存在しない場合はエラー終了
if [ -z "$targets_from_build" ]; then
    echo "build ディレクトリから実行可能な ninja ターゲット (*_od) が見つかりません。"
    echo "ninja_rebuild.sh を実行してビルド環境を再構築してください。"
    exit 1
fi

# fzf でターゲットを選択させる
selected_target=$(echo "$targets_from_build" | fzf --height 40% --reverse --prompt="比較するターゲットを選択: " --header="↑↓で選択, Enterで実行, Ctrl-Cで終了")

# fzf で何も選択されなかった場合 (ESCキーやCtrl-Cなど) は終了
if [ -z "$selected_target" ]; then
    echo "ターゲットが選択されませんでした。終了します。"
    exit 0
fi

# 選択されたターゲットを実行
echo "ターゲット '$selected_target' を実行します..."
if (cd "${BUILD_DIR}" && ninja "$selected_target"); then
    echo "ターゲット '$selected_target' の実行が完了しました。"
else
    echo "ターゲット '$selected_target' の実行中にエラーが発生しました。"
    exit 1 # エラーが発生したら終了する
fi

exit 0
