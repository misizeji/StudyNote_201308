#!/bin/bash

echo "configure......"
./configure  --host=arm-linux  --build=i386-linux  --target=arm  --prefix=/usr/local/libxml_arm CC=/usr/local/arm/4.3.2/bin/arm-linux-gcc  --without-zlib

echo "make......"
make

echo "make install......"
make install
