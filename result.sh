#!/bin/sh 

./linking.sh
/usr/bin/time -l ./tmp/test0 > trace/0
/usr/bin/time -l ./tmp/test1 > trace/1
# /usr/bin/time -l ./tmp/test2 > trace/2
# /usr/bin/time -l ./tmp/test3 > trace/3
# /usr/bin/time -l ./tmp/test4 > trace/4
# /usr/bin/time -l ./tmp/test5 > trace/5

# /usr/bin/time -l ./tmp/test0_origin
# /usr/bin/time -l ./tmp/test1_origin
# /usr/bin/time -l ./tmp/test2_origin
