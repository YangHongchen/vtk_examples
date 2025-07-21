// Table.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./table"

Item {
    id: root
    width: 800
    height: 600

    // === 公共属性 ===
    property alias model: tableModel.model
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

    // === 私有属性 ===
    property int visibleRows: Math.floor(height / rowHeight)
    property int contentHeight: model ? model.count * rowHeight : 0
    property int verticalScrollOffset: scrollView.contentY

    function getFirstVisibleRow() {
        return Math.max(0, Math.floor(verticalScrollOffset / rowHeight))
    }

    function getLastVisibleRow() {
        return Math.min(model ? model.count - 1 : 0, getFirstVisibleRow() + visibleRows)
    }

    function scrollToRow(rowIndex) {
        if (!model || rowIndex < 0 || rowIndex >= model.count) return
        const targetY = rowIndex * rowHeight
        const maxY = contentHeight - height
        scrollView.ScrollBar.vertical.position = Math.min(targetY / maxY, 1)
    }

    Rectangle {
        id: tableContainer
        anchors.fill: parent
        color: "white"
        border.color: borderColor
        border.width: 1
        clip: true

        // 表头
        RowLayout {
            id: headerRow
            width: parent.width - (scrollView.ScrollBar.vertical.visible ? scrollView.ScrollBar.vertical.width : 0)
            height: headerHeight
            spacing: 0
            Repeater {
                model: columns
                delegate: TableCell {
                    Layout.preferredWidth: modelData.width || 150
                    Layout.fillWidth: index === columns.length - 1
                    height: parent.height
                    content: modelData.title || "-"

                    backgroundColor: headerColor
                    textColor: headerTextColor
                    // fontPixelSize: headerFontSize
                    fontBold: true
                    horizontalAlignment: Text.AlignHCenter

                    showTopBorder: true
                    showBottomBorder: true
                    showLeftBorder: index === 0
                    showRightBorder: index === columns.length - 1
                }
            }
        }

        // 内容区域
        ScrollView {
            id: scrollView
            anchors.top: headerRow.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            contentWidth: width
            contentHeight: contentHeight

            Item {
                id: contentItem
                width: parent.width
                height: contentHeight

                Repeater {
                    id: tableModel
                    model: root.model
                    delegate: Item {
                        id: rowDelegate
                        width: parent.width
                        height: rowHeight
                        y: index * rowHeight
                        visible: index >= getFirstVisibleRow() && index <= getLastVisibleRow()

                        Rectangle {
                            anchors.fill: parent
                            color: {
                                if (index === currentRow) return selectedColor
                                if (mouseArea.containsMouse) return hoverColor
                                if (stripe && index % 2 === 1) return stripeColor
                                return "white"
                            }
                            border.color: borderColor
                            border.width: 1
                        }

                        RowLayout {
                            anchors.fill: parent
                            spacing: 0
                            Repeater {
                                model: columns
                                delegate: TableCell {
                                    Layout.preferredWidth: modelData.width || 150
                                    Layout.fillWidth: index === columns.length - 1
                                    height: parent.height

                                    backgroundColor: "transparent"
                                    textColor: textColor
                                    // fontPixelSize: fontSize
                                    horizontalAlignment: Text.AlignLeft
                                    content: {
                                        const rowData = root.model.get(rowDelegate.index)
                                        if (modelData.render)
                                            return modelData.render(rowData)
                                        return modelData.prop && rowData[modelData.prop] !== undefined
                                            ? rowData[modelData.prop]
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
                                rowClicked(index, root.model.get(index))
                            }
                            onDoubleClicked: {
                                rowDoubleClicked(index, root.model.get(index))
                            }
                        }
                    }
                }
            }
        }
    }
}
