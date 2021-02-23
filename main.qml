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
        source: "playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm";

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
    }
    Button{
        x: 650
        y:0
        width:50
        height:50
        onClicked: myplayer.start()
    }
}
