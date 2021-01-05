#!/bin/sh
git submodule update --recursive --init
pip install pyAutoGui
#PYTHON_DIR=""
# Copy python OBD
#cp -r ./python-OBD/obd $PYTHON_DIR 

cp -r -v ./openauto/cmake_modules ./

mkdir build
cd build || return 1
cmake -DCMAKE_BUILD_TYPE=Release -DRPI_BUILD=TRUE ../
make

