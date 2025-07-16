import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    width: 200
    color: "#f5f5f5"  // 微调背景色

    property var pages: ["仪表盘", "设置", "分析", "VTK"]
    property int currentIndex: 0
    signal pageChanged(int index)

    Column {
        spacing: 1  // 缩小间距用分割线替代
        anchors.fill:  parent

        Repeater {
            model: pages
            delegate: Rectangle {  // 改用Rectangle作为底层
                width: parent.width
                height: 48  // 规范高度
                color: {
                    currentIndex === index ? "#e3f2fd" :  // 选中态浅蓝
                    mouseArea.containsMouse  ? "#eeeeee" : "transparent"  // 悬停态
                }

                Text {
                    text: modelData
                    color: currentIndex === index ? "#1976d2" : "#333333"  // 颜色调整
                    font {
                        pixelSize: 14
                        bold: currentIndex === index
                    }
                    anchors {
                        left: parent.left
                        leftMargin: 24  // 规范间距
                        verticalCenter: parent.verticalCenter
                    }
                }

                // 底部边框
                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#e0e0e0"
                    anchors.bottom:  parent.bottom
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill:  parent
                    hoverEnabled: true
                    onClicked: {
                        currentIndex = index
                        root.pageChanged(index)
                    }
                }
            }
        }
    }
}
