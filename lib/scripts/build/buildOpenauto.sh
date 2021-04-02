##########openauto##################
DIR="$( dirname "$0" )"
cd $DIR
cd ../../../openauto
pwd
mkdir build
cd build
cmake -DGST_BUILD=true ../
make
sudo make install


