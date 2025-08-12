import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import com.dyt  1.0

Window {
    width: 900
    height: 600
    visible: true
    color: '#f1f2f6'
    title: "Widget嵌入演示(点击下方按钮)"

    Rectangle {
        id: widgetHost
        width: 400
        height: 300
        color: "transparent"  // 初始透明
        border {
            color: "red"
            width: 2
        }
        anchors.centerIn:  parent

        // 增加状态提示文本
        Text {
            id: stateText
            anchors.centerIn:  parent
            text: "等待嵌入Widget..."
            font.pixelSize:  20
            color: "red"
        }

        Connections {
            target: widgetHost
            function onXChanged() { widgetContainer.updateGeometry(widgetHost)  }
            function onYChanged() { widgetContainer.updateGeometry(widgetHost)  }
            function onWidthChanged() { widgetContainer.updateGeometry(widgetHost)  }
            function onHeightChanged() { widgetContainer.updateGeometry(widgetHost)  }
        }
    }

    Button {
        id: embedButton
        text: "嵌入Widget"
        font.pixelSize:  16
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            bottomMargin: 50
        }

        // 按钮样式
        background: Rectangle {
            color: parent.down  ? "#4CAF50" : "#8BC34A"
            radius: 5
        }
        contentItem: Text {
            text: parent.text
            font: parent.font
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            if(widgetContainer.embedWidget(widgetHost))  {
                stateText.text  = "Widget已嵌入✓"
                stateText.color  = "green"
                embedButton.text  = "已嵌入(再次点击移除)"
            } else {
                stateText.text  = "嵌入失败!"
                stateText.color  = "red"
            }

            // 添加动画效果
            blinkAnimation.start()
        }
    }

    // 闪烁动画
    SequentialAnimation {
        id: blinkAnimation
        PropertyAnimation {
            target: widgetHost
            property: "border.width"
            to: 5
            duration: 100
        }
        PropertyAnimation {
            target: widgetHost
            property: "border.width"
            to: 2
            duration: 100
        }
    }
}
