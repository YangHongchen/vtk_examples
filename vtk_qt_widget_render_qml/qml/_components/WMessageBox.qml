import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects

Popup {
    id: root
    modal: true
    focus: true
    padding: 0
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    // 配置属性
    property string title: qsTr("Message")
    property string text: ""
    property url iconSource: ""
    property color backgroundColor: "#ffffff"
    property color borderColor: "#dddddd"
    property int borderWidth: 1
    property int borderRadius: 8
    property int contentSpacing: 12
    property int buttonSpacing: 8
    property int buttonWidth: 80
    property int buttonHeight: 36

    // 阴影配置
    property bool enableShadow: true
    property color shadowColor: "#80000000"
    property int shadowRadius: 16
    property real shadowVerticalOffset: 4
    property real shadowHorizontalOffset: 0

    // 按钮枚举
    QtObject {
        id: standardButton
        readonly property int ok: 1
        readonly property int cancel: 2
        readonly property int yes: 4
        readonly property int no: 8
    }

    property int standardButtons: StandardButton.Ok

    signal accepted()
    signal rejected()

    background: Rectangle {
        id: backgroundRect
        color: root.backgroundColor
        border.color: root.borderColor
        border.width: root.borderWidth
        radius: root.borderRadius

        layer.enabled: root.enableShadow
        layer.effect: DropShadow {
            color: root.shadowColor
            radius: root.shadowRadius
            samples: radius * 2 + 1
            verticalOffset: root.shadowVerticalOffset
            horizontalOffset: root.shadowHorizontalOffset
            source: backgroundRect
        }
    }

    contentItem: ColumnLayout {
        spacing: root.contentSpacing
        Layout.preferredWidth: 320
        Layout.maximumWidth: 400
        Layout.minimumWidth: 240

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Image {
                source: root.iconSource
                visible: root.iconSource.toString() !== ""
                sourceSize: Qt.size(32, 32)
                Layout.alignment: Qt.AlignTop
            }

            Label {
                text: root.title
                font.bold: true
                font.pixelSize: 16
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            clip: true

            Label {
                text: root.text
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                padding: 4
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: root.buttonSpacing

            Button {
                text: qsTr("OK")
                visible: root.standardButtons & standardButton.ok
                width: root.buttonWidth
                height: root.buttonHeight
                onClicked: {
                    root.accepted()
                    root.close()
                }
            }

            Button {
                text: qsTr("Cancel")
                visible: root.standardButtons & standardButton.cancel
                width: root.buttonWidth
                height: root.buttonHeight
                onClicked: {
                    root.rejected()
                    root.close()
                }
            }

            Button {
                text: qsTr("Yes")
                visible: root.standardButtons & standardButton.yes
                width: root.buttonWidth
                height: root.buttonHeight
                onClicked: {
                    root.accepted()
                    root.close()
                }
            }

            Button {
                text: qsTr("No")
                visible: root.standardButtons & standardButton.no
                width: root.buttonWidth
                height: root.buttonHeight
                onClicked: {
                    root.rejected()
                    root.close()
                }
            }
        }
    }

    // 打开与关闭控制（也可通过 root.open() 调用）
    function open() { root.visible = true }
    function close() { root.visible = false }
}
