import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../_components"

Item {
    id: root

    property int _margin: 24 // 设置边距，相对于父元素

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // 左侧: 病人列表 ---------------
        Rectangle {
            id: patient_left_rect
            width: 320
            Layout.fillHeight: true
            color:"#ced6e0"

            ColumnLayout {
                anchors.fill: parent
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: '#f4fce3'
                    RowLayout {
                        anchors.fill: parent
                        Item {
                            width:_margin
                        }
                        Text {
                            text:qsTr("病人列表")
                            font.pixelSize: 20
                            font.weight: 600
                            Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Button {
                            text: qsTr("新增病例")
                            icon: "qrc:/assets/icons/plus.svg"
                            theme: "primary"
                            Layout.alignment: Qt.AlignVCenter
                            size: "small"
                            radius: 8
                            width: 108
                            height: 40
                            onClicked: {
                                // patientEdit.visible = true
                            }
                        }
                        Item {
                            width:_margin
                        }
                    }
                }

                // 搜索框
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: '#f4fce3'
                    WTextField {
                        id: txt_keyword
                        width: parent.width - _margin * 2
                        height: 36
                        topMargin: 4
                        bottomMargin: 4
                        fontSize: 14
                        x: _margin
                        y: (parent.height - height) / 2
                        placeholderText: qsTr("输入姓名或电话筛选")
                        onTextChanged: {
                            // PatientManager.getPatientList(text, 1, 20);
                        }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: '#fff4e6'
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: '#f3f0ff'
                }
            }
        }

        // 中间: 分割线 ---------------
        Rectangle {
            id: border_center
            width: 1
            Layout.fillHeight: true
            color: "#f0f2f5"
        }

        // 右侧: 病人详情 ---------------
        Rectangle {
            id: patient_right_rect
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#ffeaa7"
        }
    }

}
