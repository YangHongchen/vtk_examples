// TableCell.qml
import QtQuick 2.15

Rectangle {
    id: root
    width: 100
    height: 40
    color: backgroundColor

    // === 属性 ===
    property color backgroundColor: "white"

    property color borderTopColor: "#cccccc"
    property color borderRightColor: "#cccccc"
    property color borderBottomColor: "#cccccc"
    property color borderLeftColor: "#cccccc"

    property int borderTopWidth: 1
    property int borderRightWidth: 1
    property int borderBottomWidth: 1
    property int borderLeftWidth: 1

    // 控制哪些边框显示
    property bool showTopBorder: true
    property bool showRightBorder: true
    property bool showBottomBorder: true
    property bool showLeftBorder: true

    property color textColor: "#606266"
    property int fontSize: 14
    property bool fontBold: false
    property string fontFamily: "Microsoft YaHei"

    property int horizontalAlignment: Text.AlignLeft
    property alias content: textItem.text

    // === 边框 ===
    Rectangle {
        id: topBorder
        visible: showTopBorder && borderTopWidth > 0
        color: borderTopColor
        height: borderTopWidth
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    Rectangle {
        id: rightBorder
        visible: showRightBorder && borderRightWidth > 0
        color: borderRightColor
        width: borderRightWidth
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }

    Rectangle {
        id: bottomBorder
        visible: showBottomBorder && borderBottomWidth > 0
        color: borderBottomColor
        height: borderBottomWidth
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
    }

    Rectangle {
        id: leftBorder
        visible: showLeftBorder && borderLeftWidth > 0
        color: borderLeftColor
        width: borderLeftWidth
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
    }

    // === 内容区 ===
    Item {
        id: contentArea
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            // 内边距，保证内容不被边框遮挡
            topMargin: borderTopWidth
            rightMargin: borderRightWidth
            bottomMargin: borderBottomWidth
            leftMargin: borderLeftWidth
        }

        Text {
            id: textItem
            anchors.verticalCenter: parent.verticalCenter

            // 水平对齐处理
            anchors.left: horizontalAlignment === Text.AlignLeft ? parent.left : undefined
            anchors.leftMargin: horizontalAlignment === Text.AlignLeft ? 8 : 0

            anchors.right: horizontalAlignment === Text.AlignRight ? parent.right : undefined
            anchors.rightMargin: horizontalAlignment === Text.AlignRight ? 8 : 0

            anchors.horizontalCenter: horizontalAlignment === Text.AlignHCenter ? parent.horizontalCenter : undefined

            text: ""
            color: textColor
            font.pixelSize: fontSize
            font.bold: fontBold
            font.family: fontFamily
            elide: Text.ElideRight
            wrapMode: Text.NoWrap
        }
    }
}
