import QtQuick 2.15

import "../_components"

Rectangle {

    color: "#eaeef1"
    // 表格数据模型
    ListModel {
        id: employeeModel
        ListElement {
            name: "张三"
            age: 28
            department: "研发部"
            salary: 15000
        }
        ListElement {
            name: "李四"
            age: 32
            department: "市场部"
            salary: 12000
        }
        ListElement {
            name: "王五"
            age: 25
            department: "设计部"
            salary: 10000
        }
    }

    // 表格列定义
    property var tableColumns: [
          {
            "title": "姓名",
            "prop": "name",
            "width": 120
        }, {
            "title": "年龄",
            "prop": "age",
            "width": 80
        }, {
            "title": "部门",
            "prop": "department",
            "width": 150
        }, {
            "title": "薪资",
            "width": 150,
            "render": function (row) {
                return "¥" + row.salary.toFixed(2)
            }
        }
    ]
    WTable {
        anchors.fill: parent
        model: employeeModel
        columns: tableColumns
        stripe: true

        onRowClicked: function (index, row) {
            console.log(" 点击了第", index + 1, "行，姓名:", row.name)
        }
    }
}
