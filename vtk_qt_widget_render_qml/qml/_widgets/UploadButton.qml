import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

import "../_components"

Item {
    id: root
    width: 132
    height: 80

    property string icon: "qrc:/patient/svg/patient/skull.svg"
    property string text: "上传"
    property color  textColor: "#4E5969"
    property alias  toolTip: imageSource.tooltip
    property bool   hovered: false
    property real   uploadType: 0 // 上传类型
    property string previewUrl: ""
    property bool   enabled: false

    // 定义信号：被点击
    signal clicked(int type, string url)

    RowLayout {
        anchors.fill: parent
        WImageSvg {
            id: imageSource
            width: 40
            height: 40
            source: root.icon
            color: "#0B4DA2"
        }
        Rectangle {
            width: 80
            height: 80
            border.width: 1
            border.color: "#D8DDEB"
            radius: 8
            ColumnLayout {
                anchors.fill: parent
                Item {
                    Layout.fillHeight: true
                }
                WImageSvg {
                    id: upload_icon
                    width: 18
                    height: 18
                    source: root.previewUrl.length < 5 ? "qrc:/assets/icons/plus.svg" : "qrc:/assets/icons/preview.svg"
                    color: "#0B4DA2"
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignCenter
                }
                Text {
                    id: upload_label
                    text: root.previewUrl.length < 5 ? qsTr(root.text) : qsTr("预览")
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignCenter
                    color: textColor
                    font.pixelSize: 14
                }
                Item {
                    Layout.fillHeight: true
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true // 启用悬停检测
                cursorShape: Qt.PointingHandCursor // 鼠标悬停时显示手型光标
                enabled:root.enabled
                onEntered: {
                    // 鼠标进入时, 字体加粗
                    upload_icon.color = "#0B4DA2“"
                    upload_label.color = "#0B4DA2“"
                }
                onExited: {
                    upload_icon.color = textColor
                    upload_label.color = textColor
                }
                onClicked: {
                    if(root.previewUrl.length < 5) {
                        fileDialog.open()
                        root.clicked(root.uploadType, "")
                    }
                    else {
                        root.clicked(root.uploadType, root.previewUrl)
                    }
                }
            }
        }
    }
}
