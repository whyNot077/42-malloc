#!/bin/sh 
ln -s libft_malloc_Testing.so libft_malloc.so
ls -l libft_malloc.so
gcc -o test0 test0.c && /usr/bin/time -l ./test0
export DYLD_LIBRARY_PATH=.
export DYLD_INSERT_LIBRARIES="libft_malloc.so"
export DYLD_FORCE_FLAT_NAMESPACE=1
$@

# export DYLD_LIBRARY_PATH=. 
# export DYLD_INSERT_LIBRARIES="libft_malloc.so"
# export DYLD_FORCE_FLAT_NAMESPACE=1
# $@