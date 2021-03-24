import QtQuick 2.0
import QtQuick.Controls 2.0
import QtMultimedia 5.0
import GstVideoPlayer 0.1

import infratest.filters 1.0

Item{
     //properties
     property var previousState: "default"
     property bool settingMode: false
     property bool autoplay:false
     property real aspectRatio:4/3

     //alias properties
     property alias bb1X:bb1indicator.x
     property alias bb1Y:bb1indicator.y
     property alias bb1Vis:bb1indicator.visible
     property alias bb2X:bb2indicator.x
     property alias bb2Y:bb2indicator.y
     property alias bb2Vis:bb2indicator.visible

     property alias vis: rectonscreen.visible

     //signals
     signal bbChangedByMouse(int x, int y, int number)
     signal bbChangedByButton(int x, int y, int number)
     signal reconnect(var adress)

     //standart properties
     width:640
     height:495

     states:[
         State {name: "default"},
         State {name:"fullscreen"}
     ]
     state: "default"

    Component.onCompleted: {
        heightChanged.connect(scaling)
        widthChanged.connect(scaling)
        stateChanged.connect(stateProc)
    }

    Connections{
        target:snapshot
        onHasAddress:{
            setSource(address)
        }
    }

     Rectangle{
        id:header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height:15
        color:"#666666"
        Rectangle{
            id:workIndicator
            anchors.verticalCenter:  parent.verticalCenter
            anchors.right: parent.right
            height:10
            width:10
            radius: 10
            color:"red"
        }
     }
     FilterDeconv {id: fltDeconv;}
     FilterContrast {id: fltContrast;}
     FilterTemperature{id: fltTemperature;
        temperature.refCool: Refpoint{
            id:refCool
        }
        temperature.refHot: Refpoint{
            id:refHot
        }
        temperature.stat.onMaxChanged: {
            rectonscreen.maxT = Math.round(max*10)/10
        }
     }

     //video player
     GstVideoPlayer{
         id:myplayer
         onErrChanged: {
             console.log(err)
             workIndicator.color = "red"
         }
         onFpsChanged: {
//             console.log(fps)
         }
         onSourceChanged: console.log(source)
         onEverythingOkay: workIndicator.color = "green"
     }
     VideoOutput{
         id:output
         anchors.top:header.bottom
         anchors.left:parent.left
         anchors.right: parent.right
         anchors.bottom: parent.bottom
         filters: [fltDeconv,fltContrast, fltTemperature]
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
                x: 243
                y: 132
                color: "red"
                onXChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),1)
                onYChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),1)
                visible: true
            }
            BlackBodyIndicator{
                id:bb2indicator
                x: 408
                y: 132
                width: 8
                height: 8
                color: "green"
                onXChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),2)
                onYChanged: bbChangedByMouse(Math.round(x*scaleFoctorX),Math.round(y*scaleFoctorY),2)
                visible: true
            }

            RectOnScreen{
                id:rectonscreen
                x: 295
                y: 215
                Component.onCompleted: {
                    fltTemperature.temperature.stat.roi = Qt.rect(x,y,width,height)
                }
                onEndOfMovement: {
                    fltTemperature.temperature.stat.roi = Qt.rect(x,y,width,height)
                }
                visible: true
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

     function setRefPoints(point1, point2){
        console.log(point1, point2)
        if (point1[2] < point2[2]){
            refCool.x = point1[0]
            refCool.y = point1[1]
            refCool.t = point1[2]
            refHot.x = point2[0]
            refHot.y = point2[1]
            refHot.t = point2[2]
        }
        else{
            refCool.x = point2[0]
            refCool.y = point2[1]
            refCool.t = point2[2]
            refHot.x = point1[0]
            refHot.y = point1[1]
            refHot.t = point1[2]
        }
     }

     function scaling(){
         if (width/(height-15) < aspectRatio){
             mouseArea.width = output.width
             mouseArea.height = (1/aspectRatio)*output.width
             rectonscreen.scaleRect(mouseArea.height , mouseArea.width)
             bb1indicator.scaleDots(mouseArea.height , mouseArea.width)
             bb2indicator.scaleDots(mouseArea.height , mouseArea.width)
             }
         else{
             mouseArea.height = output.height
             mouseArea.width = aspectRatio*output.height
             rectonscreen.scaleRect(mouseArea.height , mouseArea.width)
             bb1indicator.scaleDots(mouseArea.height , mouseArea.width)
             bb2indicator.scaleDots(mouseArea.height , mouseArea.width)
         }
     }

     function stateProc(state){
         if (state === "fullscreen"){
             scaling()
         }
         if (state === "default"){
             scaling()
             }
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
        myplayer.start()
     }

     function setSource(address){
        snapshot.adress = address
        snapshot.allBlackBodyAdress()
        myplayer.source = "rtspsrc location=rtsp://"+address+"/rawdata latency=0 ! "+
                          "rtpvrawdepay ! queue ! "+
                          "appsink max-buffers=3 drop=true emit-signals=true name=sink0";
        myplayer.start()
        snapshot.saveAddress(address)

     }

     function hideIndicators(mode){
        bb1indicator.enableMovement = mode
        bb2indicator.enableMovement = mode
     }
}




