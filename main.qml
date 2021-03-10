import QtQuick 2.13
import QtQuick.Window 2.13
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Window {
    id:root
    width: 850
    height: 495
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
    WindowForAdress{
        id:windForAdr
        x:(parent.width-width)/2
        y:(parent.height-height)/2
        onAdressAccepted:{
            vpage.source = adress
        }
    }
    VideoPage{
        id:vpage
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.left: parent.left
        anchors.right:devider.left
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
    }

    Rectangle {
        id: devider
        width: 2
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.right:rightPanel.left
        color: "#595959"
    }

    function start(){
        vpage.startPlayer()
    }

    RightPanel{
        id:rightPanel
        anchors.top:parent.top
        anchors.bottom: parent.bottom
        anchors.right:parent.right
        width:208
        onSetRefPointsSig: {
            vpage.setRefPoints(point1, point2)
        }
        onStart: vpage.startPlayer()
        onSettingMode: {
            vpage.hideIndicators(mode)
        }
        onSetBlackBody1:vpage.setCoordToHighLiter(x,y,1)
        onSetBlackBody2:vpage.setCoordToHighLiter(x,y,2)
        onShowAdrWind: windForAdr.visible = true
    }
}
