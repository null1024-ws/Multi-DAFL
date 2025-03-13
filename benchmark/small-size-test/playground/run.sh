#!/bin/bash

echo "deleting the output dir"
rm -rf ./output/

echo "delete complete, starting fuzzing"
AFL_NO_UI=1 /home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/afl-fuzz -m none -d -i ./input/ -o ./output/ -F 3 -- ./small-test
