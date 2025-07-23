// root.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: root
    width: parent.width
    height: parent.height

    property int pageCount: 0
    property int currentPage: 1
    property int totalCount: 0
    signal pageChanged(int newPage)

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        RowLayout {
            id: rowLayout
            spacing: 8

            // 显示总记录数
            Text {
                text: "共 " + root.totalCount + " 条数据"
                color:ThemeManager.textNormal
                font.pixelSize: 14
                Layout.alignment: Qt.AlignHCenter
            }

            // 上一页按钮
            Item {
                width: 40
                height: 40
                Button {
                    text: "<"
                    palette.buttonText: ThemeManager.textNormal
                    anchors.fill: parent
                    enabled: root.currentPage > 1
                    font.pixelSize: 14
                    onClicked: {
                        root.currentPage--
                        root.pageChanged(root.currentPage)
                    }
                    background: Rectangle {
                        anchors.fill: parent
                        color: "white"
                        border.color: "lightgray"
                        radius: 4
                    }
                }
            }

            // 分页数字部分
            Repeater {
                id: pageRepeater
                model: root.getPagesModel()
                delegate: Item {
                    width: 40
                    height: 40
                    Button {
                        anchors.fill: parent
                        text: modelData.toString()
                        palette.buttonText: (modelData !== "..." && parseInt(modelData) === root.currentPage) ? "#FFF" : "#1A2133"
                        enabled: modelData !== "..."
                        font.pixelSize: 14
                        onClicked: {
                            root.currentPage = parseInt(modelData)
                            root.pageChanged(root.currentPage)
                        }
                        background: Rectangle {
                            anchors.fill: parent
                            color: (modelData !== "..." && parseInt(modelData) === root.currentPage) ? "#0B4DA2" : "white"
                            border.color: "#D8DDEB"
                            radius: 4
                        }
                    }
                }
            }

            // 下一页按钮
            Item {
                width: 40
                height: 40
                Button {
                    text: ">"
                    palette.buttonText: "#1A2133"
                    font.pixelSize: 14
                    anchors.fill: parent
                    enabled: root.currentPage < root.pageCount
                    onClicked: {
                        root.currentPage++
                        root.pageChanged(root.currentPage)
                    }
                    background: Rectangle {
                        anchors.fill: parent
                        color: "white"
                        border.color: "#D8DDEB"
                        radius: 4
                    }
                }
            }
        }
    }

    // 根据总页数和当前页返回数组模型
    function getPagesModel() {
        var pages = []
        if (pageCount <= 7) {
            // 页数较少时显示全部
            for (var i = 1; i <= pageCount; i++) {
                pages.push(i)
            }
        } else {
            // 总是显示第一页
            pages.push(1)
            if (currentPage <= 4) {
                // 当当前页较小，显示连续 1～5
                for (var i = 2; i <= 5; i++) {
                    pages.push(i)
                }
                pages.push("...")
                pages.push(pageCount)
            } else if (currentPage >= pageCount - 3) {
                // 当当前页接近尾部，显示倒数 5 页
                pages.push("...")
                for (var i = pageCount - 4; i < pageCount; i++) {
                    pages.push(i)
                }
                pages.push(pageCount)
            } else {
                // 当前页处于中间部分时，显示当前页及前后各一页
                pages.push("...")
                pages.push(currentPage - 1)
                pages.push(currentPage)
                pages.push(currentPage + 1)
                pages.push("...")
                pages.push(pageCount)
            }
        }
        return pages
    }

    // 当 currentPage 或 pageCount 变化时更新页码模型
    onCurrentPageChanged: pageRepeater.model = getPagesModel()
    onPageCountChanged: pageRepeater.model = getPagesModel()
}
