#!/bin/bash

mkdir $HOME/sqlite_pc
echo "......configgure..........."
./configure -prefix=$HOME/sqlite_pc 
echo "......make clean........"
make clean
echo "..........make............."
make
echo "..........make install......."
make install
