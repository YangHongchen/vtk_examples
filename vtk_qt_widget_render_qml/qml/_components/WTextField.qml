// CustomTextField.qml
import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import "./"

FocusScope {
    id: root
    width: 300
    height: 56
    focus: true

    property alias text: input.text
    property alias placeholderText: placeholder.text
    property color baseColor: "#ffffff"
    property color highlightColor: ThemeManager.primary
    property color errorColor: ThemeManager.danger
    property color borderColor: ThemeManager.borderColor
    property bool hasError: false

    // 密码模式
    property bool passwordMode: false
    property string eyeOpenIcon: "qrc:/assets/images/eye_close.png"
    property string eyeClosedIcon: "qrc:/assets/images/eye_open.png"
    property string clearIcon: "qrc:/assets/images/clear.png"
    property bool _passwordVisible: false     // 密码可见性控制
    property int _padding: 16
    property int leftMargin: 16
    property int rightMargin: 16
    property int topMargin: 12
    property int bottomMargin: 12
    property int fontSize: 16

    Rectangle {
        id: background
        anchors.fill:  parent
        radius: 8
        border.color:  {
            if (hasError) return errorColor
            return input.activeFocus  ? highlightColor : borderColor
        }
        border.width: 1
        Behavior on border.color  {
            ColorAnimation { duration: 150 }
        }
        layer.enabled:  true
    }

    TextInput {
        id: input
        anchors {
            fill: parent
            leftMargin: root.leftMargin
            rightMargin: passwordMode ? 64 : 48
            topMargin: root.topMargin
            bottomMargin: root.bottomMargin
            verticalCenter: parent.verticalCenter
        }
        verticalAlignment: TextInput.AlignVCenter
        font.pixelSize: root.fontSize
        clip: true
        selectByMouse: true
        onActiveFocusChanged: {
            if (text === "") {
                placeholder.opacity  = 1
            } else {
                placeholder.opacity  = 0
            }
        }
        onTextChanged: {
            if (text === "") {
                placeholder.opacity  = 1
            } else {
                placeholder.opacity  = 0
            }
        }
        // 输入模式
        echoMode: {
           if (passwordMode) {
               return _passwordVisible ? TextInput.Normal : TextInput.Password
           }
           return TextInput.Normal
        }
    }

    Text {
        id: placeholder
        anchors {
            left: parent.left
            leftMargin: root.leftMargin
            verticalCenter: parent.verticalCenter
        }
        color: "#D8DDEB"
        font: input.font
        opacity: input.text  === "" ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 100 }
        }
    }

    // 密码可见性切换按钮
    Image {
        id: eyeButton
        source: passwordMode ? (_passwordVisible ? eyeClosedIcon : eyeOpenIcon) : ""
        visible: passwordMode && input.text  !== ""
        width: 20
        height: 20
        anchors {
            verticalCenter: parent.verticalCenter
            right: clearBtn.left
            rightMargin: 8
        }

        MouseArea {
            anchors.fill:  parent
            onClicked: _passwordVisible = !_passwordVisible
        }

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }

    // Clear button
    Image {
        id: clearBtn
        source: root.clearIcon
        visible: input.text  !== "" && input.activeFocus
        width: 20
        height: 20
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 12
        }
        MouseArea {
            anchors.fill:  parent
            onClicked: {
                input.text  = ""
                input.focus  = true
            }
        }
    }
}
