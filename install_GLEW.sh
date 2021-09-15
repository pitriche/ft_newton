#!/bin/sh

cd GLEW
rm -rf glew-2.2.0 include lib lib64
tar -xf glew-2.2.0.tgz
cd glew-2.2.0
make
make install GLEW_DEST=$PWD/..
