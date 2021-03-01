import QtQuick 2.13
import QtQuick.Window 2.13
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Window {
    id:root
    width: 700
    height: 480
    visible: true
    color: "#2d2d2d"
    title: qsTr("Hello World")

    onVisibilityChanged:{
        if (visibility == 4){
            vpage.state = "fullscreen"
        }
        if (visibility == 2){
            vpage.state = "default"
        }
    }

    VideoPage{
        id:vpage
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.left: parent.left
        anchors.right:btn.left
        width: 640
        height: 480
        source: "rtspsrc location=rtsp://192.168.0.115/rawdata latency=0 ! "+
        "rtpvrawdepay ! queue ! "+
        "appsink max-buffers=3 drop=true emit-signals=true name=sink0";
        autoplay:true
        Component.onCompleted: {
            onAutoplayChanged.connect(start)
        }
    }
    Button{
        id:btn
        anchors.right: parent.right
        anchors.top:parent.top
        height: 45
        width: 60
        onClicked: {
            vpage.vis = true
        }
    }

    function start(){
        vpage.startPlayer()
    }
//    RightPanel{
//        id:rightPanel
//        anchors.top:parent.top
//        anchors.bottom: parent.bottom
//        anchors.right:parent.right
//        width:208
//    }
}
