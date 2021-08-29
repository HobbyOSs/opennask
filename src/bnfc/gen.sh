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

# flexでdebugログ出す設定
sed -i 's/%option noyywrap noinput nounput/%option noyywrap noinput nounput debug/g' nask.l
sed -i 's/return scanner;/nask_set_debug(true, scanner); return scanner;/g' nask.l

# comment要素追加
sed -i 's/%START CHAR CHARESC CHAREND STRING ESCAPED/%START CHAR CHARESC CHAREND STRING ESCAPED COMMENT/g' nask.l

# commentのstate設定
# BEFORE:
# <INITIAL>"#"[^\n]* /* skip */; /* BNFC: comment "#" */
# <INITIAL>";"[^\n]* /* skip */; /* BNFC: comment ";" */
# AFTER:
# <INITIAL>"#"[.] /* skip */; /* BNFC: comment "#" */
# <INITIAL>";"[.] /* skip */; /* BNFC: comment ";" */
sed -i 's/\/\* skip \*\/;/\/\* skip \*\/ BEGIN COMMENT;/g' nask.l

#sed -i "/<INITIAL>\";\"/an'<COMMENT>\n		BEGIN INITIAL;'" < nask.l
#sed -i "/<INITIAL>\";\"/an'<COMMENT>[^\n]+                     ;'" < nask.l

sed -i "s/\(<INITIAL>\";\".*\)/\1\n<COMMENT>\\\\n		BEGIN INITIAL;\n<COMMENT>\[^\\\\n\]+                     ;\n/g" nask.l

sed -i 's/<INITIAL>\[ \\t\\r\\n\\f\]/<INITIAL>\[ \\t\\r\\f\]/g' nask.l

make
