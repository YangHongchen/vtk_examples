import QtQuick 2.15


Item {
    anchors.fill: parent
    Text {
        text:"病例-首页"
        anchors.centerIn: parent
        font.pixelSize: 32
    }

    Text {
        id: txt2
        text: patientController? patientController.name:"未设置姓名"
    }
}

