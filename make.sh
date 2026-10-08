#!/bin/sh

set -eu

rm -rf build
mkdir build
cd build
cmake ../
make -j
