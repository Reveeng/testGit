import QtQuick 2.0
import QtQuick.Controls 2.12

Dialog{
    id: dialog
    signal adressAccepted(string adress)
    property alias textAdress: textField.text
    width: 212
    height:135
    modal:true
    standardButtons: Dialog.Ok | Dialog.Cancel
    header: Rectangle{
        height:20
        anchors.right:parent.right
        anchors.left:parent.left
        color:"#808080"
        Text{
            text:"Введите адрес"
            anchors.left:parent.left
            anchors.leftMargin: 3
            anchors.top:parent.top
            anchors.bottom:parent.bottom
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    TextField {
        id: textField
        x: 0
        y: 8
        width: 188
        height: 32
        placeholderText: qsTr("Введите адрес")
        Keys.onPressed:{
            if (event.key+1 === Qt.Key_Enter){
//                auxiliaryClass.saveAddresToFile(textField.text)
                dialog.close()
                adressAccepted(textField.text)
            }
        }
    }
    onAccepted: {
        if (textField.text != "")
        {
//            auxiliaryClass.saveAddresToFile(textField.text)
            adressAccepted(textField.text)
        }
    }
}


