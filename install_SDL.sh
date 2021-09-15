#!/bin/sh

LIBPATH=$PWD/Library

cd $LIBPATH
rm -rf bin include lib SDL2-2.0.16 share
tar -xf SDL2-2.0.16.tar.gz
mkdir SDL2-2.0.16/build
cd SDL2-2.0.16/build
../configure --prefix=$LIBPATH
make
make install
cd $LIBPATH/lib
rm -rf libSDL2.so