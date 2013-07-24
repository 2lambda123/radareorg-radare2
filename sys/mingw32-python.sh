#!/bin/sh

cd `dirname $(pwd)/$0`
cd ../r2-bindings
make clean
./configure --enable-devel --prefix=/usr || exit 1
sudo make install-vapi
#make python CC=i486-mingw32-gcc CXX=i486-mingw32-g++ || exit 1
make w32 CC=i486-mingw32-gcc CXX=i486-mingw32-g++ || exit 1
make w32dist
