import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    //properties
    property int index

    //alias properties
    property alias textX: textX.text
    property alias textY: textY.text
    property alias temp: tempField.text
    property alias macCB: macs
    property alias lstn: listen.checked
    property alias indColor: indic.color

    //standart properties
    visible: true
    anchors.right:parent.right
    anchors.left:parent.left
    height: 110

    //signals
    signal setBlackBody(int x, int y)

    //widgets
    TextField {
        id: tempField
        x: 105
        y: 47
        width: 87
        height: 22
        font.pointSize: 7
        enabled: true
        text: "0"
    }

    Text {
        x: 8
        y: 49
        text: qsTr("Температура")
        font.pointSize: 10
        color:"white"
    }

    TextField {
        id: textX
        x: 13
        y: 75
        width: 35
        height: 22
        horizontalAlignment: Text.AlignLeft
        renderType: Text.NativeRendering
        font.pointSize: 8
    }

    TextField {
        id: textY
        x: 64
        y: 75
        width: 35
        height: 22
        horizontalAlignment: Text.AlignLeft
        renderType: Text.NativeRendering
        font.pointSize: 8
    }

    Text {
        id: x1
        x: 4
        y: 79
        width: 15
        height: 14
        text: qsTr("X")
        font.pixelSize: 12
        color:"white"
    }

    Text {
        id: x2
        x: 54
        y: 79
        width: 15
        height: 14
        color: "#ffffff"
        text: qsTr("Y")
        font.pixelSize: 12
    }

    Button {
        id: button
        x: 105
        y: 75
        width: 87
        height: 22
        focusPolicy: Qt.NoFocus
        text: qsTr("Поставить")
        font.pixelSize: 12
        onClicked: {
            if (textX.text != "" && textY.text != "")
                setBlackBody(Number(textX.text),Number(textY.text))
            else
                setBlackBody(0,0)
        }
    }

    CheckBox {
        id: listen
        x: 175
        y: 23
        width: 24
        height: 20
        indicator.height: 10
        focusPolicy: Qt.NoFocus
        nextCheckState: {
                if (checkState === Qt.Checked){
                    snapshot.listen = true
                    tempField.enabled = false
                    tempField.text = snapshot.temps[macs.currentIndex]
                }
                else{
                    snapshot.listen = false
                    tempField.enabled = true
                    tempField.text = ""
                }
        }
        indicator.width: 10
    }

    Rectangle {
        id: rectangle
        x: 2
        y: 76
        height: 3
        color: "#666666"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textX.bottom
        anchors.topMargin: 10
        anchors.rightMargin: 0
        anchors.leftMargin: 0
    }

    Text {
        id: text1
        x: 8
        y: 0
        text: "АЧТ "+index
        font.pixelSize: 12
        color:"white"
    }

    ComboBox {
        id: macs
        x: 8
        y: 18
        width: 168
        height: 23
        focusPolicy: Qt.NoFocus
    }

    Rectangle {
        id: indic
        anchors.verticalCenter: text1.verticalCenter
        anchors.left:text1.right
        anchors.leftMargin: 8
        width: 8
        height: 8
    }
    function getRefPoint(){
        var bb = [parseInt(textX.text,10), parseInt(textY.text, 10),parseFloat(tempField.text,10)]
        if (listen.checked){
            bb.push(1)
            bb.push(macs.currentText)
        }
        else{
            bb.push(0)
            bb.push(0)
        }
        return bb
    }
}


