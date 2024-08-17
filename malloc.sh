#!/bin/bash

set -e
mkdir -p trace
make fclean && make bonus
# make re
gcc -o tmp/test0 test/test.c -L. -lft_malloc_arm64_Darwin -lpthread \
&& gcc -o tmp/test1 test/test1.c -L. -lft_malloc_arm64_Darwin -lpthread \
&& gcc -o tmp/test2 test/test2.c -L. -lft_malloc_arm64_Darwin -lpthread \
&& gcc -o tmp/test3 test/test3.c -L. -lft_malloc_arm64_Darwin -lpthread \
&& echo "Release build successful."

./tmp/test0 > trace/test_release_0.log \
&& echo "Test 0 successful."
./tmp/test1 > trace/test_release_1.log \
&& echo "Test 1 successful."
./tmp/test2 > trace/test_release_2.log \
&& echo "Test 2 successful."
./tmp/test3 > trace/test_release_3.log \
&& echo "Test 3 successful."


# make fclean && make debug DEBUG_SHOW=1 \
# && ./tmp/test0_debug > trace/test_debug_vector.log
# && ./tmp/test1_debug > trace/test_debug_vector.log
# && ./tmp/test2_debug > trace/test_debug_vector.log
# && ./tmp/test3_debug > trace/test_debug_vector.log


# export log=1 && export log_detail=1 && make fclean && make bonus \
# && gcc -o tmp/test0 test/test.c -L. -lft_malloc_x86_64_Darwin_bonus -lpthread \
# && gcc -o tmp/test1 test/test1.c -L. -lft_malloc_x86_64_Darwin_bonus -lpthread \
# && gcc -o tmp/test2 test/test2.c -L. -lft_malloc_x86_64_Darwin_bonus -lpthread \
# && gcc -o tmp/test3 test/test3.c -L. -lft_malloc_x86_64_Darwin_bonus -lpthread \
# && echo "Release build successful." \
# && ./tmp/test0 > trace/test_release.log
