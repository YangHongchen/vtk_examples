// ImagePreviewer.qml
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15


Popup {
    id: root
    width: parent.width  * 0.6
    height: parent.height  * 0.6
    x: (parent.width  - width) / 2
    y: (parent.height  - height) / 2
    modal: true
    focus: true

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    property alias source: image.source
    property real scaleFactor: 1.0
    property real rotationAngle: 0
    property string title: qsTr("图片预览")

    // 自定义按钮组件
    component CustomButton: Rectangle {
        property alias text: btnText.text
        property alias fontPixelSize: btnText.font.pixelSize
        signal clicked
        width: 40
        height: 40
        // radius: 20
        color: btnMouseArea.containsPress ? "#0B4DA2" :
            (btnMouseArea.containsMouse ? "#0B4DA2" : Qt.lighter( "#0B4DA2", 1.1))
        Behavior on color {
            ColorAnimation { duration: 150 }
        }
        Text {
            id: btnText
            anchors.centerIn:  parent
            color: "white"
            font.pixelSize:  16
        }
        MouseArea {
            id: btnMouseArea
            anchors.fill:  parent
            hoverEnabled: true
            onClicked: parent.clicked()
        }
    }

    // 背景和阴影
    background: Item {
        // color: "#1E1E1E"
        // radius: 12
        layer.enabled:  true
    }

    // 标题栏
    Rectangle {
        id: titleBar
        width: parent.width
        height: 48
        color: "#0B4DA2"
        // radius: 12
        anchors.top:  parent.top
        // 标题文本
        Text {
            text: root.title
            color: "white"
            font.pixelSize:  16
            font.weight:  Font.Medium
            anchors.verticalCenter:  parent.verticalCenter
            anchors.left:  parent.left
            anchors.leftMargin:  16
        }
        // 关闭按钮
        CustomButton {
            text: "✕"
            anchors.right:  parent.right
            anchors.rightMargin:  8
            anchors.verticalCenter:  parent.verticalCenter
            onClicked: root.close()
        }
    }

    // 图片显示区域
    Rectangle {
        id: imageContainer
        anchors {
            top: titleBar.bottom
            left: parent.left
            right: parent.right
            bottom: controlBar.top
            // margins: 8
        }
        color: "#1E1E1E"
        clip: true

        // 加载状态指示
        Rectangle {
            anchors.fill:  parent
            color: "#1E1E1E"
            visible: image.status  !== Image.Ready
            Column {
                anchors.centerIn:  parent
                spacing: 10
                Rectangle {
                    width: 40
                    height: 40
                    radius: 20
                    color: "#3A3A3A"
                    anchors.horizontalCenter:  parent.horizontalCenter
                    RotationAnimation on rotation {
                        from: 0
                        to: 360
                        duration: 1000
                        loops: Animation.Infinite
                        running: true
                    }
                }
                Text {
                    text: image.status  === Image.Loading ? "加载中..." :
                         image.status  === Image.Error ? "加载失败" : ""
                    color: "#AAAAAA"
                    font.pixelSize:  14
                    anchors.horizontalCenter:  parent.horizontalCenter
                }
            }
        }

        Flickable {
            id: flickable
            anchors.fill:  parent
            contentWidth: imageContainer.width
            contentHeight: imageContainer.height
            boundsBehavior: Flickable.DragAndOvershootBounds
            interactive: image.status  === Image.Ready
            Image {
                id: image
                width: Math.min(implicitWidth  * scaleFactor,  flickable.width  * 2)
                height: Math.min(implicitHeight  * scaleFactor,  flickable.height  * 2)
                anchors.centerIn:  parent
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                cache: true
                smooth: true
                mipmap: true
                transform: Rotation {
                    origin.x: image.width  / 2
                    origin.y: image.height  / 2
                    angle: rotationAngle
                }
                Behavior on width { NumberAnimation { duration: 100 } }
                Behavior on height { NumberAnimation { duration: 100 } }
            }

            // 捏合缩放支持
            PinchArea {
                anchors.fill:  parent
                pinch.target:  image
                pinch.minimumScale:  0.1
                pinch.maximumScale:  10
                pinch.dragAxis:  Pinch.XAndYAxis
                onPinchFinished: {
                    flickable.returnToBounds()
                }
            }

            // 鼠标交互支持
            MouseArea {
                anchors.fill:  parent
                enabled: image.status  === Image.Ready
                drag.target:  image
                drag.axis:  Drag.XAndYAxis
                drag.minimumX:  -image.width  * 0.5 + flickable.width  * 0.5
                drag.maximumX:  image.width  * 0.5 - flickable.width  * 0.5
                drag.minimumY:  -image.height  * 0.5 + flickable.height  * 0.5
                drag.maximumY:  image.height  * 0.5 - flickable.height  * 0.5
                onDoubleClicked: {
                    if (scaleFactor === 1.0) {
                        scaleFactor = 2.0
                    } else {
                        scaleFactor = 1.0
                        image.x = 0
                        image.y = 0
                    }
                }
                onWheel: {
                    var zoomFactor = 1.0 + wheel.angleDelta.y  / 1200.0
                    scaleFactor = Math.min(Math.max(scaleFactor  * zoomFactor, 0.1), 10)
                }
            }
        }
    }

    // 控制栏
    Rectangle {
        id: controlBar
        width: parent.width
        height: 64
        // color: "#2A2A2A"
        // radius: 12
        anchors.bottom:  parent.bottom
        Row {
            anchors.centerIn:  parent
            spacing: 16
            // 缩小按钮
            CustomButton {
                text: "-"
                fontPixelSize: 20
                onClicked: scaleFactor = Math.max(0.1,  scaleFactor - 0.1)
            }
            // 重置按钮
            CustomButton {
                text: "↻"
                onClicked: {
                    scaleFactor = 1.0
                    rotationAngle = 0
                    image.x = 0
                    image.y = 0
                }
            }
            // 放大按钮
            CustomButton {
                text: "+"
                fontPixelSize: 20
                onClicked: scaleFactor = Math.min(10,  scaleFactor + 0.1)
            }
            // 旋转按钮
            CustomButton {
                text: "⟳"
                onClicked: rotationAngle = (rotationAngle + 90) % 360
            }
        }
    }
}
