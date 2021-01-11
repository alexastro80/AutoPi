DIR="$( dirname "$0" )"
cd $DIR
cd ../../../aasdk/
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j2
sudo make install
