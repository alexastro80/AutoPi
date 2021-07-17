DIR="$( dirname "$0" )"
echo "$DIR"
sudo rm -v /lib/systemd/system/joystick.service
sleep 1
sudo systemctl daemon-reload
sleep 1

python $DIR/joystickService.py
sudo cp -v $DIR/joystick.service /lib/systemd/system/joystick.service
sleep 1
sudo chmod 644 /lib/systemd/system/joystick.service
sleep 1
sudo systemctl daemon-reload
sudo systemctl enable joystick.service
$DIR/../Input/input.sh