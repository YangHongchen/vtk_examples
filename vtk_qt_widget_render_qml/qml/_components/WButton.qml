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
    property int iconWidth: 16
    property int iconHeight: 16

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
        case WButton.Mini: return 24
        case WButton.Small: return 32
        case WButton.Medium: return 40
        case WButton.Large: return 48
        default: return customHeight > 0 ? customHeight : 40
        }
    }


    // ====================== 主题样式 ======================
    enum Theme { Default, Primary, Success, Warning, Danger }
    property int theme: WButton.Default

    // 沉稳配色方案
    property color backgroundColor: {
        switch(theme) {
        case WButton.Primary: return "#0B4DA2"  // 沉稳蓝色
        case WButton.Success: return "#16a34a"  // 沉稳绿色
        case WButton.Warning: return "#d97706"  // 沉稳橙色
        case WButton.Danger: return "#dc2626"   // 沉稳红色
        default: return "#ffffff"               // 白色
        }
    }

    property color borderColor: {
        switch(theme) {
        case WButton.Primary: return "#0B4DA2"
        case WButton.Success: return "#16a34a"
        case WButton.Warning: return "#d97706"
        case WButton.Danger: return "#dc2626"
        default: return "#d1d5db"  // 浅灰色边框
        }
    }

    property color textColor: {
        switch(theme) {
        case WButton.Default: return "#1f2937"  // 深灰色文字
        default: return "#ffffff"               // 白色文字
        }
    }

    property color disabledColor: "#9ca3af"  // 中灰色禁用状态
    property int borderRadius: 8
    property int borderWidth: 1

    // ====================== 内容属性 ======================
    property string text: ""
    property string iconSource: ""
    property int _spacing: 8

    property int fontSize: {
        switch(sizePreset) {
        case WButton.Mini: return 12
        case WButton.Small: return 14
        case WButton.Medium: return 16
        case WButton.Large: return 20
        default: return 14
        }
    }
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
    contentItem: Item {
        implicitWidth: row.implicitWidth
        implicitHeight: row.implicitHeight
        anchors.centerIn:  parent

        Row {
            id: row
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
                width: root.iconWidth
                height: root.iconHeight
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
    }

    // ====================== 背景样式 ======================
    background: Rectangle {
        implicitWidth: root.implicitWidth
        implicitHeight: root.implicitHeight
        radius: root.borderRadius
        color: root.flat  ? "transparent" :
              (root.enabled  ? root.backgroundColor  : Qt.darker(root.disabledColor,  1.2))
        border.width:  root.flat  ? 0 : root.borderWidth
        border.color:  root.enabled  ? root.borderColor  : root.disabledColor

        // 悬停效果
        opacity: mouseArea.containsMouse  && root.enabled  ? 0.9 : 1.0

        // 按下效果
        scale: mouseArea.pressed  && root.enabled  ? 0.98 : 1.0
        Behavior on scale { NumberAnimation { duration: 50 } }
    }

    // ====================== 交互处理 ======================
    MouseArea {
        id: mouseArea
        anchors.fill:  parent
        hoverEnabled: true
        onClicked: {
            if (!root.loading  && root.enabled)  {
                clickAnim.start()
                root.clicked()
            }
        }
        onPressed: {
            if (!root.loading  && root.enabled)  {
                root.pressed()
            }
        }
        onReleased: {
            if (!root.loading  && root.enabled)  {
                root.released()
            }
        }
        onContainsMouseChanged: {
            root.hoveredChanged(containsMouse)
        }
    }

    // ====================== 点击动画 ======================
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

    // ====================== 阴影效果 ======================
    layer.enabled:  !root.flat  && root.enabled
    layer.effect:  DropShadow {
        transparentBorder: true
        horizontalOffset: 0
        verticalOffset: root.enabled  ? 2 : 0
        color: "#20000000"
        radius: root.enabled  ? 4 : 0
        samples: 8
        spread: 0.1
        Behavior on verticalOffset { NumberAnimation { duration: 100 } }
        Behavior on radius { NumberAnimation { duration: 100 } }
    }
}
