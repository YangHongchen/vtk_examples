import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Control {
    id: root

    // ====================== 尺寸预设 ======================
    enum SizePreset { Mini, Small, Medium, Large }
    property int sizePreset: WButton.Medium
    property int customWidth: 0
    property int customHeight: 0

    implicitWidth: {
        switch(sizePreset) {
        case WButton.Mini: return 60
        case WButton.Small: return 108
        case WButton.Medium: return 120
        case WButton.Large: return 160
        default: return customWidth > 0 ? customWidth : 120
        }
    }

    implicitHeight: {
        switch(sizePreset) {
        case WButton.Mini: return 32
        case WButton.Small: return 40
        case WButton.Medium: return 48
        case WButton.Large: return 56
        default: return customHeight > 0 ? customHeight : 48
        }
    }

    // ====================== 主题样式 ======================
    enum Theme { Default, Primary, Success, Warning, Danger }
    property int theme: WButton.Default

    property color backgroundColor: {
        switch(theme) {
        case WButton.Primary: return ThemeManager.primary
        case WButton.Success: return ThemeManager.success
        case WButton.Warning: return ThemeManager.warning
        case WButton.Danger: return ThemeManager.danger
        default: return ThemeManager.colorWhite
        }
    }

    property color borderColor: {
        switch(theme) {
        case WButton.Primary: return ThemeManager.primary
        case WButton.Success: return ThemeManager.success
        case WButton.Warning: return ThemeManager.warning
        case WButton.Danger: return ThemeManager.danger
        default: return ThemeManager.borderColor
        }
    }

    property color textColor: {
        switch(theme) {
        case WButton.Default: return ThemeManager.textNormal
        default: return ThemeManager.textWhite
        }
    }

    property color disabledColor: ThemeManager.textGray
    property int borderRadius: 8
    property int borderWidth: 1

    // ====================== 内容属性 ======================
    property string text: ""
    property string iconSource: ""
    property int _spacing: 8
    property int fontSize: 14
    property int fontWeight: Font.Medium

    // ====================== 状态属性 ======================
    property bool loading: false
    property bool flat: false

    // ====================== 信号 ======================
    signal clicked()
    signal pressed()
    signal released()
    signal hoveredChanged(bool hovered)

    // ====================== 内容布局 ======================
    contentItem: Row {
        spacing: root._spacing
        layoutDirection: Qt.LeftToRight
        anchors.centerIn:  parent

        // 加载状态指示器
        BusyIndicator {
            width: 16
            height: 16
            running: root.loading
            visible: root.loading
            anchors.verticalCenter:  parent.verticalCenter
        }

        // 图标
        Image {
            id: icon
            width: 16
            height: 16
            source: root.iconSource
            visible: root.iconSource  !== "" && !root.loading
            anchors.verticalCenter:  parent.verticalCenter
            sourceSize: Qt.size(width,  height)
        }

        // 文本
        Label {
            id: label
            text: root.text
            font.pixelSize:  root.fontSize
            font.weight:  root.fontWeight
            color: root.enabled  ? root.textColor  : root.disabledColor
            visible: text !== ""
            anchors.verticalCenter:  parent.verticalCenter
        }
    }

    // ====================== 背景样式 ======================
    background: Rectangle {
        implicitWidth: root.implicitWidth
        implicitHeight: root.implicitHeight
        radius: root.borderRadius
        color: root.flat  ? "transparent" :
              (root.enabled  ? root.backgroundColor  : root.disabledColor)
        border.width:  root.flat  ? 0 : root.borderWidth
        border.color:  root.enabled  ? root.borderColor  : root.disabledColor

        // 悬停效果
        opacity: mouseArea.containsMouse  ? 0.9 : 1.0
    }

    // ====================== 交互处理 ======================
    MouseArea {
        id: mouseArea
        anchors.fill:  parent
        hoverEnabled: true
        onClicked: {
            if (!root.loading)  {
                clickAnim.start()
                root.clicked()
            }
        }
        onPressed: {
            if (!root.loading)  {
                pressAnim.start()
                root.pressed()
            }
        }
        onReleased: {
            if (!root.loading)  {
                pressAnim.stop()
                root.released()
            }
        }
        onContainsMouseChanged: {
            root.hoveredChanged(containsMouse)
        }
    }

    // ====================== 动画效果 ======================
    SequentialAnimation {
        id: clickAnim
        PropertyAnimation {
            target: root
            property: "scale"
            from: 1.0
            to: 0.95
            duration: 50
        }
        PropertyAnimation {
            target: root
            property: "scale"
            from: 0.95
            to: 1.0
            duration: 100
        }
    }

    PropertyAnimation {
        id: pressAnim
        target: root
        property: "scale"
        to: 0.98
        duration: 50
    }

    // ====================== 阴影效果 ======================
    layer.enabled:  !root.flat
    layer.effect:  DropShadow {
        transparentBorder: true
        horizontalOffset: 0
        verticalOffset: 2
        color: "#20000000"
        samples: 8
        spread: 0.1
        visible: !root.flat
    }
}
