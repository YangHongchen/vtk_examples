import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQml 2.15

import "../_components"

Popup {
    id: addPatientPopup
    width: Math.min(600,  parent ? parent.width  * 0.9 : 600)
    height: Math.min(700,  parent ? parent.height  * 0.9 : 700)
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    property string language: Qt.locale().name
    property int mainContentWidth: 800
    property int mainContentHeight: 700

    signal accepted(string firstName, string lastName, string birthday, string phone, string gender)
    signal cancelled()

    background: Rectangle {
        color: "#80000000"
        anchors.fill:  parent
    }

    Item {
        id: popupContent
        anchors.centerIn:  parent
        width: mainContentWidth
        height: mainContentHeight

        Rectangle {
            anchors.fill:  parent
            color: "#ffffff"

            // 标题栏 (固定在顶部)
            Rectangle {
                id: header
                width: parent.width
                height: 50
                color: '#005a9e'
                anchors.top:  parent.top
                Text {
                    text: language === "zh_CN" ? "新增病例" : "Add New Patient"
                    font.weight: 500
                    font.pixelSize:  18
                    color: "#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                    x:15
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
                            font.pixelSize:  16
                            color: "#555555"
                            Layout.preferredWidth:  100  // 固定标签宽度
                            Layout.alignment:  Qt.AlignRight
                        }
                        WTextField {
                            id: firstNameField
                            Layout.preferredWidth:  300  // 固定输入框宽度
                            placeholderText: language === "zh_CN" ? "请输入姓" : "Enter first name"
                            validator: "none"
                        }
                    }

                    // 名 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "名：" : "Last Name:"
                            font.pixelSize:  16
                            color: "#555555"
                            Layout.preferredWidth:  100
                            Layout.alignment:  Qt.AlignRight
                        }
                        WTextField {
                            id: lastNameField
                            Layout.preferredWidth:  300
                            placeholderText: language === "zh_CN" ? "请输入名" : "Enter last name"
                            validator: "none"
                        }
                    }

                    // 性别 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "性别：" : "Gender:"
                            font.pixelSize:  16
                            color: "#555555"
                            Layout.preferredWidth:  100
                            Layout.alignment:  Qt.AlignRight
                        }
                        RowLayout {
                            spacing: 15
                            Layout.preferredWidth:  300
                            RadioButton {
                                id: maleRadio
                                text: language === "zh_CN" ? "男" : "Male"
                                checked: true
                            }
                            RadioButton {
                                id: femaleRadio
                                text: language === "zh_CN" ? "女" : "Female"
                            }
                            RadioButton {
                                id: otherRadio
                                text: language === "zh_CN" ? "其他" : "Other"
                            }
                        }
                    }

                    // 生日 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "生日：" : "Birthday:"
                            font.pixelSize:  16
                            color: "#555555"
                            Layout.preferredWidth:  100
                            Layout.alignment:  Qt.AlignRight
                        }
                        RowLayout {
                            spacing: 10
                            WTextField {
                                id: birthDayField
                                Layout.preferredWidth:  300  // 固定输入框宽度
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
                                onDateSelected: (date) => birthDayField.text  = date
                                visible: false
                            }
                        }
                    }

                    // 手机号 =============
                    RowLayout {
                        spacing: 15
                        Text {
                            text: language === "zh_CN" ? "手机号：" : "Phone:"
                            font.pixelSize:  16
                            color: "#555555"
                            Layout.preferredWidth:  100
                            Layout.alignment:  Qt.AlignRight
                        }
                        WTextField {
                            id: phoneField
                            Layout.preferredWidth:  300
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
                    anchors.centerIn:  parent
                    spacing: 30

                    Button {
                        text: language === "zh_CN" ? "取消" : "Cancel"
                        font.pixelSize:  14
                        width: 120
                        height: 40
                        onClicked: {
                            addPatientPopup.close()
                            addPatientPopup.cancelled()
                        }
                    }

                    Button {
                        text: language === "zh_CN" ? "确认" : "Confirm"
                        font.pixelSize:  14
                        width: 120
                        height: 40
                        enabled: formValid()
                        onClicked: {
                            const gender = maleRadio.checked  ?
                                (language === "zh_CN" ? "男" : "Male") :
                                femaleRadio.checked  ?
                                (language === "zh_CN" ? "女" : "Female") :
                                (language === "zh_CN" ? "其他" : "Other")

                            addPatientPopup.accepted(
                                firstNameField.text,
                                lastNameField.text,
                                birthDayField.text,
                                phoneField.text,
                                gender
                            )
                            addPatientPopup.close()
                        }
                    }
                }
            }
        }
    }

    function formValid() {
        return firstNameField.text.length  > 0 &&
               lastNameField.text.length  > 0 &&
               birthDayField.text  !== "" &&
               phoneField.isValid()
    }

    function resetForm() {
        firstNameField.text  = ""
        lastNameField.text  = ""
        maleRadio.checked  = true
        birthDayField.text  = ""
        phoneField.text  = ""
    }

    onOpened: {
        firstNameField.forceActiveFocus()
        resetForm()
    }
}
