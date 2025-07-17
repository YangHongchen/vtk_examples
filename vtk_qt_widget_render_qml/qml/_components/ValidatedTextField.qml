// ValidatedTextField.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15
import QtQuick.Controls.Material 2.15  //（可选）美化样式

Item {
    id: root
    width: 300
    height: 60

    // ✅ 外部可配置属性
    property alias text: textField.text
    property string placeholder: "请输入"
    property bool isPassword: false
    property int maxLength: 100
    property string regexPattern: ".*"   // 默认允许任意内容
    property string errorMessage: "格式不正确"
    property bool showError: !validator.acceptableInput

    signal textChanged(string newText)

    // ✅ 正则校验器
    RegularExpressionValidator  {
        id: validator
        regularExpression: new RegExp(root.regexPattern)
    }

    // ✅ 输入框
    TextField {
        id: textField
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 36
        placeholderText: root.placeholder
        echoMode: root.isPassword ? TextInput.Password : TextInput.Normal
        validator: validator
        maximumLength: root.maxLength
        selectByMouse: true

        // ✅ 样式变化：错误时变红
        background: Rectangle {
            implicitHeight: 36
            radius: 6
            color: textField.enabled ? (root.showError ? "#ffe5e5" : "white") : "#eee"
            border.color: root.showError ? "red" : "#999"
            border.width: 1
        }

        onTextChanged: {
            root.textChanged(text)
        }
    }

    // ✅ 错误提示
    Text {
        visible: root.showError
        anchors.top: textField.bottom
        anchors.left: parent.left
        anchors.topMargin: 4
        color: "red"
        font.pixelSize: 12
        text: root.errorMessage
    }
}
