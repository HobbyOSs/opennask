#!/bin/bash

bnfc -m -cpp_stl nask.cf
rm -f *.bak *.cc *.hh Makefile
rename 's/\.C$/.cc/' *.C
rename 's/\.H$/.hh/' *.H
rename 'y/A-Z/a-z/' *
rm -f *.c *.h

# sed -i 's/Absyn.H/absyn.hh/g'
find . -name \*.cc -or -name \*.hh -or -name \*.l -or -name \*.y | xargs sed -i 's/"\(.\+\)\.H"/"\L\1\.hh"/'

# https://github.com/BNFC/bnfc/issues/377
sed -i '6i #include<algorithm>' absyn.hh
sed -i 's/%pure_parser/%define api.pure/g' nask.y

# entrypointの定義と実装のメソッド名が一致していない
sed -i 's/ p\(.*const\)/ ps\1/g' parser.hh

# flexでdebugログ出す設定
# sed -i 's/%option noyywrap noinput nounput/%option noyywrap noinput nounput debug/g' nask.l
# sed -i 's/return scanner;/nask_set_debug(true, scanner); return scanner;/g' nask.l

# commentのstate設定の修正
sed -i 's/<COMMENT>\".*\"/<COMMENT>\\n/g' nask.l
sed -i '/^<COMMENT>\./d' nask.l
sed -i 's/^<COMMENT>\[/<COMMENT>\[^/g' nask.l
sed -i 's/<COMMENT1>\".*\"/<COMMENT1>\\n/g' nask.l
sed -i '/^<COMMENT1>\./d' nask.l
sed -i 's/^<COMMENT1>\[/<COMMENT1>\[^/g' nask.l

# memory leakの原因のdeleteをコメントアウト
sed -i 's/delete(/\/\/delete(/g' absyn.cc


make
