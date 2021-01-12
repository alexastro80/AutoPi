#!/bin/sh
git submodule update --recursive --init
pip install pyAutoGui
#PYTHON_DIR=""
# Copy python OBD
#cp -r ./python-OBD/obd $PYTHON_DIR 

cp -r -v ./openauto/cmake_modules ./


###########aasdk#####################
#cd aasdk
#mkdir build
#cd build
#cmake -DCMAKE_BUILD_TYPE=Release ../
#make -j2
#sudo make install
#cd ..
#cd ..

##########openauto##################
#cd openauto
#mkdir build
#cd build
#cmake -DGST_BUILD=true ../
#make
#sudo make install
#cd ..
#cd ..


mkdir build
cd build || return 1
cmake -DCMAKE_BUILD_TYPE=Release -DRPI_BUILD=TRUE -DGST_BUILD ../
make

