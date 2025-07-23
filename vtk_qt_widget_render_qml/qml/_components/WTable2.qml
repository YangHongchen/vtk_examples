// WTable.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: tableRoot
    property var model      // 传入 C++ 模型
    property var columns    // [{ title: "医生", prop: "doctor" }, ...]

    Column {
        spacing: 1

        // 表头
        Row {
            spacing: 10
            Repeater {
                model: columns
                delegate: Text {
                    width: 100
                    text: modelData.title
                    font.bold: true
                }
            }
        }

        // 表体
        Repeater {
            model: tableRoot.model   // ✅ 注意此处用 tableRoot.model

            delegate: Row {
                spacing: 10

                property var row: model   // ✅ 取当前行

                Repeater {
                    model: columns
                    delegate: Text {
                        width: 100
                        text: row[modelData.prop] !== undefined ? row[modelData.prop] : "-"
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
