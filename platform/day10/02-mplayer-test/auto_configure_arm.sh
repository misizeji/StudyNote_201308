#!/bin/bash

echo "configure......."
./configure --host-cc=gcc --cc=arm-linux-gcc --target=armv4l --enable-static  --disable-win32dll  --disable-dvdread --disable-dvdread-internal --disable-dvdnav --disable-libdvdcss-internal --enable-fbdev --disable-mencoder --disable-live --disable-mp3lib --enable-mad --enable-libavcodec_a --language=zh_CN
