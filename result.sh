#!/bin/sh 

# gcc -o ./tmp/test0 eval/test0.c && /usr/bin/time -l ./tmp/test0 \
# && gcc -o ./tmp/test1 eval/test1.c && /usr/bin/time -l ./tmp/test1

./linking.sh
# /usr/bin/time -l ./tmp/test0
/usr/bin/time -l ./tmp/test1
/usr/bin/time -l ./tmp/test2

# /usr/bin/time -l ./tmp/test0_origin
# /usr/bin/time -l ./tmp/test1_origin
