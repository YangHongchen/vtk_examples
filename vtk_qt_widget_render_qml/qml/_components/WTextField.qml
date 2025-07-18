import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQml 2.15

Control {
    id: root

    // ====================== 国际化支持 ======================
    property string language:  Qt.locale().name // "en" 或 "zh_CN"

    // 国际化字符串
    QtObject {
        id: i18n
        property string minLengthError: {
            if (root.language  === "zh_CN") return "最小长度为 %1"
            return "Minimum length is %1"
        }
        property string rangeError: {
            if (root.language  === "zh_CN") return "值必须在 %1 和 %2 之间"
            return "Value must be between %1 and %2"
        }
        property string invalidInteger: {
            if (root.language  === "zh_CN") return "无效的整数"
            return "Invalid integer"
        }
        property string invalidNumber: {
            if (root.language  === "zh_CN") return "无效的数字"
            return "Invalid number"
        }
        property string invalidEmail: {
            if (root.language  === "zh_CN") return "无效的邮箱地址"
            return "Invalid email address"
        }
        property string invalidPhone: {
            if (root.language  === "zh_CN") return "无效的电话号码"
            return "Invalid phone number"
        }
        property string invalidFormat: {
            if (root.language  === "zh_CN") return "格式无效"
            return "Invalid format"
        }
    }

    // ====================== 尺寸预设 ======================
    enum SizePreset { Mini, Small, Medium, Large }
    property int sizePreset: WTextField.Medium
    property int customWidth: 0
    property int customHeight: 0

    implicitWidth: {
        switch(sizePreset) {
        case WTextField.Mini: return 100
        case WTextField.Small: return 150
        case WTextField.Medium: return 200
        case WTextField.Large: return 300
        default: return customWidth > 0 ? customWidth : 200
        }
    }

    implicitHeight: {
        switch(sizePreset) {
        case WTextField.Mini: return 24
        case WTextField.Small: return 32
        case WTextField.Medium: return 48
        case WTextField.Large: return 64
        default: return customHeight > 0 ? customHeight : 48
        }
    }

    property int contentPadding: {
        switch (sizePreset) {
        case WTextField.Mini: return 4
        case WTextField.Small: return 6
        case WTextField.Medium: return 8
        case WTextField.Large: return 12
        default: return 8
        }
    }

    // ====================== 文本样式 ======================
    property int fontSize: {
        switch(sizePreset) {
        case WTextField.Mini: return 12
        case WTextField.Small: return 14
        case WTextField.Medium: return 16
        case WTextField.Large: return 20
        default: return 14
        }
    }

    property int placeholderFontSize: Math.max(fontSize  - 2, 10)
    property font fontFamily: Qt.font({
        family: "Arial",
        weight: Font.Normal,
        pixelSize: fontSize
    })

    // ====================== 外观样式 ======================
    property color borderColor: "#999999"
    property color borderColorFocused: "#3a86ff"
    property color errorColor: "#e53935"
    property color hintColor: "#888888"
    property color textColor: "#333333"

    property int borderWidth: 1
    property int borderWidthFocused: 2
    property int borderRadius: 4

    // ====================== 功能属性 ======================
    property bool passwordMode: false
    property bool passwordVisible: false
    property bool clearButtonEnabled: true

    property string placeholderText: ""
    property string text: ""

    // 验证相关
    property string validator: "none" // none, regex, int, double, email, phone
    property string regex: ".*"
    property int decimals: 2
    property int minValue: 0
    property int maxValue: 100
    property int maxLength: 32767
    property int minLength: 0

    // ====================== 信号 ======================
    signal textEdited()
    signal editingFinished()
    signal accepted()
    signal focusChanged(bool focused)

    // ====================== 内部状态 ======================
    QtObject {
        id: internal
        property bool hasFocus: false
        property bool valid: true
        property string lastValidText: ""
        property string validationError: ""
    }

    // ====================== 可视部分 ======================
    contentItem: Rectangle {
        implicitWidth: root.implicitWidth
        implicitHeight: root.implicitHeight
        color: "transparent"
        border {
            width: internal.hasFocus  ? root.borderWidthFocused  : root.borderWidth
            color: !internal.valid  ? root.errorColor  :
                  (internal.hasFocus  ? root.borderColorFocused  : root.borderColor)
        }
        radius: root.borderRadius
        TextField {
            id: textField
            anchors {
                fill: parent
                margins: root.borderWidth
                leftMargin: root.contentPadding
                rightMargin: root.contentPadding
            }
            verticalAlignment: TextInput.AlignVCenter
            color: root.textColor
            font: root.fontFamily
            placeholderText: root.placeholderText
            placeholderTextColor: root.hintColor
            echoMode: root.passwordMode  ?
                (root.passwordVisible  ? TextInput.Normal : TextInput.Password) :
                TextInput.Normal
            maximumLength: root.maxLength
            text: root.text
            selectByMouse: true
            clip: true

            background: Rectangle {
                color: "transparent"
            }
            onTextChanged: {
                root.text  = text
                validateInput()
                root.textEdited()
            }
            onEditingFinished: root.editingFinished()
            onAccepted: root.accepted()
            onFocusChanged: {
                internal.hasFocus  = focus
                root.focusChanged(focus)
            }
        }

        // ====================== 右侧功能按钮 ======================
        // 明文/密文切换按钮
        Button {
            id: togglePasswordButton
            visible: root.passwordMode
            anchors {
                right: parent.right
                rightMargin: root.contentPadding
                verticalCenter: parent.verticalCenter
            }
            width: root.implicitHeight  * 0.7
            height: width
            flat: true
            icon.source:  root.passwordVisible ?
                "qrc:/assets/icons/visibility_off.svg"  :
                "qrc:/assets/icons/visibility.svg"
            icon.color:  "#a19f9d"
            icon.width:  width * 0.6
            icon.height:  height * 0.6
            background: Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height
                radius: width / 2
                color: togglePasswordButton.hovered  ? Qt.rgba(0,  0, 0, 0.1) : "transparent"
            }
            onClicked: {
                root.passwordVisible  = !root.passwordVisible
            }
        }

        // 清除按钮
        Button {
            id: clearButton
            anchors {
                right: root.passwordMode  ? togglePasswordButton.left  : parent.right
                rightMargin: root.contentPadding
                verticalCenter: parent.verticalCenter
            }
            visible: root.clearButtonEnabled  && textField.text.length > 0
            width: root.implicitHeight  * 0.7
            height: width
            flat: true
            icon.source:  "qrc:/assets/icons/close.svg"
            icon.color:  "#a19f9d"
            icon.width:  width * 0.6
            icon.height:  height * 0.6
            background: Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height
                radius: width / 2
                color: clearButton.hovered  ?
                    Qt.rgba(0,  0, 0, 0.1) : "transparent"
            }
            onClicked: {
                textField.clear()
                textField.forceActiveFocus()
            }
        }


        // 错误提示
        Label {
            id: errorLabel
            visible: !internal.valid  && textField.text.length  > 0
            text: internal.validationError
            color: root.errorColor
            font {
                pixelSize: Math.max(root.fontSize  - 4, 10)
                family: root.fontFamily.family
            }
            anchors {
                left: parent.left
                top: parent.bottom
                topMargin: 2
            }
        }
    }

    // ====================== 验证逻辑 ======================
    function validateInput() {
        const input = textField.text
        // 最小长度检查
        if (input.length  < root.minLength)  {
            setInvalid(i18n.minLengthError.arg(root.minLength))
            return
        }
        switch (root.validator)  {
        case "none":
            setValid(input)
            break
        case "regex":
            validateWithRegex(input)
            break

        case "int":
            validateInteger(input)
            break
        case "double":
            validateDouble(input)
            break
        case "email":
            validateEmail(input)
            break
        case "phone":
            validatePhone(input)
            break
        default:
            setValid(input)
        }
    }

    function validateWithRegex(input) {
        const r = new RegExp(root.regex)
        if (r.test(input))  {
            setValid(input)
        } else {
            setInvalid(i18n.invalidFormat)
        }
    }

    function validateInteger(input) {
        if (/^-?\d+$/.test(input)) {
            const val = parseInt(input)
            if (val >= root.minValue  && val <= root.maxValue)  {
                setValid(input)
            } else {
                setInvalid(i18n.rangeError.arg(root.minValue).arg(root.maxValue))
            }
        } else {
            setInvalid(i18n.invalidInteger)
        }
    }

    function validateDouble(input) {
        if (/^-?\d*\.?\d+$/.test(input)) {
            const val = parseFloat(input)
            if (val >= root.minValue  && val <= root.maxValue)  {
                const formatted = val.toFixed(root.decimals)
                setValid(formatted)
                if (input !== formatted) {
                    textField.text  = formatted
                }
            } else {
                setInvalid(i18n.rangeError.arg(root.minValue).arg(root.maxValue))
            }
        } else {
            setInvalid(i18n.invalidNumber)
        }
    }

    function validateEmail(input) {
        const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/
        if (emailRegex.test(input))  {
            setValid(input)
        } else {
            setInvalid(i18n.invalidEmail)
        }
    }

    function validatePhone(input) {
        const phoneRegex = /^\+?[\d\s-]{10,}$/
        if (phoneRegex.test(input))  {
            setValid(input)
        } else {
            setInvalid(i18n.invalidPhone)
        }
    }

    function setValid(value) {
        internal.valid  = true
        internal.lastValidText  = value
        internal.validationError  = ""
    }

    function setInvalid(error) {
        internal.valid  = false
        internal.validationError  = error
    }

    // ====================== 公共方法 ======================
    function clear() {
        textField.text  = ""
        setValid("")
    }

    function forceActiveFocus() {
        textField.forceActiveFocus()
    }

    function isValid() {
        return internal.valid
    }

    function getValidationError() {
        return internal.validationError
    }

    // ====================== 初始化 ======================
    Component.onCompleted:  {
        validateInput()
    }
}
