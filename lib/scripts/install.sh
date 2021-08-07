git submodule update --recursive --init

#python installation
pip install pyAutoGui
pip install pint

pip3 install pyAutoGui
pip3 install pint
pip3 install adafruit-blinka
pip3 install adafruit_mcp3xxx
pip3 install pydbus

pwd
DIR="$( dirname "$0" )"

#pip install --prefix /usr/local -e $DIR/../../python-OBD/obd
#pip3 install --prefix /usr/local -e $DIR/../../python-OBD/

sudo cp -r -v $DIR/../../python-OBD/obd /usr/local/lib/python2.7/dist-packages/
sudo cp -r -v $DIR/../../python-OBD/obd /usr/local/lib/python3.7/dist-packages/

sudo ldconfig
cd $DIR

#Ensure Bluetooth Audio will work
sudo apt -y purge "pulseaudio*"
sudo sed -i "s/ExecStart=\/usr\/lib\/bluetooth\/bluetoothd$/ExecStart=\/usr\/lib\/bluetooth\/bluetoothd --noplugin=sap/" /lib/systemd/system/bluetooth.service


dependencies=(
"alsa*"
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
"pulseaudio-utils"
"pulseaudio-module-bluetooth"
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
"python-dbus"
"python-gobject"
"python-gobject-2"
)
installString="sudo apt-get install -y "

#create apt install string
for i in ${dependencies[@]}; do
	installString+=" $i"
done
sudo apt update
echo $installString
${installString}

sudo apt-get upgrade
    
if [[ $? -eq 0 ]]; then
      echo -e AutoPi make ok, executable can be found ../bin/AutoPi
      echo

      #check and add usb rules for openauto if they dont exist
      echo Checking if permissions exist
      #udev rule to be created below, change as needed
      FILE=/etc/udev/rules.d/51-autoPiusb.rules
      if [[ ! -f "$FILE" ]]; then
          # OPEN USB RULE, CREATE MORE SECURE RULE IF REQUIRED
          echo "SUBSYSTEM==\"usb\", ATTR{idVendor}==\"*\", ATTR{idProduct}==\"*\", MODE=\"0660\", GROUP=\"plugdev\"" | sudo tee $FILE
        if [[ $? -eq 0 ]]; then
            echo -e Permissions created'\n'
          else
            echo -e Unable to create permissions'\n'
        fi
        else
          echo -e Rules exists'\n'
      fi
    else
      echo AutoPi make failed with error code $?
      exit 1
 fi


#Setting openGL driver and GPU memory to 256mb
if $isRpi; then
	sudo raspi-config nonint do_memory_split 256
	if [[ $? -eq 0 ]]; then
		echo -e Memory set to 256mb'\n'
		else
		echo Setting memory failed with error code $? please set manually
		exit 1
	fi

	sudo raspi-config nonint do_gldriver G2
	if [[ $? -eq 0 ]]; then
		echo -e OpenGL set ok'\n'
		else
		echo Setting openGL failed with error code $? please set manually
		exit 1
	fi
fi

./build/buildAll.sh
	
