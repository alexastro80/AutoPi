git submodule update --recursive --init

#python installation
pip install pyAutoGui
pip install pint
pip3 install pyAutoGui
pip3 install pint
pwd
DIR="$( dirname "$0" )"

sudo pip install --prefix /usr/local -e $DIR/../../python-OBD/
sudo pip3 install --prefix /usr/local -e $DIR/../../python-OBD/


#run cmake
echo Beginning cmake
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib/$(dpkg-architecture -qDEB_HOST_MULTIARCH) -DCMAKE_INSTALL_INCLUDEDIR=include -DQT_VERSION=5 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-std=c++11

if [[ $? -eq 0 ]]; then
echo -e Make ok'\n'
else
echo Gstreamer CMake failed
exit 1
fi

echo Making Gstreamer
make -j4

if [[ $? -eq 0 ]]; then
echo -e Gstreamer make ok'\n'
else
echo Make failed with error code $?
exit 1
fi

#run make install
echo Beginning make install
sudo make install

if [[ $? -eq 0 ]]; then
echo -e Gstreamer installed ok'\n'
else
echo Gstreamer make install failed with error code $?
exit 1
fi

sudo ldconfig
cd ../../AutoPi/lib/scripts/

dependencies=(
"alsa-utils"
"cmake"
"libboost-all-dev"
"libusb-1.0.0-dev"
"libssl-dev"
"libprotobuf-dev"
"protobuf-c-compiler"
"protobuf-compiler"
"libqt5multimedia5"
"libqt5multimedia5-plugins"
"libqt5multimediawidgets5"
"qtmultimedia5-dev"
"qtdeclarative5-dev"
"libqt5bluetooth5"
"libqt5bluetooth5-bin"
"qtconnectivity5-dev"
"pulseaudio"
"librtaudio-dev"
"librtaudio6"
"libkf5bluezqt-dev"
"libtag1-dev"
"qml-module-qtquick2"
"libglib2.0-dev"
"libgstreamer1.0-dev"
"gstreamer1.0-plugins-base-apps"
"gstreamer1.0-plugins-bad"
"gstreamer1.0-libav"
"gstreamer1.0-alsa"
"libgstreamer-plugins-base1.0-dev"
"qtdeclarative5-dev"
"qt5-default"
"libgstreamer-plugins-bad1.0-dev"
"libunwind-dev"
"qml-module-qtmultimedia"
"libqt5serialbus5-dev"
"libqt5serialbus5-plugins"
"libqt5serialport5-dev"
"libqt5websockets5-dev"

)
installString="sudo apt install -y "

#create apt install string
for i in ${dependencies[@]}; do
	installString+=" $i"
done
sudo apt update
${installString}

sudo apt-get upgrade
    


./build/buildAll.sh
	
