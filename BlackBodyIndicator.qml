import QtQuick 2.0

Rectangle{
    //properties

    property int xBeforeExp:0
    property int yBeforeExp:0
    property int heightBeforeExp:8
    property int widthBeforeExp:8
    property double scaleFoctorX:1
    property double scaleFoctorY:1

    property alias enableMovement:ma.enabled

    signal endOfMovement()
    //standart properties
    id: bbi
    width: 8
    height:8
    Component.onCompleted:savePosAndGeom(x,y,width,height)
    //signal proccesing
    onVisibleChanged:{
        xBeforeExp = x*scaleFoctorX
        yBeforeExp = y*scaleFoctorY
        heightBeforeExp = height*scaleFoctorY
        widthBeforeExp = width*scaleFoctorX
    }

    //widgets
    MouseArea{
        id:ma
        anchors.fill: parent
        enabled: false
        drag{
            target:parent
            minimumX: 0
            minimumY: 0
            maximumX: parent.parent.width - parent.width
            maximumY: parent.parent.height - parent.height
            smoothed: true
        }
        onReleased: {
            parent.xBeforeExp = parent.x*parent.scaleFoctorX
            parent.yBeforeExp = parent.y*parent.scaleFoctorY
            endOfMovement()
        }
    }

    function savePosAndGeom(x,y,width,height){
        xBeforeExp = x
        yBeforeExp = y
        heightBeforeExp = height
        widthBeforeExp = width
    }
    function scaleDots(h, w){
       scaleFoctorX = 640/w
       scaleFoctorY = 480/h
       console.log("before", xBeforeExp)
       x = Math.round(xBeforeExp*w/640)
       y = Math.round(yBeforeExp*h/480)
       console.log("after", x)
       height = Math.round(heightBeforeExp*h/480)
       width = Math.round(widthBeforeExp*w/640)
    }

}
