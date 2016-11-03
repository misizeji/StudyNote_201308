#!/bin/bash

echo "configure......"
./configure --host=arm-linux --without-gtk --without-carbon  --without-quicktime --without-1394libs --without-ffmpeg --without-python --without-swig --disable-static --enable-shared --disable-apps CXX=arm-linux-g++ CPPFLAGS="-I$OPENCV_BUILD_DIR/install/include -I/home/edu/share/unsp210_linux_2.6.35/include" --prefix=$OPENCV_BUILD_DIR/install

echo "make......"
make

echo "make install......"
make install



