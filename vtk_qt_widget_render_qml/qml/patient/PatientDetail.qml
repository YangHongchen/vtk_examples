import QtQuick 2.15

Rectangle {
    implicitWidth: parent?parent.width:800
    implicitHeight: parent?parent.height:600
    color: "purple"

    property alias patientId: txt.text


    Text {
        id:txt

        font.pixelSize: 32
        anchors.centerIn: parent
    }
}



