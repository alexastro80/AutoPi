DIR="$( dirname "$0" )"
echo $DIR
echo "Building AASDK. . ."
chmod +x $DIR/buildAASDK.sh
$DIR/buildAASDK.sh

echo "Building openauto. . ."
chmod +x $DIR/buildOpenauto.sh
$DIR/buildOpenauto.sh

echo "Building AutoPi. . ."
chmod +x $DIR/buildAutoPi.sh
$DIR/buildAutoPi.sh
