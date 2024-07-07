#!/bin/bash

emcc src/generate-maze.cpp \
     src/main.cpp \
     src/solve-maze.cpp \
     src/tools.cpp \
     src/generators/binary-tree.cpp \
     src/generators/depth-first-search.cpp \
     src/generators/kruskal.cpp \
     src/generators/recursive-division.cpp \
     src/generators/side-winder.cpp \
     src/generators/wilson.cpp \
     src/solvers/a-start.cpp \
     src/solvers/bidirectional-bfs.cpp \
     src/solvers/breadth-first.cpp \
     -o bin/maze-toolkit.js \
     -O3 -s NO_EXIT_RUNTIME=1 \
     -s "EXPORTED_RUNTIME_METHODS=['ccall']" \
     -s "EXPORTED_FUNCTIONS=['_malloc', '_free']"

# exec "$SHELL"