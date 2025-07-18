import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item {
    id: datePicker

    visible: false

    // 外部提供默认日期（支持字符串或 date 类型）
    property variant defaultDate: undefined

    // 外部触发按钮作为定位参考，用于弹窗定位
    property Item anchorItem

    // 当前年份
    property int currentYear :new Date().getFullYear()

    // 当前选中日期，会在弹窗中更新并触发信号
    property date selectedDate: {
        if (typeof defaultDate === "string") {
            let parsed = Date.fromLocaleString(Qt.locale(), defaultDate, "yyyy-MM-dd")
            return parsed && parsed.getTime() > 0 ? parsed : new Date()
        } else if (typeof defaultDate === "object" && defaultDate.getTime) {
            return defaultDate
        } else {
            return new Date()
        }
    }

    // 日期主题颜色（选中时使用）
    property color themeColor: "#2196F3"

    // 日期单元格大小（宽高一致实现正圆）
    property int cellSize: 36
    property int cellRadius: cellSize / 2

    // 星期与月份国际化支持
    property var weekdayNames: Qt.locale().name.startsWith("zh")
        ? ["一", "二", "三", "四", "五", "六", "日"]
        : ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]

    property var monthNames: Qt.locale().name.startsWith("zh")
        ? ["1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月"]
        : ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]

    // 日期选择完成后触发的信号（选中或清除）
    signal dateSelected(date newDate)

    // 新增日期格式属性
    property string dateFormat: "yyyy-MM-dd"

    function formatDate(date) {
        if (!date) return ""
        const year = date.getFullYear()
        const month = (date.getMonth()  + 1).toString().padStart(2, '0')
        const day = date.getDate().toString().padStart(2,  '0')
        const formattedDate =  `${year}-${month}-${day}`
        console.log('格式化后的 日期', formattedDate)
        return formattedDate
    }

    Popup {
        id: calendarPopup
        visible: datePicker.visible
        closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
        padding: 8
        width: 300
        height: 340

        // 正确定位：紧贴 anchorItem 右侧并顶部对齐
        x: anchorItem ? anchorItem.mapToItem(null, anchorItem.x, 0).x : 0
        y: anchorItem ? anchorItem.mapToItem(null,  0, anchorItem.height  + 4).y : 0


        background: Rectangle {
            color: "white"
            radius: 8
            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 2
                verticalOffset: 2
                radius: 6
                samples: 12
                color: "#40000000"
            }
        }

        onClosed: datePicker.visible = false

        ColumnLayout {
            anchors.fill: parent
            spacing: 6

            // 顶部导航：月份与年份选择
            RowLayout {
                Layout.fillWidth: true
                spacing: 6

                Button {
                    text: "<"
                    font.pixelSize: 12
                    Layout.preferredWidth: 26
                    onClicked: {
                        if (calendar.month === 0) {
                            calendar.month = 11
                            calendar.year -= 1
                        } else {
                            calendar.month -= 1
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }

                ComboBox {
                    model: datePicker.monthNames
                    currentIndex: calendar.month
                    font.pixelSize: 12
                    Layout.preferredWidth: 72
                    onCurrentIndexChanged: calendar.month = currentIndex
                }
                ComboBox {
                    font.pixelSize: 12
                    Layout.preferredWidth: 72
                    model: ListModel {
                        Component.onCompleted: {
                            for (let i = datePicker.currentYear - 80; i <= datePicker.currentYear + 50; ++i)
                                append({ text: i })
                        }
                    }
                    currentIndex: datePicker.selectedDate.getFullYear() - (datePicker.currentYear - 80)
                    onCurrentIndexChanged: {
                        const item = model.get(currentIndex)
                        if (item && item.text !== undefined) {
                            calendar.year = parseInt(item.text)
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
                Button {
                    text: ">"
                    font.pixelSize: 12
                    Layout.preferredWidth: 26
                    onClicked: {
                        if (calendar.month === 11) {
                            calendar.month = 0
                            calendar.year += 1
                        } else {
                            calendar.month += 1
                        }
                    }
                }
            }

            // 星期标题（精确宽度对齐 cellSize）
            GridLayout {
                Layout.fillWidth: true
                columns: 7
                Repeater {
                    model: datePicker.weekdayNames
                    Item {
                        width: datePicker.cellSize
                        height: datePicker.cellSize
                        Label {
                            text: modelData
                            font.pixelSize: 12
                            font.bold: true
                            color: "#666666"
                            anchors.centerIn: parent
                        }
                    }
                }
            }

            // 日历内容网格
            MonthGrid {
                id: calendar
                month: datePicker.selectedDate.getMonth()
                year: datePicker.selectedDate.getFullYear()
                Layout.fillWidth: true
                Layout.fillHeight: true

                delegate: Rectangle {
                    width: datePicker.cellSize
                    height: datePicker.cellSize
                    radius: datePicker.cellRadius
                    color: {
                        const isSelected = model.day === datePicker.selectedDate?.getDate() &&
                                           model.month === calendar.month &&
                                           model.year === calendar.year
                        if (isSelected) {
                            return datePicker.themeColor
                        } else if (mouseArea.containsMouse) {
                            return Qt.darker(datePicker.themeColor, 1.4)
                        } else {
                            return "transparent"
                        }
                    }

                    Label {
                        anchors.centerIn: parent
                        text: model.day
                        font.pixelSize: 12
                        color: {
                            const isSelected = model.day === datePicker.selectedDate?.getDate() &&
                                               model.month === calendar.month &&
                                               model.year === calendar.year
                            if (model.month !== calendar.month) {
                                return "#999999"
                            } else if (isSelected) {
                                return "white"
                            } else if (mouseArea.containsMouse) {
                                return "#333333"
                            } else {
                                return "black"
                            }
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            if (model.month === calendar.month) {
                                datePicker.selectedDate = new Date(model.year, model.month, model.day)
                                datePicker.dateSelected(datePicker.formatDate(datePicker.selectedDate))
                                calendarPopup.close()
                            }
                        }
                    }
                }
            }

            // 底部按钮区域
            RowLayout {
                Layout.fillWidth: true
                spacing: 6
                Item { Layout.fillWidth: true }
                Button {
                    text: Qt.locale().name.startsWith("zh") ? "今天" : "Today"
                    font.pixelSize: 12
                    onClicked: {
                        datePicker.selectedDate = new Date()
                        datePicker.dateSelected(datePicker.formatDate(datePicker.selectedDate))
                        calendarPopup.close()
                    }
                }

                Button {
                    text: Qt.locale().name.startsWith("zh") ? "清除" : "Clear"
                    font.pixelSize: 12
                    onClicked: {
                        datePicker.selectedDate = null
                        datePicker.dateSelected(null)
                        calendarPopup.close()
                    }
                }
            }
        }
    }
}
