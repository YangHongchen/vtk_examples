import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../_components"

Item {
    id: patientPage

    property int _margin: 16 // 设置边距，相对于父元素
    property bool loaded: false

    // 查询条件（关键词）
    property alias keyword: txt_keyword.text

    property variant currentPatiant

    Component.onCompleted: {
        if (!patientPage.loaded) {
            patientPage.loaded = true
            PatientController.loadPatientsConditional(patientPage.keyword, 1, 20)
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
                            id:flag2
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
                            PatientController.loadPatientsConditional(patientPage.keyword, 1, 20)
                        }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: '#fff4e6'
                    ListView {
                        id: listView
                        width: parent.width
                        height: parent.height
                        model: PatientModel
                        delegate: PatientListItem {
                            _id:  model.id
                            firstName: model.firstName
                            lastName: model.lastName
                            phone: model.phone
                            birthDay: model.birthday
                            sex: model.gender
                            selected: index === listView.currentIndex
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    console.log("选中当前病人信息：", model.fullName)
                                    listView.currentIndex = index
                                    PatientController.selectPatient(model.id)
                                }
                            }
                        }

                        Component.onCompleted: {
                            // 页面加载，自动更新数据
                            PatientManager.getPatientList("",1, 20);
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 20
                    color: 'red'


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

            PatientDetail {
                id: dt
                pid: patientPage.currentPatiant?patientPage.currentPatiant.id: "0"
                fullName: patientPage.currentPatiant?patientPage.currentPatiant.fullName: "-"
                gender: patientPage.currentPatiant?patientPage.currentPatiant.gender: 2
                age: patientPage.currentPatiant?patientPage.currentPatiant.age: 0
                birthDay: patientPage.currentPatiant?patientPage.currentPatiant.birthDay: "0000-00-00"
            }
        }
    }

    Connections {
        target: PatientModel
        function onCurrentPatientIdChanged(id) {
            console.log('病例的id变化：',id)
        }
        function onCurrentPatientChanged(_pt) {
            console.log('病例发生变化：',JSON.stringify(_pt))
            patientPage.currentPatiant = _pt
        }
    }

    // 编辑病例
    PatientEditPopup {
        id: patientEditPopup
        anchors.centerIn: parent
    }
}
