import QtQuick 2.0

Rectangle{
    //properties

    property int xBeforeExp:0
    property int yBeforeExp:0
    property int heightBeforeExp:8
    property int widthBeforeExp:8
    property double scaleFactorX:1
    property double scaleFactorY:1

    property alias enableMovement:ma.enabled

    signal endOfMovement()
    //standart properties
    id: bbi
    width: 8
    height:8
    Component.onCompleted:savePosAndGeom(x,y,width,height)
    //signal proccesing
    onVisibleChanged: savePosAndGeom(x*scaleFoctorX, y*scaleFoctorY, height*scaleFoctorY, width*scaleFoctorX )
//    onXChanged: savePosAndGeom(x*scaleFoctorX, y*scaleFoctorY, height*scaleFoctorY, width*scaleFoctorX )
//    onYChanged: savePosAndGeom(x*scaleFoctorX, y*scaleFoctorY, height*scaleFoctorY, width*scaleFoctorX )

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
            savePosAndGeom(parent.x*parent.scaleFactorX,parent.y*parent.scaleFactorY,parent.height*scaleFactorX,parent.width*scaleFactorY)
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
       scaleFactorX = 640/w
       scaleFactorY = 480/h
       x = Math.round(xBeforeExp*w/640)
       y = Math.round(yBeforeExp*h/480)
       height = Math.round(heightBeforeExp*h/480)
       width = Math.round(widthBeforeExp*w/640)
    }

}
