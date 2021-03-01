import QtQuick 2.0
import QtQuick.Controls 2.0
import QtMultimedia 5.0
import GstVideoPlayer 0.1

Item{
     //properties
     property var previousState: "default"
     property bool firstChangeW: true
     property bool firstChangeH: true
     property bool settingMode: false
     property bool autoplay:false

     //alias properties
     property alias bb1X:bb1indicator.x
     property alias bb1Y:bb1indicator.y
     property alias bb1Vis:bb1indicator.visible
     property alias bb2X:bb2indicator.x
     property alias bb2Y:bb2indicator.y
     property alias bb2Vis:bb2indicator.visible
     property alias source:myplayer.source
     property alias vis: rectonscreen.visible

     //signals
     signal bbChangedByMouse(int x, int y, int number)
     signal bbChangedByButton(int x, int y, int number)
     signal reconnect(var adress)

     //standart properties
     width:640
     height:480
     states:[
         State {name: "default"},
         State {name:"fullscreen"}
     ]
     state: "default"

     //signals proccesing

     //proccesing fullscreen signal

     onStateChanged: {
        if (state == "fullscreen"){
            scaling()
        }
        if (state == "default"){
            scaling()
            }
        }

     //proccesing size changed signals
     onHeightChanged:{
        if (firstChangeH)
            firstChangeH = false
        else{
            scaling()
        }
     }
     onWidthChanged: {
        if (firstChangeW)
            firstChangeW = false
        else{
            scaling()
        }
     }

     //call when program set to setting mode
     onSettingModeChanged: {
        if (!settingMode){
            rectonscreen.visible = false
            bb1indicator.visible = false
            bb2indicator.visible = false
        }
        else{
            rectonscreen.visible = true
            bb1indicator.visible =  true
            if (!snapshot.modeOneBB)
                bb2indicator.visible = true
        }
     }

     //video player
     GstVideoPlayer{
         id:myplayer
         onErrChanged: {
             console.log(err)
         }
     }
     VideoOutput{
         id:output
         anchors.fill:parent
         source: myplayer
         MouseArea{
            id:mouseArea
            height:480
            width:640
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            hoverEnabled: true
            acceptedButtons: Qt.RightButton
            BlackBodyIndicator{
                id:bb1indicator
                color: "red"
                onXChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),1)
                onYChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),1)
            }
            BlackBodyIndicator{
                id:bb2indicator
                color: "green"
                onXChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),1)
                onYChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),1)

            }

            RectOnScreen{
                id:rectonscreen
            }

            BusyIndicator {
                id: busyIndicator
                x: 145
                y: 215
                anchors.right: disc.left
                anchors.verticalCenter: parent.verticalCenter
                width: 60
                height: 50
                running: false
            }

            Text {
                property var txt:""
                id: disc
                x: 211
                y: 228
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: txt
                color:"white"
                font.pixelSize: 20
                visible: false
            }
        }
     }

     //functions
     function scaling(){
         if (width/height < 4/3){
             mouseArea.width = output.width
             mouseArea.height = 3/4*output.width
             rectonscreen.scaleRect(mouseArea.height , mouseArea.width)
             bb1indicator.scaleDots(mouseArea.height , mouseArea.width)
             bb2indicator.scaleDots(mouseArea.height , mouseArea.width)
             }
         else{
             mouseArea.height = output.height
             mouseArea.width = 4/3*output.height
             rectonscreen.scaleRect(mouseArea.height , mouseArea.width)
             bb1indicator.scaleDots(mouseArea.height , mouseArea.width)
             bb2indicator.scaleDots(mouseArea.height , mouseArea.width)
         }
     }
     function scaleAndSendCoord1BB(oldx, oldy, number){
            var newx = Math.round(oldx*640/mouseArea.width)
            var newy = Math.round(oldy*480/mouseArea.height)
            bbChangedByButton(newx,newy,number)
     }
     function setCoordToHighLiter(x,y,number){
         if(number === 1){
             bb1indicator.x = x
             bb1indicator.y = y
         }
         else{
             bb2indicator.x = x
             bb2indicator.y = y
         }
     }

     function startPlayer(){
        console.log("start")
        myplayer.start()
     }

}



