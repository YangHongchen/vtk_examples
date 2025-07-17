// CustomButton.qml
import QtQuick
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts

Rectangle {
    id: root

    // ===== 主题配置 =====
    property string theme: "default"  // primary/success/warning/danger
    property var themeColors: ({
        "default":   { bg: ThemeManager.colorWhite, border: ThemeManager.borderColor, text: ThemeManager.textNormal },
        "primary":   { bg: ThemeManager.primary, border: ThemeManager.primary, text: ThemeManager.textWhite },
        "success": { bg: ThemeManager.success, border: ThemeManager.success, text: ThemeManager.textWhite },
        "warning":  { bg: ThemeManager.warning, border: ThemeManager.warning, text: ThemeManager.textWhite },
        "danger":  { bg: ThemeManager.danger, border: ThemeManager.danger, text:ThemeManager.textWhite},
    })

    // ===== 尺寸预设 =====
    property string size: "medium"  // mini/small/medium/large
    readonly property var sizeMap: ({
        "mini":   { w: 60,  h: 32, r: 4 },
        "small":  { w: 108, h: 40, r: 8 },
        "medium": { w: 120, h: 48, r: 8 },
        "large":  { w: 160, h: 56, r: 8 }
    })

    // ===== 自定义属性 =====
    property string text: "按钮"
    property int fontSize: 14
    property int fontWeight: 500
    property string icon: "" // 按钮图标
    property color iconColor: themeColors[theme].text

    // ===== 信号定义 =====
    signal clicked()
    signal pressed()
    signal released()

    width: sizeMap[size]?.w || implicitWidth
    height: sizeMap[size]?.h || implicitHeight
    radius: sizeMap[size]?.r || 0
    color: root.enabled?themeColors[theme].bg: ThemeManager.textGray
    border { width: 1; color:  root.enabled ? themeColors[theme].border: ThemeManager.textGray }

    layer.enabled:  true
    // layer.effect: DropShadow {
    //     transparentBorder: true
    //     horizontalOffset: 0
    //     verticalOffset: 2
    //     color: "#20000000"
    //     samples: 8
    //     spread: 0.1
    // }

    RowLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
        }
        WImageSvg {
            width: root.icon ? 16 : 0
            height: root.icon ? 16 : 0
            source: root.icon
            Layout.alignment: Qt.AlignCenter| Qt.AlignVCenter
            visible: root.icon ? true:false
            color: root.iconColor ? root.iconColor : themeColors[theme].text
        }
        Text {
            id: btnText
            text: root.text
            color: themeColors[theme].text
            font.pixelSize:  fontSize
            font.weight: fontWeight
            Layout.alignment: Qt.AlignCenter|Qt.AlignVCenter
        }
        Item {
            Layout.fillWidth: true  // 占位符，右侧自动填充
        }
    }

    MouseArea {
        anchors.fill:  parent
        hoverEnabled: true
        onClicked: {
            clickAnim.restart()
            root.clicked()
        }
        onPressed: {
            pressAnim.start()
            root.pressed()
        }
        onReleased: {
            pressAnim.stop()
            root.released()
        }
        // 悬停动画
        onEntered: hoverAnim.start()
        onExited: hoverAnim.stop()
    }

    // ===== 动画定义 =====
    PropertyAnimation {  // 鼠标悬浮动画
        id: hoverAnim
        target: root
        property: "opacity"
        from: 1.0
        to: 0.85
        duration: 200
    }

    SequentialAnimation {  // 鼠标点击动画
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

    PropertyAnimation {   // 鼠标按下动画
        id: pressAnim
        target: root
        property: "scale"
        to: 0.98
        duration: 50
    }
}
