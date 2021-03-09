import QtQuick 2.0

Rectangle{
    //properties

    property int xBeforeExp:0
    property int yBeforeExp:0
    property int heightBeforeExp:8
    property int widthBeforeExp:8
    property double scaleFoctorX:1
    property double scaleFoctorY:1


    signal endOfMovement()
    //standart properties
    id: bbi
    x:0
    y:0
    width: 8
    height:8
    visible:false

    //signal proccesing
    onVisibleChanged:{
        xBeforeExp = x*scaleFoctorX
        yBeforeExp = y*scaleFoctorY
        heightBeforeExp = height*scaleFoctorY
        widthBeforeExp = width*scaleFoctorX
    }

    //widgets
    MouseArea{
        anchors.fill: parent
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
    function scaleDots(h, w){
<<<<<<< HEAD
       bbi.scaleFoctorX = 640/w
       bbi.scaleFoctorY = 480/h
       bbi.x = Math.round(bb1indicator.xBeforeExp*w/640)
       bbi.y = Math.round(bb1indicator.yBeforeExp*h/480)
       bbi.height = Math.round(bb1indicator.heightBeforeExp*h/480)
       bbi.width = Math.round(bb1indicator.widthBeforeExp*w/640)
=======
       scaleFoctorX = 640/w
       scaleFoctorY = 480/h
       x = Math.round(xBeforeExp*w/640)
       y = Math.round(yBeforeExp*h/480)
       height = Math.round(heightBeforeExp*h/480)
       width = Math.round(widthBeforeExp*w/640)
>>>>>>> fa21a2fbf5457dcfdb3cb28ab6434158f2aad858
    }

}
