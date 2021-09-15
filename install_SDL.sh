#!/bin/sh

LIBPATH=$PWD/Library

cd $LIBPATH
rm -rf bin include lib SDL share
tar -xf SDL.tgz
mkdir SDL/build
cd SDL/build
../configure --prefix=$LIBPATH
make
make install
