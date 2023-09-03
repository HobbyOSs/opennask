#!/bin/bash

# すべてのnaskファイルをパースできるかテストする
find ../../projects/ -type f -name '*.nas' -print0 | sort -z | xargs -0 -I {} sh -c './testnask {} > /dev/null 2>&1 && echo -e "\e[0;32mパース成功: {}\e[0m\e[m" || echo -e "\e[0;31mパース失敗: {}\e[0m\e[m"'
