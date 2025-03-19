#!/bin/bash
set -e
if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake ..
cmake --build .
if [ ! -f "graph.txt" ]; then
    cp ../graph.txt .
fi
./graph_program
