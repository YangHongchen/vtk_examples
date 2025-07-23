import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    width: 200
    height: 300
    color: "#4DFFFFFF"  // 30%透明白色
    radius: 8
    border.color:  "#80FFFFFF"
    border.width:  1

    Column {
        anchors.fill:  parent
        anchors.margins:  12
        spacing: 8

        Label {
            text: "测量步骤"
            font.bold:  true
            font.pixelSize:  14
            color: "#333333"
        }

        Repeater {
            model: [
                "1. 选择起点",
                "2. 选择终点",
                "3. 确认测量",
                "4. 查看结果"
            ]

            delegate: Label {
                text: modelData
                font.pixelSize:  12
                color: index === currentStep ? "#0066CC" : "#666666"

                // 步骤指示器
                Rectangle {
                    width: 6
                    height: 6
                    radius: 3
                    color: index === currentStep ? "#0066CC" : "#CCCCCC"
                    anchors.verticalCenter:  parent.verticalCenter
                    anchors.right:  parent.left
                    anchors.rightMargin:  -14
                }
            }
        }
    }

    // 当前步骤状态 (可从C++绑定)
    property int currentStep: 0

    // 点击穿透处理
    MouseArea {
        anchors.fill:  parent
        acceptedButtons: Qt.AllButtons
        onPressed: mouse => mouse.accepted  = false
        onReleased: mouse => mouse.accepted  = false
    }


}
