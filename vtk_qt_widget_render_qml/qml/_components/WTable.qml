import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./table"

Item {
    id: root
    width: 800
    height: 600

    // === 公共属性 ===
    property alias model: tableRepeater.model
    property var columns: []
    property int rowHeight: 48
    property int headerHeight: 56
    property color headerColor: "#f5f7fa"
    property color borderColor: "#ebeef5"
    property color hoverColor: "#f5f7fa"
    property color selectedColor: "#ecf5ff"
    property color textColor: "#606266"
    property color headerTextColor: "#909399"
    property int fontSize: 14
    property int headerFontSize: 14
    property bool stripe: true
    property color stripeColor: "#fafafa"
    property int currentRow: -1

    signal rowClicked(int rowIndex, var rowData)
    signal rowDoubleClicked(int rowIndex, var rowData)

    function getRowData(index) {
        if (!model || index < 0) return {}
        if (typeof model.get === "function") return model.get(index)
        return model[index] || {}
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
        border.color: borderColor
        border.width: 1
        clip: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // 表头 ==========
            RowLayout {
                id: headerRow
                height: headerHeight
                width: parent.width
                spacing: 0
                Repeater {
                    model: columns
                    delegate: TableCell {
                        Layout.preferredWidth: modelData.width || 150
                        Layout.fillWidth: index === columns.length - 1
                        height: headerHeight
                        content: modelData.title || "-"
                        backgroundColor: headerColor
                        textColor: headerTextColor
                        fontBold: true
                        horizontalAlignment: Text.AlignHCenter
                        showTopBorder: true
                        showBottomBorder: true
                        showLeftBorder: index === 0
                        showRightBorder: index === columns.length - 1
                    }
                }
            }

            // 表体 ==========
            ScrollView {
                id: scrollView
                Layout.fillWidth: true
                Layout.fillHeight: true
                ScrollBar.vertical.policy: ScrollBar.AsNeeded
                spacing: 0

                Label {
                    id: emptyHint
                    anchors.centerIn:  parent
                    text: "暂无数据"
                    color: "#909399"
                    font.pixelSize:  16
                    visible: tableRepeater.count === 0
                }

                Column {
                    id: tableContent
                    width: parent.width
                    spacing: 0

                    Repeater {
                        id: tableRepeater
                        model: []
                        delegate: Item {  // 绘制行
                            id: rowDelegate
                            width: parent.width
                            height: rowHeight
                            visible: model !== undefined  // 确保有数据时才显示
                            Rectangle {
                                anchors.fill: parent
                                color: {
                                    if (index === currentRow) return selectedColor
                                    if (mouseArea.containsMouse) return hoverColor
                                    if (stripe && index % 2 === 1) return stripeColor
                                    return "white"
                                }
                                border.color: borderColor
                                border.width: 0
                            }
                            RowLayout {
                                anchors.fill: parent
                                spacing: 0
                                Repeater {   // 绘制列
                                    model: columns
                                    delegate: TableCell {
                                        Layout.preferredWidth: modelData.width || 150
                                        Layout.fillWidth: index === columns.length - 1
                                        height: parent.height
                                        backgroundColor: "transparent"
                                        textColor: textColor
                                        horizontalAlignment: Text.AlignLeft
                                        content: {
                                            const row = getRowData(rowDelegate.index)
                                            if (modelData.render)
                                                return modelData.render(row)
                                            return modelData.prop && row[modelData.prop] !== undefined
                                                ? row[modelData.prop]
                                                : "-"
                                        }
                                        showTopBorder: false
                                        showBottomBorder: true
                                        showLeftBorder: index === 0
                                        showRightBorder: index === columns.length - 1
                                    }
                                }
                            }

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    currentRow = index
                                    rowClicked(index, getRowData(index))
                                }
                                onDoubleClicked: {
                                    rowDoubleClicked(index, getRowData(index))
                                }
                            }
                        }
                    }
                }
            }

        }
    }
}
