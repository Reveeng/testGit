import QtQuick 2.13
import QtQuick.Window 2.13
import QtMultimedia 5.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id:root
    width: 850
    height: 495
    visible: true
    color: "#2d2d2d"
    title: qsTr("Получение сырых кадров")
//    flags: /*Qt.FramelessWindowHint | */  Qt.CustomizeWindowHint | Qt.WindowSystemMenuHint | Qt.WindowCloseButtonHint
//    Component.onCompleted: {
//        snapshot.allBlackBodyAdress()
//    }
    onVisibilityChanged:{
        if (visibility == 4){
            vpage.state = "fullscreen"
            snapshot.fullscreen = true
        }
        if (visibility == 2){
            vpage.state = "default"
            snapshot.fullscreen = false
        }
    }
    onClosing: {
        snapshot.writeToConfig()
    }
    onHeightChanged:snapshot.windHeight = height
    onWidthChanged: snapshot.windWidth = width

    Connections{
        target:snapshot
        onHasConfig:{
            var jsonConfig = JSON.parse(config)
            snapshot.address = jsonConfig["device"]["address"]
            snapshot.allBlackBodyAdress()
            if (jsonConfig["window"]["fullscreen"])
                root.showMaximized()
            else{
                if (Screen.desktopAvailableHeight > jsonConfig["window"]["height"])
                    root.height = jsonConfig["window"]["height"]
                if (Screen.desktopAvailableWidth > jsonConfig["window"]["width"])
                    root.width = jsonConfig["window"]["width"]
//                console.log(Screen.desktopAvailableHeight, Screen.desktopAvailableWidth)
            }
            vpage.parseConfig(jsonConfig)
            rightPanel.parseConfig(jsonConfig)
        }
    }
    WindowForAdress{
        id:windForAdr
        x:(parent.width-width)/2
        y:(parent.height-height)/2
        onAdressAccepted:{
            vpage.setSource(adress)
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
        onMtChanged:rightPanel.maxT = maxTemp
    }

    Rectangle {
        id: devider
        width: 2
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.right:rightPanel.left
        color: "#595959"
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
