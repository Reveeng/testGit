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
     signal setRefPoints(var point1, var point2)

     //proccesing signals

    //two tab view
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

//             CheckBox {
//                 id:checkbox
//                 x: 176
//                 y: 0
//                 width: 24
//                 height: 20
//                 text: qsTr("Check Box")
//                 indicator.width:10
//                 indicator.height: 10
//                 focusPolicy: Qt.NoFocus
//                 nextCheckState: {
//                    if (checkState === Qt.Checked){
//                        showBlackbody()
//                        snapshot.modeOneBB = false
//                        bb2.visible = true
//                        settingbuttons.vis =  true
//                        settingbuttons.anchors.top = bb2.bottom
//                        settingbuttons.state = "2blackBody"
//                    }
//                    else{
//                        snapshot.modeOneBB = true
//                        bb2.visible = false
//                        hideBlackbody2()
//                        settingbuttons.vis = false
//                        settingbuttons.anchors.top = bb1.bottom
//                        settingbuttons.state = "1blackBody"
//                    }
//                 }
//             }

//             Text {
//                 id:twobbtext
//                 x: 131
//                 y: 0
//                 anchors.verticalCenter: checkbox.verticalCenter
//                 height: 12
//                 text: qsTr("Два АЧТ")
//                 color:"white"
//                 font.pixelSize: 10
//                 anchors.rightMargin: -1
//             }

             BBItem{
                 id:bb1
                 y: 54
                 index:1
                 anchors.top: parent.top
                 anchors.topMargin: 5
                 Component.onCompleted: setBlackBody.connect(setBlackBody1)
             }

             BBItem{
                id:bb2
                y: 135
                anchors.top: bb1.bottom
                anchors.topMargin: 3
                index: 2
                Component.onCompleted: setBlackBody.connect(setBlackBody2)
             }

//             SettingButtons{
//                id:settingbuttons
//                anchors.top:bb1.bottom
//                onCalibRefPoints: snapshot.calibrRefPoints(fbb,sbb)
//             }

//             PalletePanel{
//                 id:palPanel
//                 x: 0
//                 y: 153
//                 width: 200
//                 anchors.top: rpdivider.bottom
//                 anchors.topMargin: 6
//                 onYChanged: {
//                    if(y+height>rightPanel.height){
//                        scroll.size = (rightPanel.height-70)/(y+height)
//                        scroll.visible = true
//                    }
//                    else
//                        scroll.visible = false
//                 }
//             }

//             Rectangle {
//                 id: rpdivider
//                 x: 2
//                 y: 153
//                 height: 3
//                 color: "#666666"
//                 anchors.left: parent.left
//                 anchors.right: parent.right
//                 anchors.top: bb2.bottom
//                 anchors.topMargin: 10
//                 anchors.leftMargin: 0
//                 anchors.rightMargin: 0
//             }

            Button{
                id:setRefPoints
                text:"Установить по опорным точкам"
                anchors.left:parent.left
                anchors.right: parent.right
                anchors.top: bb2.bottom
                anchors.margins: 5
                height: 25
                onClicked: {

                }
            }
            Button{
                id:startbtn
                text: "start"
                width:50
                height:50
                x:50
                y:300
                onClicked:rightPanel.start()
            }
             ScrollBar{
                id: scroll
                x: 198
                y: 0
                width: 10
                visible: true
                anchors.top:parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                orientation: Qt.Vertical
                hoverEnabled: true
                active: hovered || pressed
//                size: rightPanel.height/(palPanel.y+palPanel.height)
                onPositionChanged: {
                    checkbox.y = -scroll.position*rightPanel.height
                    twobbtext.y = -scroll.position*rightPanel.height
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

