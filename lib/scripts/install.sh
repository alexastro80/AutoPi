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
	
