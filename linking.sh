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
echo "build eval successful."

gcc eval/test0.c -o tmp/test0_origin
gcc eval/test1.c -o tmp/test1_origin
gcc eval/test2.c -o tmp/test2_origin

# gcc -L. -lft_malloc test/test.c -o tmp/my_test0
# gcc -L. -lft_malloc test/test1.c -o tmp/my_test1
# gcc -L. -lft_malloc test/test2.c -o tmp/my_test2
# gcc -L. -lft_malloc test/test3.c -o tmp/my_test3
# echo "build test successful."
