import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Dialogs


import "../_widgets"
import "../_components"

Rectangle {
    id:root
    implicitWidth: parent?parent.width:800
    implicitHeight: parent?parent.height:600
    color: "purple"

    property int patientId : 0
    property string fullName: ""
    property int gender: 2
    property string birthDay: "0000-00-00"
    property string phone: "-"
    property int age: 0

    // 最近检查日期
    property string lastTestingTime:  "0000-00-00"

    // 上传模型(模型URL)
    property string mandibleStllUrl: ''
    property string maxillaStllUrl: ''
    property string upperDentitionStllUrl: ''
    property string lowerDentitionStllUrl: ''

    // 上传模型(缩略图URL)
    property string mandibleStlThumbnailUrl: ''
    property string maxillaStlThumbnailUrl: ''
    property string upperDentitionStlThumbnailUrl: ''
    property string lowerDentitionStlThumbnailUrl: ''


    // 模型上传类型
    property int stlUploadType: 1


    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        // 病人信息
        Rectangle {
            id: patient_info
            Layout.fillWidth: true
            height: 104
            ColumnLayout {
                anchors.fill: parent
                spacing: 15
                RowLayout {
                    Layout.fillWidth: true
                    height: 28
                    Text {
                        // 病人信息
                        id: patient_info_name
                        text: root.fullName || '未设定'
                        font.pixelSize: 20
                        color: "#1A2133"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    }
                    WImageSvg {
                        source:root.gender === 0 ? "qrc:/assets/icons/women.svg" : "qrc:/assets/icons/men.svg"
                        width: 14
                        height: 14
                        color: root.gender === 0 ? "#F98981" : "#4080FF"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    }
                }

                // 病人信息 =======================
                RowLayout {
                    Layout.fillWidth: true
                    height: 20
                    spacing: 24
                    Text {
                        // 病人信息_ID
                        id: patient_info_id
                        text: "ID: " + root.patientId
                        font.pixelSize: 14
                        color: "#9FA7BD"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        onTextChanged: {

                        }
                    }
                    Text {
                        // 病人信息_电话
                        id: patient_info_phone
                        text: "电话: " +  root.phone || '未设定'
                        font.pixelSize: 14
                        color: "#9FA7BD"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    }
                    Text {
                        // 病人信息_出生日期
                        id: patient_info_birth
                        text: "出生日期: " + root.birthDay || "0000-00-00"
                        font.pixelSize: 14
                        color: "#9FA7BD"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    }
                    Text {
                        // 病人信息_最近检查日期
                        id: patient_info_check_date
                        text: "最近检查日期: " + root.lastTestingTime || "0000-00-00"
                        font.pixelSize: 14
                        color: "#9FA7BD"
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    }
                }
                Item {
                    Layout.fillHeight: true
                }
            }
        }

        // 病人模型上传 =======================
        Rectangle {
            Layout.fillWidth: true
            height: 128
            border.width: 1
            border.color: '#EBEEF5'
            radius: 0
            RowLayout {
                anchors.fill: parent
                spacing: 40
                Item {
                    width: 1
                }
                UploadButton {
                    icon: "qrc:/assets/icons/patient/skull.svg"
                    toolTip: qsTr("上传上颌骨模型")
                    uploadType: 1
                    previewUrl: root.maxillaStlThumbnailUrl
                    enabled: root.patientId > 0
                    onClicked: (stlType, _previewUrl)=> {
                        root.stlUploadType = stlType
                    }
                }
                UploadButton {
                    icon: "qrc:/assets/icons/patient/mandible.svg"
                    toolTip: qsTr("上传下颌骨模型")
                    uploadType: 2
                    previewUrl: root.mandibleStlThumbnailUrl
                    enabled: root.patientId > 0
                    onClicked: (stlType, _previewUrl)=> {
                        root.stlUploadType = stlType

                    }
                }
                UploadButton {
                    icon: "qrc:/assets/icons/patient/upper_jaw.svg"
                    toolTip: qsTr("上传上牙列模型")
                    uploadType: 3
                    previewUrl:root.upperDentitionStlThumbnailUrl
                    enabled: root.patientId > 0
                    onClicked: (stlType, _previewUrl)=> {
                        root.stlUploadType = stlType

                    }
                }
                UploadButton {
                    icon: "qrc:/assets/icons/patient/jaw.svg"
                    toolTip: qsTr("上传下牙列模型")
                    uploadType: 4
                    previewUrl: root.lowerDentitionStlThumbnailUrl
                    enabled: root.patientId > 0
                    onClicked: (stlType, _previewUrl) => {
                        root.stlUploadType = stlType
                    }
                }

                WButton {
                    text: qsTr("开始测量")
                    enabled: root.patientId > 0
                    onClicked:  {
                        console.log('xxxxxxxxx 开始测量')
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }

        // 病人检测记录表格
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                // 表头
                Item {
                    id: table_header
                    Layout.fillWidth: true
                    height: 56
                    Text {
                        text: qsTr("检测记录")
                        font.pixelSize: 16
                        font.weight: 500
                        color: "#1A2133"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                // 表数据
                ColumnLayout {
                    id: table_content
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 0

                }
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }

}



