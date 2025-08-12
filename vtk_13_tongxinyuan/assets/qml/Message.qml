// Toast.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    // 公共属性
    property string text: ""
    property int duration: 2000 // 默认2秒
    property var type: "info"   // primary, warning, info, error
    property var position: "top" // top, center, bottom

    // 内部属性
    property bool showing: false
    property int margin: 20

    // 颜色定义
    readonly property var colors: {
        "primary": "#007bff",
        "warning": "#ffc107",
        "info": "#17a2b8",
        "error": "#dc3545"
    }

    // 显示Toast
    function show() {
        showing = true
        timer.restart()
    }

    // 隐藏Toast
    function hide() {
        showing = false
    }

    width: parent ? parent.width  : 0
    height: parent ? parent.height  : 0
    visible: showing

    Rectangle {
        id: toastRect
        width: Math.min(parent.width  - 2 * margin, 300)
        height: content.height  + 20
        radius: 4
        color: colors[type]
        opacity: showing ? 0.9 : 0

        anchors.horizontalCenter:  parent.horizontalCenter

        y: {
            if (position === "top") return margin
            if (position === "center") return (parent.height  - height) / 2
            if (position === "bottom") return parent.height  - height - margin
            return margin
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }

        Column {
            id: content
            width: parent.width  - 20
            anchors.centerIn:  parent
            spacing: 5

            Label {
                width: parent.width
                text: root.text
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                font.pixelSize:  14
            }
        }
    }

    Timer {
        id: timer
        interval: duration
        onTriggered: hide()
    }
}
