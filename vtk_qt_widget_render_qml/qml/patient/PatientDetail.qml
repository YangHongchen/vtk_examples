import QtQuick 2.15

Rectangle {
    implicitWidth: parent?parent.width:800
    implicitHeight: parent?parent.height:600
    color: "purple"

    Text {
        text: PatientModel.currentPateint? PatientModel.currentPateint.fullName:"未选中病例"
        anchors.centerIn: parent
        font.pixelSize: 24
        color: 'red'
    }
}
