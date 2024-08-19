#!/bin/sh 
export HOSTTYPE=Testing
make re
ln -s libft_malloc_Testing.so libft_malloc.so
ls -l libft_malloc.so

./run_mac.sh 
gcc -L. -lft_malloc eval/test0.c -o tmp/test0
gcc -L. -lft_malloc eval/test1.c -o tmp/test1
gcc -L. -lft_malloc eval/test2.c -o tmp/test2
gcc -L. -lft_malloc eval/test3.c -o tmp/test3
gcc -L. -lft_malloc eval/test4.c -o tmp/test4
gcc -L. -lft_malloc eval/test5.c -o tmp/test5

# gcc -o test0_origin test0.c
# gcc -o test1_origin test1.c
# gcc -o test2_origin test2.c
# gcc -o test3_origin test3.c
# gcc -o test5_origin test5.c