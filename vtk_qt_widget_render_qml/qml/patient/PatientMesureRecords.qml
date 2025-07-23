import QtQuick 2.15

import "../_components"

Rectangle {
    id:root
    color: "#eaeef1"
    property var tableModel         // ✅ 可传入任意模型对象（如 QAbstractListModel 实例）

    // 表格列定义
    property var tableColumns: [
          {
            "title": "编号",
            "prop": "id",
            "width": 120
        }, {
            "title": "医生",
            "prop": "doctor",
            "width": 80
        }, {
            "title": "备注",
            "prop": "comments",
            "width": 150
        },
    ]
    WTable {
        anchors.fill: parent
        model:  root.tableModel
        columns: root.tableColumns
        stripe: false
        onRowClicked: function (index, row) {
            console.log(" 点击了第", index + 1, "行，姓名:", row.name)
        }
    }
}
