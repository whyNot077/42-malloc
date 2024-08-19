#!/bin/sh 
export HOSTTYPE=Testing
# make fclean && make debug
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

# gcc -o tmp/test0_origin eval/test0.c
# gcc -o tmp/test1_origin eval/test1.c
# gcc -o tmp/test2_origin eval/test2.c
# gcc -o tmp/test3_origin eval/test3.c
# gcc -o tmp/test5_origin eval/test5.c