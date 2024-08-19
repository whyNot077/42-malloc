#!/bin/bash

set -e
mkdir -p trace
# make fclean && make bonus
./linking.sh

./tmp/my_test0 > trace/test_release_0.log \
&& echo "Test 0 successful."
./tmp/my_test1 > trace/test_release_1.log \
&& echo "Test 1 successful."
./tmp/my_test2 > trace/test_release_2.log \
&& echo "Test 2 successful."
./tmp/my_test3 > trace/test_release_3.log \
&& echo "Test 3 successful."

echo "All tests successful. log files are in trace/ directory."