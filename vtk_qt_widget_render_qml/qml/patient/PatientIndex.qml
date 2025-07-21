import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../_components"

Item {
    id: patientPage

    property int _margin: 16 // 设置边距，相对于父元素
    property bool loaded: false

    // 信号：查询病例列表
    signal queryPatients(string keyword, int page, int pageSize)

    Component.onCompleted: {
        if(!patientPage.loaded) {
            patientPage.loaded = true
            console.log('[病例管理页面] 首次加载，调用 loadPatients ')
            PatientController.loadPatients()
        } else {
          console.log('[病例管理页面]重复加载 ')
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // 左侧: 病人列表 ---------------
        Rectangle {
            id: patient_left_rect
            width: 320
            Layout.fillHeight: true
            color: "#ced6e0"

            ColumnLayout {
                anchors.fill: parent
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: '#f4fce3'
                    RowLayout {
                        anchors.fill: parent
                        Item {
                            width: _margin
                        }
                        Text {
                            text: qsTr("病人列表")
                            font.pixelSize: 20
                            font.weight: 600
                            Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        WButton {
                            text: qsTr("新增病例")
                            theme: WButton.Primary
                            iconSource: "qrc:/assets/icons/plus.svg"
                            onClicked: {
                                patientEditPopup.visible = true
                            }
                        }
                        Item {
                            width: _margin
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

                    Text {
                        text: PatientModel.totalCount || "totalCount"
                    }
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

    // 编辑病例
    PatientEditPopup {
        id: patientEditPopup
        anchors.centerIn: parent
    }
}
