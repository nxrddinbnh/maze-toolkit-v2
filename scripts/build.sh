#!/bin/bash

cd build
g++ -std=c++17 -I../include ../src/main.cpp ../src/solve/*.cpp ../src/generators/*.cpp -o maze-toolkit-v2
mv maze-toolkit-v2 ../bin/
cd ..
