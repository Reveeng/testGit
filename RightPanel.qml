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
     property alias maxT:mt.maxTemp

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

         //tab with settings
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
         Item{


             Text{
                 id:twoBB
                 x: 548
                 y: 0
                text:"2 АЧТ"
                color:"white"
                font.pixelSize: 10
                anchors.verticalCenter: chB.verticalCenter
                anchors.right: chB.left
                anchors.leftMargin: 3
             }

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
                 anchors.top: chB.bottom
                 anchors.topMargin:3
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
                visible: false
                index: 2
                Component.onCompleted: setBlackBody.connect(setBlackBody2)
                onTempChanged:temp == "" ? snapshot.secondBB.t = 0 : snapshot.secondBB.t = parseFloat(temp, 10)
                onLstnChanged: {
                    snapshot.secondBB.listen = lstn
                    lstn ?  snapshot.secondBB.mac = curMac : snapshot.secondBB.mac = ""
                }
             }

             MaxTemp{
                id:mt
                anchors.top: bb1.bottom
                anchors.topMargin: 2

             }

            Button{
                id:setRefPoints
                text:"Установить по опорным точкам"
                anchors.left:parent.left
                anchors.right: parent.right
                anchors.top: mt.bottom
                anchors.margins: 5
                height: 25
                onClicked: {
                    var point1 = bb1.getRefPoint()
                    var point2 = bb2.getRefPoint()
                    setRefPointsSig(point1, point2)
                }
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
             CheckBox {
                 id: chB
                 x: 20
                 y: 0
                 width: 15
                 height: 15
                 anchors.right:parent.right
                 anchors.top: parent.top
                 anchors.rightMargin: 17
                 indicator.width:10
                 indicator.height:10
                 text: qsTr("Check Box")
                 onCheckedChanged: {
                    if (checkState == Qt.Checked)
                    {
                        bb2.visible = true
                        mt.anchors.top = bb2.bottom
                    }
                    else{
                        bb2.visible = false
                        mt.anchors.top = bb1.bottom
                    }
                 }
             }
//             ScrollBar{
//                 id: scroll
//                 x: 198
//                 y: 0
//                 width: 10
//                 visible: true
//                 anchors.top:parent.top
//                 anchors.right: parent.right
//                 anchors.bottom: parent.bottom
//                 orientation: Qt.Vertical
//                 hoverEnabled: true
//                 active: hovered || pressed
//                 size: rightPanel.height/(shutterCal.y+shutterCal.height)
//                 onPositionChanged: {
//                     console.log(scroll.position)
//                     chB.y = -scroll.position*rightPanel.height
//                     twoBB.y = -scroll.position*rightPanel.height
//                 }
//              }
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

     function parseConfig(jsonConfig){
         bb1.textX = jsonConfig["FirstBlackBody"]["x"]
         bb1.textY = jsonConfig["FirstBlackBody"]["y"]

         if (jsonConfig["FirstBlackBody"]["listen"])
         {
             bb1.lstn = true
             var index1 = bb1.macCB.indexOfValue(jsonConfig["FirstBlackBody"]["mac"])
             bb1.macCB.currentIndex = index1
         }
         else bb1.temp = jsonConfig["FirstBlackBody"]["t"]
         bb2.textX = jsonConfig["SecondBlackBody"]["x"]
         bb2.textY = jsonConfig["SecondBlackBody"]["y"]
         if (jsonConfig["SecondBlackBody"]["listen"])
         {
             bb2.lstn = true
             var index2 = bb2.macCB.indexOfValue(jsonConfig["SecondBlackBody"]["mac"])
             bb2.macCB.currentIndex = index2
         }
         else bb2.temp = jsonConfig["SecondBlackBody"]["t"]
     }
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
