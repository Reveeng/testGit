import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3


Rectangle{
     //properties

     //alias properties
     property alias textX1: bb1.textX
     property alias textY1: bb1.textY
     property alias textX2: bb2.textX
     property alias textY2: bb2.textY

     //standart properties
     id:rightPanel
     anchors.top:parent.top
     anchors.bottom:parent.bottom
     color: "#2d2d2d"

     //signals
     signal start()
     signal setBlackBody1(int x, int y)
     signal setBlackBody2(int x, int y)
     signal hasTwoBB()
     signal hideBlackbody2()
     signal showBlackbody()
     signal saveImagePressed()
     signal settingMode(bool mode)
     signal setRefPointsSig(var point1, var point2)
     signal showAdrWind()

     //proccesing signals

    //two tab view
     Connections{
         target: snapshot
         onMacsChanged:{
                bb1.macCB.model = snapshot.macs
                bb2.macCB.model = snapshot.macs
         }
         onGetABBadress:{
              snapshot.aBBAdressWithDelay()
         }
         onHasConfig:{
            var jsonConfig = JSON.parse(config)
            bb1.textX = jsonConfig["FirstBlackBody"]["x"]
            bb1.textY = jsonConfig["FirstBlackBody"]["y"]
            bb1.temp = jsonConfig["FirstBlackBody"]["t"]
            if (jsonConfig["FirstBlackBody"]["listen"])
            {
                bb1.lstn = true
                var index1 = bb1.macCB.indexOfValue(jsonConfig["FirstBlackBody"]["mac"])
                bb1.macCB.currentIndex = index1
            }
            bb2.textX = jsonConfig["SecondBlackBody"]["x"]
            bb2.textY = jsonConfig["SecondBlackBody"]["y"]
            bb2.temp = jsonConfig["SecondBlackBody"]["t"]
            if (jsonConfig["SecondBlackBody"]["listen"])
            {
                bb2.lstn = true
                var index2 = bb2.macCB.indexOfValue(jsonConfig["SecondBlackBody"]["mac"])
                bb2.macCB.currentIndex = index2
            }
         }
     }
     StackLayout{
         width:parent.width
         height: 380
         anchors.top:bar.bottom
         anchors.topMargin: 2
         anchors.bottom:  parent.bottom
         currentIndex: bar.currentIndex
         onCurrentIndexChanged:{
            if (currentIndex == 1)
                settingMode(true)
            else
                settingMode(false)
         }
         //tab with save frame button
         Item{
             Button {
                 id: saveFrame
                 y:5
                 anchors.left:parent.left
                 anchors.right: parent.right
                 anchors.margins:5
                 height: 30
                 text: qsTr("Сохранить кадр")
                 onClicked:{
                    saveImagePressed()
                 }
             }
         }
         //tab with settings
         Item{
             MouseArea{
                 x: 0
                 y: -27
                 width: 210
                 height: 482
                 onWheel: {
                     if (wheel.angleDelta.y < 0 & palPanel.y+palPanel.height>rightPanel.height-25)
                        scroll.position = scroll.position - wheel.angleDelta.y/8000
                     if (wheel.angleDelta.y > 0 & checkbox.y < 0)
                        scroll.position = scroll.position - wheel.angleDelta.y/8000
                 }
             }

             BBItem{
                 id:bb1
                 y: 54
                 index:1
                 indColor: "red"
                 anchors.top: parent.top
                 anchors.topMargin: 5
                 Component.onCompleted: setBlackBody.connect(setBlackBody1)
                 onTempChanged:temp == "" ? snapshot.firstBB.t = 0 : snapshot.firstBB.t = parseFloat(temp, 10)
                 onLstnChanged:{
                    snapshot.firstBB.listen = lstn
                    lstn ? snapshot.firstBB.mac = curMac : snapshot.firstBB.mac = ""
                 }
            }

             BBItem{
                id:bb2
                y: 135
                indColor: "green"
                anchors.top: bb1.bottom
                anchors.topMargin: 3
                index: 2
                Component.onCompleted: setBlackBody.connect(setBlackBody2)
                onTempChanged:temp == "" ? snapshot.secondBB.t = 0 : snapshot.secondBB.t = parseFloat(temp, 10)
                onLstnChanged: {
                    snapshot.secondBB.listen = lstn
                    lstn ?  snapshot.secondBB.mac = curMac : snapshot.secondBB.mac = ""
                }
             }

            Button{
                id:setRefPoints
                text:"Установить по опорным точкам"
                anchors.left:parent.left
                anchors.right: parent.right
                anchors.top: bb2.bottom
                anchors.margins: 5
                height: 25
                onClicked: {
                    var point1 = bb1.getRefPoint()
                    var point2 = bb2.getRefPoint()
                    setRefPointsSig(point1, point2)
//                    snapshot.writeToConfig()
                }
            }
            Button{
                id:startbtn
                text: "Получить видео"
                anchors.left:parent.left
                anchors.right: parent.right
                anchors.margins: 5
                anchors.top: shutterCal.bottom
                height:25
                x:50
                y:300
                onClicked:rightPanel.start()
            }
             Button{
                id:setAddres
                anchors.left:parent.left
                anchors.right: parent.right
                anchors.top: setRefPoints.bottom
                anchors.margins: 5
                height:25
                text:"Установить адрес"
                onClicked:{
                    showAdrWind()
                }
             }
             Button{
                 id:shutterCal
                 text:"Калибровать по затвору"
                 anchors.left:parent.left
                 anchors.right: parent.right
                 anchors.top: setAddres.bottom
                 anchors.margins: 5
                 height: 25
                 onClicked: {
                    snapshot.shutterCalibr()
                 }
             }
         }
     }
     TabBar{
         id:bar
         width:parent.width
         height:fbar.height
         TabButton{
             id:fbar
             text: checked ? "<font color='#ffffff'>Наблюдение</font>" : "<font color='#000000'>Наблюдение</font>"
             focusPolicy: Qt.NoFocus
             background: Rectangle{
                 color:parent.checked ? "#808080" : "#666666"
             }
         }
         TabButton{
             text: checked ? "<font color='#ffffff'>Настройка</font>" : "<font color='#000000'>Настройка</font>"
             focusPolicy: Qt.NoFocus
             background: Rectangle{
                 color:parent.checked ? "#808080" : "#666666"
             }
         }
     }
}

