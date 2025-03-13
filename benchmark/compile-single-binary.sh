#!/bin/bash

# For Original DAFL compile 
export CC="/home/moran/Workspace/Project/DirectedFuzz/afl/OriginalDAFL/DAFL/afl-clang-fast"
export CXX="/home/moran/Workspace/Project/DirectedFuzz/afl/OriginalDAFL/DAFL/afl-clang-fast++"
export DAFL_SELECTIVE_COV="/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/StaticTest/swftophp4-7/inst-targ/2016-9829"
export DAFL_DFG_SCORE="/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/StaticTest/swftophp4-7/dfg/2016-9829"
export CFLAGS="-g -fno-omit-frame-pointer -fcommon -Wno-error -fsanitize=address"
export CXXFLAGS="-g -fno-omit-frame-pointer -fcommon -Wno-error -fsanitize=address"
./autogen.sh
./configure --disable-shared --disable-freetype 
make

# For multi-targets DAFL compile
export CC="/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/afl-clang-fast"
export CXX="/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/afl-clang-fast++"
export DAFL_SELECTIVE_COV="/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/StaticTest/swftophp4-7/inst-targ/"
export DAFL_DFG_SCORE="/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/StaticTest/swftophp4-7/dfg/"
export CFLAGS="-g -fno-omit-frame-pointer -fcommon -Wno-error -fsanitize=address"
export CXXFLAGS="-g -fno-omit-frame-pointer -fcommon -Wno-error -fsanitize=address"


./autogen.sh
./configure --disable-shared --disable-freetype 
make


# Original DAFL fuzz shell
timeout 60 /home/moran/Workspace/Project/DirectedFuzz/afl/OriginalDAFL/DAFL/afl-fuzz -m none -d -i ./input/target0/bumble-bee1.swf -o ./output/ -- ./single-instr-swftophp @@


timeout 60 /home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/afl-fuzz -m none -d -i ./input/ -o ./output/ -F 2 -- ./mul-instr-swftophp @@



# In 73, the commands should be
timeout 60 /data/sda_18tdisk/morangeous/Fuzzing/multi-dafl-code/DAFL/afl-fuzz -m none -d -i ./input/ -o ./output/ -F 2 -- ./mul-instr-swftophp @@


# Compile Command in 73
export CC="/data/sda_18tdisk/morangeous/Fuzzing/multi-dafl-code/DAFL/afl-clang-fast"
export CXX="/data/sda_18tdisk/morangeous/Fuzzing/multi-dafl-code/DAFL/afl-clang-fast++"
export DAFL_SELECTIVE_COV="/data/sda_18tdisk/morangeous/Fuzzing/multi-dafl-code/DAFL/MultiTest/StaticTest/swftophp4-7/inst-targ"
export DAFL_DFG_SCORE="/data/sda_18tdisk/morangeous/Fuzzing/multi-dafl-code/DAFL/MultiTest/StaticTest/swftophp4-7/dfg"
export CFLAGS="-g -fno-omit-frame-pointer -fcommon -Wno-error -fsanitize=address"
export CXXFLAGS="-g -fno-omit-frame-pointer -fcommon -Wno-error -fsanitize=address"