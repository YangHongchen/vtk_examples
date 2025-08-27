import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    anchors.fill: parent

    // 顶部工具栏
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        color: "#66000000"   // 半透明黑色
        Row {
            anchors.centerIn: parent
            spacing: 20
            Button { text: "Open" }
            Button { text: "Save" }
            Button { text: "Exit" }
        }
    }

    // 右侧控制面板
    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 280
        color: "#44FFFFFF"   // 半透明白色
        Column {
            anchors.margins: 20
            anchors.fill: parent
            spacing: 15
            Label { text: "控制面板"; font.pixelSize: 20 }
            Button { text: "旋转模型" }
            Button { text: "重置视角" }
            Button { text: "开始动画" }
        }
    }
}
