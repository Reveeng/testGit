import QtQuick 2.13
import QtQuick.Window 2.13
import GstVideoPlayer 0.1
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Window {
    width: 1000
    height: 1000
    visible: true
    title: qsTr("Hello World")
    GstVideoPlayer{
        id:myplayer
        source: "rtspsrc location=rtsp://192.168.0.115/rawdata latency=0 ! "+
        "rtpvrawdepay ! queue ! "+
        "appsink max-buffers=3 drop=true emit-signals=true name=sink0";
        onErrChanged: {
            console.log(err)
        }
    }
    VideoOutput{
        id:output
        x:0
        y:0
        width:640
        height:480
        source: myplayer
        anchors.fill: parent
    }
    Button{
        x: 650
        y:0
        width:50
        height:50
        onClicked: myplayer.start()
    }
}
