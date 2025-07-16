// NavigationMenu.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic

Rectangle {
    id:root
    width: 200
    color: "#f0f0f0"

    property var pages: ["仪表盘", "设置", "分析"]
    property int currentIndex: 0

    signal pageChanged(int index)

    Column {
        spacing: 5
        anchors.fill:  parent

        Repeater {
            model: pages
            delegate: Button {
                width: parent.width
                height: 50
                text: modelData
                highlighted: currentIndex === index

                // background: Rectangle {
                //     color: highlighted ? "#6200EE" : "transparent"
                //     opacity: highlighted ? 0.1 : 1
                // }

                contentItem: Text {
                    text: parent.text
                    color: highlighted ? "#6200EE" : "#333"
                    font.bold:  highlighted
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                }

                onClicked: {
                    currentIndex = index
                    root.pageChanged(index)
                    console.log('page changed....',index)
                }
            }
        }
    }

}
