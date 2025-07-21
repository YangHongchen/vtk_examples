import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQml 2.15

import "../_components"

Popup {
    id: addPatientPopup
    width: Math.min(600, parent ? parent.width * 0.9 : 600)
    height: Math.min(700, parent ? parent.height * 0.9 : 700)
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    property string language: Qt.locale().name
    property int mainContentWidth: 800
    property int mainContentHeight: 700

    // form表单字段属性
    property int id: 0
    property alias firstName: txt_firstName.text
    property alias lastName: txt_lastName.text
    property int gender: 2
    property alias phone: txt_phone.text
    property alias birthDay: txt_birthDay.text

    // 基于表单生成数据
    function gatherFormData() {
        var formDataObj = {
            "id": id,
            "firstName": firstName,
            "lastName": lastName,
            "gender": gender,
            "phone": phone,
            "birthDay": birthDay
        }
        return formDataObj
    }

    signal accepted(var formDataObj)
    signal cancelled

    background: Rectangle {
        color: "#80000000"
        anchors.fill: parent
    }

    Item {
        id: popupContent
        anchors.centerIn: parent
        width: mainContentWidth
        height: mainContentHeight

        Rectangle {
            anchors.fill: parent
            color: "#ffffff"

            // 标题栏 (固定在顶部)
            Rectangle {
                id: header
                width: parent.width
                height: 50
                color: '#005a9e'
                anchors.top: parent.top
                Text {
                    text: language === "zh_CN" ? "新增病例" : "Add New Patient"
                    font.weight: 500
                    font.pixelSize: 18
                    color: "#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 15
                }
            }

            // 表单区域 (可滚动)
            ScrollView {
                id: scrollView
                anchors {
                    top: header.bottom
                    left: parent.left
                    right: parent.right
                    bottom: footer.top
                    margins: 20
                }
                clip: true

                // 表单内容容器
                ColumnLayout {
                    width: scrollView.availableWidth
                    spacing: 15

                    // 姓 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "姓：" : "First Name:"
                            font.pixelSize: 16
                            color: "#555555"
                            Layout.preferredWidth: 100 // 固定标签宽度
                            Layout.alignment: Qt.AlignRight
                        }
                        WTextField {
                            id: txt_lastName

                            Layout.preferredWidth: 300 // 固定输入框宽度
                            placeholderText: language === "zh_CN" ? "请输入姓" : "Enter first name"
                            validator: "none"
                        }
                    }

                    // 名 =============
                    RowLayout {
                        spacing: 15
                        Text {

                            text: language === "zh_CN" ? "名：" : "Last Name:"
                            font.pixelSize: 16
                            color: "#555555"
                            Layout.preferredWidth: 100
                            Layout.alignment: Qt.AlignRight
                        }
                        WTextField {
                            id: txt_firstName
                            Layout.preferredWidth: 300
                            placeholderText: language === "zh_CN" ? "请输入名" : "Enter last name"
                            validator: "none"
                        }
                    }

                    // 性别 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "性别：" : "Gender:"
                            font.pixelSize: 16
                            color: "#555555"
                            Layout.preferredWidth: 100
                            Layout.alignment: Qt.AlignRight
                        }

                        RowLayout {
                            spacing: 15
                            Layout.preferredWidth: 300

                            ButtonGroup {
                                id: genderGroup
                                onCheckedButtonChanged: {
                                    // 更新绑定属性
                                    if (checkedButton) {
                                        selectedGender = checkedButton.text
                                    }
                                }
                            }

                            RadioButton {
                                id: maleRadio
                                text: language === "zh_CN" ? "男" : "Male"
                                property int value: 1
                                checked: gender = value
                                onCheckedChanged: if (checked)
                                                      gender = value
                                ButtonGroup.group: genderGroup
                            }
                            RadioButton {
                                id: femaleRadio
                                text: language === "zh_CN" ? "女" : "Female"
                                property int value: 0
                                checked: gender = value
                                onCheckedChanged: if (checked)
                                                      gender = value
                                ButtonGroup.group: genderGroup
                            }
                            RadioButton {
                                id: otherRadio
                                text: language === "zh_CN" ? "其他" : "Other"
                                property int value: 2
                                checked: gender = value
                                onCheckedChanged: if (checked)
                                                      gender = value
                                ButtonGroup.group: genderGroup
                            }
                        }
                    }

                    // 生日 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "生日：" : "Birthday:"
                            font.pixelSize: 16
                            color: "#555555"
                            Layout.preferredWidth: 100
                            Layout.alignment: Qt.AlignRight
                        }
                        RowLayout {
                            spacing: 10
                            WTextField {
                                id: txt_birthDay
                                Layout.preferredWidth: 300 // 固定输入框宽度
                                placeholderText: language === "zh_CN" ? "选择生日" : "Select date"
                            }
                            Button {
                                id: btnDateToggle
                                width: 32
                                height: 32
                                text: "📅"
                                onClicked: datePicker.visible = true
                            }
                            WDatePicker {
                                id: datePicker
                                anchorItem: birthDayField
                                onDateSelected: date => txt_birthDay.text = date
                                visible: false
                            }
                        }
                    }

                    // 手机号 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "手机号：" : "Phone:"
                            font.pixelSize: 16
                            color: "#555555"
                            Layout.preferredWidth: 100
                            Layout.alignment: Qt.AlignRight
                        }
                        WTextField {
                            id: txt_phone
                            Layout.preferredWidth: 300
                            placeholderText: language === "zh_CN" ? "请输入手机号" : "Enter phone number"
                            validator: "phone"
                        }
                    }
                }
            }

            // 底部按钮区域
            Rectangle {
                id: footer
                height: 80
                color: "transparent"
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 30

                    Button {
                        text: language === "zh_CN" ? "取消" : "Cancel"
                        font.pixelSize: 14
                        width: 120
                        height: 40
                        onClicked: {
                            addPatientPopup.close()
                            addPatientPopup.cancelled()
                        }
                    }

                    Button {
                        text: language === "zh_CN" ? "确认" : "Confirm"
                        font.pixelSize: 14
                        width: 120
                        height: 40
                        enabled: formValid()
                        onClicked: {
                            var formDataObj = gatherFormData()
                            console.log('生成的表单数据：', JSON.stringify(formDataObj))
                            accepted(formDataObj)
                            addPatientPopup.close()
                        }
                    }
                }
            }
        }
    }

    // 校验表单数据（合法则可以提交）
    function formValid() {
        return txt_firstName.text.length > 0 && txt_lastName.text.length > 0
                && txt_birthDay.text !== "" && txt_phone.isValid()
    }

    function resetForm() {
        txt_firstName.text = ""
        txt_lastName.text = ""
        txt_birthDay.text = ""
        txt_phone.text = ""
    }

    onOpened: {
        txt_firstName.forceActiveFocus()
        resetForm()
    }
}
