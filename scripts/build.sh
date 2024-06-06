#!/bin/bash

cd bin
g++ ../src/*.cpp  ../src/solve/*.cpp ../src/generators/*.cpp -o maze-toolkit-v2
mv maze-toolkit-v2 ../bin/
cd ..
