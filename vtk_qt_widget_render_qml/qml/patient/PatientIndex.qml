import QtQuick 2.15
import QtQuick.Controls

Item {
    anchors.fill: parent
    Text {
        text:"病例-首页"
        anchors.centerIn: parent
        font.pixelSize: 32
    }


    Column {
        Text {
            id: txt2
            text: patientController? patientController.name:"未设置姓名"
        }

        Button {
            id:btn
            text: "点我更新姓名"
            onClicked: {
                patientController.setName("xingchen")
            }
        }

    }

    Connections {
        target: patientController
        function onNameChanged(_name) {
            console.log('名字更新了：',_name)
            txt2.text = _name
        }
    }
}

