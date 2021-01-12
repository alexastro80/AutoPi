DIR="$( dirname "$0" )"
cd $DIR
cd ../../../
mkdir build
cd build || return 1
cmake -DCMAKE_BUILD_TYPE=Release -DRPI_BUILD=TRUE -DGST_BUILD ../
make
