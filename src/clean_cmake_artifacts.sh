#!/bin/bash

files=( 
    *.cmake
    CMakeCache.txt
    Makefile
    CMakeFiles
    include/CMakeFiles
    source/CMakeFiles
    source/*.cmake
    source/Makefile
)

for i in "${files[@]}"
do
   rm -rf $i;
done