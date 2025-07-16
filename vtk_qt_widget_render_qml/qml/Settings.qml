import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property bool active: false

    onActiveChanged: if (active) loadSettings()

    function loadSettings() {
        console.log(" 加载设置...")
    }

    Grid {
        columns: 2
        spacing: 10

        Label { text: "主题颜色:" }

        Label { text: "语言设置:" }
        ComboBox { model: ["中文", "English"] }
    }
}
