#!/bin/bash

CF_FILE=$HOME/git/opennask/src/bnfc/nask.cf
BNFC_OPT="--cpp -l"
OUTPUT_DIR=$HOME/git/opennask/src/bnfc/

rm -f *.bak *.cc *.hh Makefile
cd ~/git/bnfc/ && cabal run bnfc -- -mMakefile $BNFC_OPT $CF_FILE --outputdir=$OUTPUT_DIR
cd -

rename 'y/A-Z/a-z/' *
sed -e 's/Test/test/g' -e 's/Absyn/absyn/g' \
    -e 's/Buffer/buffer/g' -e 's/Parser/parser/g' \
    -e 's/Driver/driver/g' -e 's/Printer/printer/g' \
    -e 's/Scanner/scanner/g' -e 's/Lexer/lexer/g' \
    -e 's/Skeleton/skeleton/g'  -i makefile

find . -name \*.cc -or -name \*.hh -or -name \*.ll -or -name \*.yy | xargs sed -i 's/"\(.\+\)\.hh"/"\L\1\.hh"/'

# flexでdebugログ出す設定
# sed -i 's/%option noyywrap noinput nounput/%option noyywrap noinput nounput debug/g' nask.l
# sed -i 's/return scanner;/nask_set_debug(true, scanner); return scanner;/g' nask.l

# commentのstate設定の修正
# sed -i 's/<COMMENT>\".*\"/<COMMENT>\\n/g' nask.l
# sed -i '/^<COMMENT>\./d' nask.l
# sed -i 's/^<COMMENT>\[/<COMMENT>\[^/g' nask.l
# sed -i 's/<COMMENT1>\".*\"/<COMMENT1>\\n/g' nask.l
# sed -i '/^<COMMENT1>\./d' nask.l
# sed -i 's/^<COMMENT1>\[/<COMMENT1>\[^/g' nask.l

make
