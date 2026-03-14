#!/bin/bash

mkdir -p bin
g++ src/*.cpp src/solvers/*.cpp src/generators/*.cpp -o bin/maze-toolkit-v2
