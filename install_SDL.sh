#!/bin/sh

mkdir Library
LIBPATH=$PWD/Library

git clone https://github.com/libsdl-org/SDL $LIBPATH/SDL_install
rm -rf $LIBPATH/SDL_install/.git
mkdir $LIBPATH/SDL_install/build
cd $LIBPATH/SDL_install/build
../configure --prefix=$LIBPATH
make
make install
