import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../_components"

Rectangle {
    id: root

    property int _id: 0
    property string firstName: "-"
    property string lastName: "-"
    property int sex: 0 // 性别：1男性，0女性，2未知
    property string phone: "-"
    property string birthDay: "-"
    property string lastTestingTime: "-" // 最近检查时间

    property int leftMargin: 24
    property int topMargin: 12
    property bool selected: false

    width: 320
    height: 64
    color: root.selected ? '#f2f3f5' : "#ffffff" // 背景色
    Item {
        id: content
        x: root.leftMargin
        y: root.topMargin
        width: root.width - root.leftMargin * 2
        height: root.height - root.topMargin * 2

        RowLayout {
            anchors.fill: parent
            spacing: 0
            Rectangle {
                // 头像（默认展示姓氏）
                id: avatar
                color: "#E5F3FF"
                width: 40
                height: 40
                radius: 20
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                Text {
                    id: txt_avatar
                    anchors.centerIn: parent
                    font.pixelSize: 16
                    font.weight: 500
                    color: "#0B4DA2"
                    text: root.firstName ? root.firstName.substring(0, 1) : "#"
                }
            }
            Item {
                width: 12
                height: 40
            }
            Item {
                Layout.fillWidth: true
                height: 40
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0
                    RowLayout {
                        Layout.fillWidth: true
                        height: 20
                        Text {
                            // 姓名
                            text: root.lastName + " " + root.firstName
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            color: "#1A2133"
                            font.pixelSize: 14
                            font.weight: 500
                        }

                        WImageSvg {
                            source: root.sex === 0 ? "qrc:/assets/icons/women.svg" : "qrc:/assets/icons/men.svg"
                            width: 14
                            height: 14
                            color: root.sex === 0 ? "#F98981" : "#4080FF"
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        height: 20
                        Text {
                            // 手机
                            text: root.phone
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            color: "#9FA7BD"
                        }
                        Item {
                            // 中间自动填充
                            Layout.fillWidth: true
                        }
                        Text {
                            // 生日
                            text: root.birthDay
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            color: "#9FA7BD"
                        }
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            menu.popup(mouse.x, mouse.y) // 在鼠标位置弹出菜单
        }
    }

    Menu {
        id: menu
        MenuItem {
            text: qsTr("编辑")
            onTriggered: console.log("编辑")
        }
        MenuItem {
            text: qsTr("删除")
            onTriggered: confirmDialog.open()
        }
    }

    Dialog {
        id: confirmDialog
        title: qsTr("确认操作")
        width: 240
        height: 100
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: PatientController.deletePatient(root._id)
        onRejected: console.log("用户取消")
        contentItem: Column {
            spacing: 10
            Label {
                text: qsTr("你确定要执行此操作吗？")
                font.pixelSize: 16
            }
        }
    }
}
