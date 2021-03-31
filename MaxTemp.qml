import QtQuick 2.0

Item {

    property var maxTemp:0
    anchors.left: parent.left
    anchors.right:parent.right
    height: 50
    onMaxTempChanged: {
        if (0 < maxTemp < 34)
            back.color = "#ccf1f6"
        if (34 < maxTemp < 37)
            back.color = "#d6a268"
        if (maxTemp > 37)
            back.color = "red"
        txt.text = maxTemp
    }
    Text{
        id:jt
        text: qsTr("Максимальная температура в квадрате")
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
    }
    Rectangle{
        id:back
        anchors.top:jt.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        height:25
        color: "white"
        width:100
        Text{
            id:txt
            font.pixelSize: 20
            color:"white"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        id: botDivider
        height: 3
        color: "#666666"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
