import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: root
    width: parent ? parent.width  : 1080
    height: parent ? parent.height  : 900
    color: "#F5F7FA"

    signal connectToSocket(string ip, string port)

    // 背景装饰元素
    Rectangle {
        width: 400; height: 400
        radius: width/2
        color: "#E3F2FD"
        opacity: 0.6
        x: -150; y: -150
    }

    Rectangle {
        width: 300; height: 300
        radius: width/2
        color: "#E8F5E9"
        opacity: 0.6
        x: parent.width  - 100
        y: parent.height  - 100
    }

    // 主卡片
    Rectangle {
        id: container
        width: Math.min(600,  parent.width  * 0.9)
        height: Math.min(700,  parent.height  * 0.8)
        radius: 16
        anchors.centerIn:  parent
        color: "#FFFFFF"
        layer.enabled:  true


        ColumnLayout {
            anchors.fill:  parent
            anchors.margins:  40
            spacing: 30

            // 标题区域
            Item {
                Layout.fillWidth:  true
                Layout.preferredHeight:  80

                Image {
                    source: "qrc:/assets/icon/websocket.svg"  // 替换为你的图标路径
                    width: 48; height: 48
                    anchors.horizontalCenter:  parent.horizontalCenter
                    anchors.top:  parent.top
                }

                Label {
                    text: "硬件Socket连接"
                    font.pixelSize:  24
                    font.weight:  Font.DemiBold
                    color: "#263238"
                    anchors.horizontalCenter:  parent.horizontalCenter
                    anchors.bottom:  parent.bottom
                }
            }

            // 输入区域
            ColumnLayout {
                spacing: 20
                Layout.fillWidth:  true

                Label {
                    text: "IP地址"
                    font.pixelSize:  14
                    color: "#546E7A"
                    Layout.leftMargin:  5
                }

                TextField {
                    id: txt_ip
                    Layout.fillWidth:  true
                    placeholderText: "请输入服务器IP"
                    font.pixelSize:  16
                    leftPadding: 15
                    rightPadding: 15
                    text: "192.168.31.100"
                    background: Rectangle {
                        radius: 8
                        border.width:  1
                        border.color:  txt_ip.activeFocus  ? "#2196F3" : "#B0BEC5"
                    }
                }

                Label {
                    text: "端口号"
                    font.pixelSize:  14
                    color: "#546E7A"
                    Layout.leftMargin:  5
                }

                TextField {
                    id: txt_port
                    Layout.fillWidth:  true
                    placeholderText: "请输入端口号"
                    text: "10086"
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator { bottom: 1; top: 65535 }
                    font.pixelSize:  16
                    leftPadding: 15
                    rightPadding: 15
                    background: Rectangle {
                        radius: 8
                        border.width:  1
                        border.color:  txt_port.activeFocus  ? "#2196F3" : "#B0BEC5"
                    }
                }
            }

            // 连接按钮
            Button {
                id: btnConnect
                Layout.fillWidth:  true
                Layout.preferredHeight:  50
                text: "连接Socket"
                font.pixelSize:  16
                font.weight:  Font.Medium
                Material.background:  "#2196F3"
                Material.foreground:  "white"
                Material.elevation:  2

                onClicked: {
                    if(txt_ip.text  && txt_port.text)  {
                        root.connectToSocket(txt_ip.text,  txt_port.text)
                    } else {
                        btnConnect.text  = "请输入IP和端口"
                        timer.start()
                    }
                }

                background: Rectangle {
                    radius: 8
                    color: btnConnect.down  ? "#1976D2" : "#2196F3"
                }

                Timer {
                    id: timer
                    interval: 2000
                    onTriggered: btnConnect.text  = "连接Socket"
                }
            }

            // 状态指示器
            Item {
                Layout.fillWidth:  true
                Layout.preferredHeight:  50
                Layout.topMargin:  20

                Rectangle {
                    id: statusIndicator
                    width: 12; height: 12
                    radius: 6
                    color: "gray"
                    anchors.verticalCenter:  parent.verticalCenter
                }

                Label {
                    text: "未连接"
                    font.pixelSize:  14
                    color: "#78909C"
                    anchors.left:  statusIndicator.right
                    anchors.leftMargin:  10
                    anchors.verticalCenter:  parent.verticalCenter
                }
            }

            // 底部空白
            Item { Layout.fillHeight:  true }
        }
    }

    // 连接状态变化效果
    states: [
        State {
            name: "CONNECTED"
            PropertyChanges {
                target: statusIndicator
                color: "#4CAF50"
            }
            PropertyChanges {
                target: statusIndicator.children[0]  // 状态文字
                text: "已连接"
            }
        },
        State {
            name: "CONNECTING"
            PropertyChanges {
                target: statusIndicator
                color: "#FFC107"
            }
            PropertyChanges {
                target: statusIndicator.children[0]
                text: "连接中..."
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "*"
            ColorAnimation { duration: 300 }
        }
    ]
}
