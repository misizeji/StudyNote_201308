#!/bin/bash

echo "configure......."
./configure --prefix=/usr/local/libxml

echo "make......"
make

echo "make install......"
make install

