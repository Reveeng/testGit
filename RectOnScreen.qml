import QtQuick 2.0

Rectangle {
    //properties
    property int heightBeforeExp:50
    property int widthBeforeExp:50
    property int xBeforeExp:0
    property int yBeforeExp:0
    property double scaleFactorX: 1
    property double scaleFactorY: 1
    property int xDev:0
    property int yDev:0
    property int heightDev:0
    property int widthDev:0


    signal endOfMovement(int x, int y , int height, int width)
    //standart properies
    id:parRect
    visible: false
    x:0
    y:0
    width:50
    height:50
    border {
        width: 2
        color: "steelblue"
    }
    color: "#00000000"

    //signal proccesing
    onXDevChanged: parRect.x = xDev*scaleFactorX-4
    onYDevChanged: parRect.y = yDev*scaleFactorY-4
    onWidthDevChanged: {
        parRect.width = widthDev*scaleFactorY
        savePosAndGeom(parRect.x*scaleFactorX,parRect.y*scaleFactorY,parRect.height*scaleFactorX,parRect.width*scaleFactorY,0)
    }
    onHeightDevChanged: parRect.height = heightDev*scaleFactorX

    //widgets
    MouseArea {
        anchors.fill: parent
        drag{
            target: parent
            minimumX: 0
            minimumY: 0
            maximumX: parent.parent.width - parent.width
            maximumY: parent.parent.height - parent.height
            smoothed: true
        }
        onReleased: savePosAndGeom(parRect.x*scaleFactorX,parRect.y*scaleFactorY,parRect.height*scaleFactorX,parRect.width*scaleFactorY,1)
        onDoubleClicked: parent.visible = false
    }

    Rectangle {
        width: 7
        height: 7
        color: "#00000000"
        anchors.left: parent.left
        anchors.top: parent.top
        Image{
            anchors.fill: parent
            source: "dotsleft.png"
        }
        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAndYAxis }
            onMouseXChanged: {
                if(drag.active){
                    parRect.x =parRect.x + mouseX
                }
            }
            onMouseYChanged: {
                if(drag.active){
                    parRect.height = parRect.height - mouseY
                    parRect.y = parRect.y + mouseY

                }
            }
            onReleased: savePosAndGeom(parRect.x*scaleFactorX,parRect.y*scaleFactorY,parRect.height*scaleFactorX,parRect.width*scaleFactorY,1)
        }
    }
    Rectangle {
        width: 7
        height: 7
        color: "#00000000"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Image{
            anchors.fill: parent
            source: "dotsright.png"
        }
        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAndYAxis }
            onMouseXChanged: {
                if(drag.active){
                    parRect.width = parRect.width + mouseX
                }
            }
            onMouseYChanged: {
                if(drag.active){
                    parRect.height = parRect.height + mouseY
                }
            }
            onReleased: savePosAndGeom(parRect.x*scaleFactorX,parRect.y*scaleFactorY,parRect.height*scaleFactorX,parRect.width*scaleFactorY, 1)
        }
    }

    //functions
    function savePosAndGeom(x,y,height,width,add){
        endOfMovement(x,y,height,width)
        xBeforeExp = x
        yBeforeExp = y
        heightBeforeExp = height
        widthBeforeExp = width
//            if (add === 1){
//                snapshot.addRectClick(Math.round(parRect.x*scaleFactorX).toString(),Math.round(parRect.y*scaleFactorY).toString(),Math.round(parRect.width*scaleFactorX).toString(),
//                                  Math.round(parRect.height*scaleFactorY).toString())}
    }
    function scaleRect(h, w){

        parRect.height = Math.round(rectonscreen.heightBeforeExp*h/480)
        parRect.width = Math.round(rectonscreen.widthBeforeExp*w/640)
        parRect.x = Math.round(rectonscreen.xBeforeExp*w/640)
        parRect.y = Math.round(rectonscreen.yBeforeExp*h/480)
        console.log(parRect.height,parRect.width,parRect.x,parRect.y)
        parRect.scaleFactorX = 640/w
        parRect.scaleFactorY = 480/h
    }
}

