import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Item {
    id: root
    property color color: "transparent" // svg图像颜色
    property string source: "" // svg图像的url
    property string tooltip: "" // 提示文字内容
    property color tooltipColor: "#333333" // 气泡背景色
    property color tooltipTextColor: "white" // 文字颜色

    Image {
        id: img
        anchors.fill:  parent
        source: root.source
        sourceSize {
            width: parent.width
            height: parent.width
        }

        MouseArea {
            id: mouseArea
            anchors.fill:  parent
            hoverEnabled: true
            acceptedButtons: Qt.NoButton // 不处理点击，只悬停
        }
    }

    ColorOverlay {
        anchors.fill:  img
        source: img
        color: root.color  ? root.color  : 'transparent'
    }

    // 气泡提示框
    Popup {
        id: tooltipPopup
        x: mouseArea.mouseX  + 12
        y: mouseArea.mouseY  + 12
        width: Math.min(tooltipContent.implicitWidth  + 20, 100) // 最大宽度100px
        height: tooltipContent.implicitHeight  + 10
        padding: 0
        margins: 0
        closePolicy: Popup.NoAutoClose

        background: Rectangle {
            color: tooltipColor
            radius: 4
            layer.enabled:  true
            layer.effect:  DropShadow {
                transparentBorder: true
                radius: 4
                samples: 9
                color: "#80000000"
            }
        }

        contentItem: Text {
            id: tooltipContent
            text: tooltip
            color: tooltipTextColor
            font.pixelSize:  10
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 150 }
        }

        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 150 }
        }
    }

    // 显示/隐藏控制
    Connections {
        target: mouseArea
        function onContainsMouseChanged() {
            if (mouseArea.containsMouse  && tooltip !== "") {
                tooltipPopup.open()
            } else {
                tooltipPopup.close()
            }
        }
    }

    // 跟随鼠标移动
    Connections {
        target: mouseArea
        function onPositionChanged() {
            if (tooltipPopup.opened)  {
                tooltipPopup.x = mouseArea.mouseX  + 12
                tooltipPopup.y = mouseArea.mouseY  + 12
            }
        }
    }
}
