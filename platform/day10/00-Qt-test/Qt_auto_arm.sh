#!/bin/bash

echo "configure......"
echo yes |./configure -silent -opensource -release -fast -embedded arm -xplatform qws/linux-arm-g++ -depths 8,16,32 -no-webkit -no-stl -no-qt3support -no-nis -no-cups -no-iconv -no-qdbus  -make libs  -nomake docs -qt-freetype  -qt-sql-sqlite -qt-kbd-tty -qt-libtiff -qt-libjpeg -qt-gif -qt-libpng -continue -silent -little-endian -no-mouse-tslib  -qt-mouse-linuxinput -prefix ../qttag

echo "make......."
make

echo "make install......"
make install