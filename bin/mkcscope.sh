#!/bin/sh
rm -rf cscope.files cscope.files
find . -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" > cscope.files
cat ./cscope.files
cscope -i cscope.files
