import QtQuick
import QtQuick.Layouts

import "../_components"

Rectangle {
    id: root
    width: 420
    height: 40
    color: "#ffffff"
    radius: 8

    property string name: "未设定"
    property int gender: 0
    property string birthDay: "0000-00-00"
    property int age: 999
    property string status: '治疗中'

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        spacing: 10
        Text {
            id: txt_patient_name
            text: root.name || '未设定'
            font.pixelSize: 14
            color: "#1A2133"
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        }
        WImageSvg {
            id: img_patient_sex
            source: root.gender === 1 ? "qrc:/assets/icons/men.svg" : "qrc:/assets/icons/women.svg"
            width: 14
            height: 14
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        }
        Text {
            id: txt_patient_birth
            text: root.birthDay || '0000-00-00'
            font.pixelSize: 14
            color: "#9FA7BD"
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            x: img_patient_sex.x + img_patient_sex.width + 16
        }
        Text {
            id: txt_patient_age
            text: root.age + "岁"
            font.pixelSize: 14
            color: "#9FA7BD"
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            x: txt_patient_birth.x + txt_patient_birth.width + 16
        }
        Text {
            id: txt_patient_status
            text:root.status || '指治疗前'
            font.pixelSize: 14
            color: "#9FA7BD"
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            x: txt_patient_age.x + txt_patient_age.width + 16
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
