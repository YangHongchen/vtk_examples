import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

Item {
    id: root
    width: Math.min(400,  parent ? parent.width  * 0.8 : 400)  // 最大400或屏幕80%
    height: 48
    implicitWidth: Math.min(contentRow.width  + 40, parent ? parent.width  * 0.9 : 400) // 内容宽度+边距
    // 定位到窗口顶层并居中
    parent: Overlay.overlay
    y: -height

    // 可配置属性
    property string message: ""
    property string type: "info"
    property int duration: 2000  // 默认延长到2秒更合理

    // 主题颜色配置
    readonly property var theme: {
        "info":     { "color": "#3498db", "icon": "ⓘ" },
        "success":  { "color": "#2ecc71", "icon": "✓" },
        "warning":  { "color": "#f39c12", "icon": "⚠" },
        "danger":   { "color": "#e74c3c", "icon": "✖" }
    }

    // 动画配置
    states: State {
        name: "visible"
        PropertyChanges {
            target: root
            y: 50  // 距离顶部20像素
            opacity: 1
        }
    }

    transitions: Transition {
        NumberAnimation {
            properties: "y,opacity"
            duration: 300
            easing.type: Easing.OutBack
        }
    }

    // 组件主体
    Rectangle {
        id: notificationBody
        width: parent.width
        height: parent.height
        radius: 4
        color: theme[type].color
        opacity: 0
        layer.enabled:  true

        RowLayout {
            id: contentRow
            anchors.fill: parent
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            spacing: 10
            Item {
                width: 1
                height: parent.height
            }
            // 图标
            Text {
                text: theme[type].icon
                color: "white"
                font {
                    pixelSize: 18
                    bold: type === "danger"
                }
            }
            // 消息文本
            Text {
                text: message
                color: "white"
                width: Math.min(implicitWidth,  root.width  - 80) // 动态宽度限制
                elide: Text.ElideRight
                maximumLineCount: 1
                font.pixelSize:  14
            }
            Item {
                Layout.fillWidth: true
            }
        }

        // 关闭按钮（悬停显示）
        Rectangle {
            width: 24
            height: 24
            radius: 12
            color: ma.containsMouse  ? "#20ffffff" : "transparent"
            anchors {
                right: parent.right
                rightMargin: 8
                verticalCenter: parent.verticalCenter
            }

            Text {
                text: "×"
                color: "white"
                anchors.centerIn:  parent
                font.pixelSize:  18
            }

            MouseArea {
                id: ma
                anchors.fill:  parent
                hoverEnabled: true
                onClicked: hide()
            }
        }
    }

    // 自动隐藏
    Timer {
        id: autoHideTimer
        interval: duration
        onTriggered: hide()
    }

    // 显示控制
    function show(msg="", showType="info") {
        if (msg) message = msg
        type = showType
        state = "visible"
        notificationBody.opacity  = 1
        autoHideTimer.restart()
    }

    function hide() {
        notificationBody.opacity  = 0
        y = -height
        state = ""
    }

    // 快捷方法
    function info(msg) { show(msg, "info") }
    function success(msg) { show(msg, "success") }
    function warning(msg) { show(msg, "warning") }
    function error(msg) { show(msg, "danger") }

    // 初始化隐藏
    Component.onCompleted:  notificationBody.opacity  = 0
}
