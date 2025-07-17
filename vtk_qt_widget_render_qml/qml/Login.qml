// LoginPage.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: loginRoot
    width: 400
    height: 300

    // 登录请求信号
    signal loginRequest(string username, string password)
    signal loginCanceled(int code)

    Rectangle {
        anchors.fill: parent
        color: "#f7f7f7"
        border.color: "#cccccc"
        radius: 8

        ColumnLayout {
            anchors.fill: parent
            anchors.centerIn: parent
            spacing: 16
            Layout.margins: 16

            // 标题
            Label {
                text: "用户登录"
                font.pointSize: 20
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            // 用户名输入
            TextField {
                id:txt_username
                placeholderText: "用户名"
                font.pointSize: 14
                Layout.fillWidth: true
                background: Rectangle {
                    radius: 6
                    color: "#ffffff"
                    border.color: "#999999"
                }
            }

            // 密码输入
            TextField {
                id:txt_password
                placeholderText: "密码"
                echoMode: TextInput.Password
                font.pointSize: 14
                Layout.fillWidth: true
                background: Rectangle {
                    radius: 6
                    color: "#ffffff"
                    border.color: "#999999"
                }
            }

            // 按钮区域
            RowLayout {
                Layout.fillWidth: true
                spacing: 20
                Button {
                    text: "登录"
                    Layout.fillWidth: true
                    background: Rectangle {
                        color: "#4CAF50"
                        radius: 6
                    }
                    contentItem: Text {
                        text: qsTr("登录")
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        console.log('发起登录请求。')
                        let username = txt_username.text
                        let password = txt_password.text
                        console.log(username, password)
                        loginRoot.loginRequest(username, password)
                    }
                }
                Button {
                    text: "取消"
                    Layout.fillWidth: true
                    background: Rectangle {
                        color: "#f44336"
                        radius: 6
                    }
                    contentItem: Text {
                        text: qsTr("取消")
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        console.log('取消')
                        txt_username.text = "admin"
                        txt_password.text = "123456"

                        loginRoot.loginCanceled(222)
                    }
                }
            }
        }
    }
}
