import QtQuick 2.13
import QtQuick.Window 2.13
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Window {
    id:root
    width: 850
    height: 480
    visible: true
    color: "#2d2d2d"
    title: qsTr("Hello World")
//    flags: /*Qt.FramelessWindowHint | */  Qt.CustomizeWindowHint | Qt.WindowSystemMenuHint | Qt.WindowCloseButtonHint

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
        anchors.right:devider.left
        source: "rtspsrc location=rtsp://192.168.0.115/rawdata latency=0 ! "+
        "rtpvrawdepay ! queue ! "+
        "appsink max-buffers=3 drop=true emit-signals=true name=sink0";
        autoplay:true
        onBbChangedByMouse: {
            if (number == 1){
                rightPanel.textX1 = x
                rightPanel.textY1 = y
            }
            else{
                rightPanel.textX2 = x
                rightPanel.textY2 = y
            }
        }
        Component.onCompleted: {
            bbChangedByMouse.connect()
        }
    }

    Rectangle {
        id: devider
        width: 2
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.right:rightPanel.left
        color: "#808080"
    }

//    Button{
//        id:btn
//        anchors.right: parent.right
//        anchors.top:parent.top
//        height: 45
//        width: 60
//        onClicked: {
//            start()
//        }
//    }

    function start(){
        vpage.startPlayer()
    }

    RightPanel{
        id:rightPanel
        anchors.top:parent.top
        anchors.bottom: parent.bottom
        anchors.right:parent.right
        width:208
        onSetRefPoints: {
            vpage.setRefPoints(point1, point2)
        }
        onStart: vpage.startPlayer()
//        onStart: root.start()
    }
}
