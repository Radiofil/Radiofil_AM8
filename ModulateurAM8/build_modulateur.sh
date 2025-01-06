#!/bin/bash

export PICO_SDK_FETCH_FROM_GIT=ON

rm -rf build
mkdir build
cd build
cmake ..
make
cd ..
