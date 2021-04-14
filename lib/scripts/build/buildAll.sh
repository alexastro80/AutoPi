DIR="$( dirname "$0" )"
echo $DIR

echo "Building GSTREAMER. . ."
chmod +x $DIR/buildGStreamer.sh
$DIR/buildGStreamer.sh

echo "Building AASDK. . ."
chmod +x $DIR/buildAASDK.sh
$DIR/buildAASDK.sh

echo "Building openauto. . ."
chmod +x $DIR/buildOpenauto.sh
$DIR/buildOpenauto.sh

echo "Building resources. . ."
sudo python $DIR/resources.py

echo "Building AutoPi. . ."
chmod +x $DIR/buildAutoPi.sh
$DIR/buildAutoPi.sh
