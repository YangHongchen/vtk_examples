import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import "./_components"

Item {
    id: loginPage
    Layout.leftMargin: _padding
    Layout.rightMargin: _padding


    // 登录成功信号
    signal loginSuccess(string username)

    // properties
    property real _padding: 16

    property int _width: 32
    property int _height: 32
    property string _title: '电子面弓'

    // 发起登录请求的信号
    signal loginRequest(string username, string password)

    Rectangle {
        anchors.fill: parent
        WImageSvg {
            anchors.fill: parent
            source: "qrc:/assets/images/login_background.jpg"
            scale: 1.2
        }
        z: 1
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        z: 2

        // 顶部工具栏
        // Item {
        //     id: topBar
        //     Layout.fillWidth: true
        //     height: 56
        //     visible:false
        //     RowLayout {
        //         anchors.fill: parent
        //         // 自定义标题
        //         Item {
        //             id: title
        //             implicitWidth: 300
        //             implicitHeight: topBar.height
        //             RowLayout {
        //                 anchors.fill: parent
        //                 WImageSvg {
        //                     source: "qrc:/assets/images/logo.png"
        //                     width: _width
        //                     height: _height
        //                     Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        //                     Layout.leftMargin: 16
        //                 }
        //                 // 自定义标题位置
        //                 Text {
        //                     id: titleLabel
        //                     text: '电子面弓'
        //                     font.pixelSize: 16
        //                     color: "#ffffff"
        //                     Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        //                 }
        //                 // 自动占用甚于空间，左边元素向左
        //                 Item {
        //                     Layout.fillWidth: true
        //                 }
        //             }
        //         }

        //         // 最小化按钮
        //         Rectangle {
        //             id: minimizeButton
        //             width: _width
        //             height: _height
        //             x: closeButton.x - width - 8
        //             color: 'transparent'
        //             Image {
        //                 id: imgMini
        //                 source: "qrc:/window_minimize.svg"
        //                 anchors.centerIn: parent
        //                 width: _width / 2
        //                 height: _height / 2
        //                 sourceSize {
        //                     // 双保险尺寸控制
        //                     width: _width / 2
        //                     height: _height / 2
        //                 }
        //             }
        //             ColorOverlay {
        //                 anchors.fill: imgMini
        //                 source: imgMini
        //                 color: "#ffffff" // 动态修改此属性即可更新颜色
        //             }
        //             MouseArea {
        //                 anchors.fill: parent
        //                 cursorShape: Qt.PointingHandCursor
        //                 onClicked: {
        //                     window.showMinimized()
        //                 }
        //             }
        //         }

        //         // 关闭按钮
        //         Rectangle {
        //             id: closeButton
        //             width: _width
        //             height: _height
        //             x: topBar.width - width - 16
        //             color: 'transparent'
        //             Image {
        //                 id:svgClose
        //                 source: "qrc:/window_close.svg"
        //                 width: _width / 2
        //                 height: _height / 2
        //                 anchors.centerIn: parent
        //                 sourceSize {
        //                     width: _width / 2
        //                     height: _height / 2
        //                 }
        //             }
        //             ColorOverlay {
        //                 anchors.fill: svgClose
        //                 source: svgClose
        //                 color: "#ffffff" // 动态修改此属性即可更新颜色
        //             }
        //             MouseArea {
        //                 anchors.fill: parent
        //                 cursorShape: Qt.PointingHandCursor
        //                 onClicked: {
        //                     window.close()
        //                 }
        //                 onEntered: {
        //                     closeButton.color = ThemeManager.danger
        //                 }
        //                 onExited: {
        //                     closeButton.color = "transparent"
        //                 }
        //             }
        //         }
        //     }
        // }

        // 底部内容
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: 'transparent'
            Rectangle {
                width: parent.width * 0.36
                height: 450
                radius: 24
                anchors.centerIn: parent
                z: 2
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 24
                    Layout.alignment: Qt.AlignTop
                    Layout.leftMargin: _padding
                    Layout.rightMargin: _padding

                    // 登录标题
                    Item {
                        Layout.fillWidth: true
                        height: 80
                        Text {
                            text: qsTr("欢迎登录")
                            font.pixelSize: 32
                            color: ThemeManager.primary
                            font.weight: 500
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottom: parent.bottom
                        }
                    }

                    // 用户名输入框
                    WTextField {
                        id: txt_username
                        placeholderText: qsTr("请输入账号")
                        implicitWidth: parent.width - _padding * 2
                        Layout.alignment: Qt.AlignHCenter
                        text: "admin"
                    }

                    // 密码输入框
                    WTextField {
                        id: txt_password
                        placeholderText: qsTr("请输入密码")
                        passwordMode: true
                        implicitWidth: parent.width - _padding * 2
                        Layout.alignment: Qt.AlignHCenter
                        text: "123456"
                    }

                    // 登录按钮
                    WButton {
                        id: btn_login
                        text: qsTr("登 录")
                        theme: WButton.Primary
                        sizePreset: WButton.Medium
                        implicitWidth: parent.width - _padding * 2
                        Layout.alignment: Qt.AlignHCenter
                        onClicked: {
                            // 调用登录Service
                            var username = txt_username.text
                            var password = txt_password.text
                            loginPage.loginRequest(username, password)
                        }
                    }

                    Item {
                        // 占位符，以上元素向顶部对齐
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }

    // 支持拖动窗口
    MouseArea {
        anchors.fill: parent
        property point startMousePos
        onPressed: {
            startMousePos = Qt.point(mouseX, mouseY)
        }
        onPositionChanged: {
            if (pressed) {
                var delta = Qt.point(mouseX - startMousePos.x, mouseY - startMousePos.y)
                // window.x += delta.x
                // window.y += delta.y
            }
        }
    }

}
