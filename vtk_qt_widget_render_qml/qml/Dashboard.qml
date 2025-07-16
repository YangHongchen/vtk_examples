import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    property bool active: false

    onActiveChanged: if (active) refreshData()

    function refreshData() {
        console.log(" 仪表盘数据刷新...")
    }

    Column {
        spacing: 20
        anchors.centerIn:  parent

        Label {
            text: "主仪表盘"
            font.pixelSize:  24
        }

        Button {
            text: "模拟操作"

        }
    }
}
