import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls

import "../_widgets"
import "../_components"

Rectangle {
    id: root
    color: "#eaeef1"
    property int patientId: 0
    property string fullName: ""
    property int gender: 2
    property string birthDay: "0000-00-00"
    property string phone: "-"
    property int age: 0

    // 最近检查日期
    property string lastTestingTime: "0000-00-00"

    // 上传模型(模型URL)
    property string mandibleStlUrl: ''
    property string maxillaStlUrl: ''
    property string upperDentitionStlUrl: ''
    property string lowerDentitionStlUrl: ''

    // 上传模型(缩略图URL)
    property string mandibleStlThumbnailUrl: ''
    property string maxillaStlThumbnailUrl: ''
    property string upperDentitionStlThumbnailUrl: ''
    property string lowerDentitionStlThumbnailUrl: ''

    // 模型上传类型
    property int stlUploadType: 1

    // 定义信号：PatientIndex.qml 统一负责后端交互
    signal updateRequest(int stlType, string stlFileUrl)

    // 跳转到检测页面
    signal jumpToMesure();

    // 格式化图片路径：解决图片加载失败的问题
    function normalizeFilePath(path) {
        if (path.startsWith("file:///")) {
            return path
        } else if (path.startsWith("file://")) {
            return "file:///" + path.substring(7) // 修正双斜杠为三斜杠
        } else if (path.startsWith("C:/") || path.startsWith("/")) {
            return "file:///" + path
        } else {
            return ""
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        Rectangle {
            Layout.fillWidth: true
            height: 104

              // 病人信息 ===========
              Rectangle {
                  id: patient_info
                  width: parent.width - 16*2
                  height: parent.height- 16 * 2
                  x:16
                  y:16
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
                              source: root.gender === 0 ? "qrc:/assets/icons/women.svg" : "qrc:/assets/icons/men.svg"
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
                          }
                          Text {
                              // 病人信息_电话
                              id: patient_info_phone
                              text: "电话: " + root.phone || '未设定'
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
        }

        // 病人模型上传 ===========
        Rectangle {
            Layout.fillWidth: true
            height: 128
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
                    onClicked: (stlType, _previewUrl) => {
                        root.stlUploadType = stlType
                        if (previewUrl.length > 5) {
                            previewer.source = normalizeFilePath(previewUrl)
                            previewer.title = qsTr("上颌模型-图片预览")
                            previewer.open()
                        }
                   }
                }
                UploadButton {
                    icon: "qrc:/assets/icons/patient/mandible.svg"
                    toolTip: qsTr("上传下颌骨模型")
                    uploadType: 2
                    previewUrl: root.mandibleStlThumbnailUrl
                    enabled: root.patientId > 0
                    onClicked: (stlType, _previewUrl) => {
                        root.stlUploadType = stlType
                        if (previewUrl.length > 5) {
                            previewer.source = normalizeFilePath(previewUrl)
                            previewer.title = qsTr("下颌模型-图片预览")
                            previewer.open()
                        }
                    }
                }
                UploadButton {
                    icon: "qrc:/assets/icons/patient/upper_jaw.svg"
                    toolTip: qsTr("上传上牙列模型")
                    uploadType: 3
                    previewUrl: root.upperDentitionStlThumbnailUrl
                    enabled: root.patientId > 0
                    onClicked: (stlType, _previewUrl) => {
                        root.stlUploadType = stlType
                        if (previewUrl.length > 5) {
                           previewer.source =normalizeFilePath(previewUrl)
                           previewer.title = qsTr("上牙列模型-图片预览")
                           previewer.open()
                        }
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
                        if (previewUrl.length > 5) {
                            previewer.source =normalizeFilePath(previewUrl)
                            previewer.title = qsTr("下牙列模型-图片预览")
                            previewer.open()
                        }
                    }
                }

                WButton {
                    text: qsTr("开始测量")
                    enabled: root.patientId > 0
                    theme: WButton.Primary
                    iconSource: "qrc:/assets/icons/play_circle.svg"
                    onClicked: {
                        console.log("TODO: 开始检测");
                        let stlUploadComplete = root.maxillaStlUrl.length > 0
                            && root.mandibleStlUrl.length > 0
                            && root.upperDentitionStlUrl.length > 0
                            && root.lowerDentitionStlUrl.length > 0
                        if(stlUploadComplete) {
                            // 切换界面
                            // sideMenu.test()
                            console.log('切换页面。。。')
                            root.jumpToMesure()
                        } else {
                            // 打开确认弹窗
                            warnDialog.open()
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    // 文件选择 ==========
    FileDialog {
        id: fileDialog
        title: qsTr("选择STL文件")
        nameFilters: ["STL文件 (*.stl)", "所有文件 (*)"]
        onAccepted: {
            // 去除【file:///】前缀
            var localPath = String(selectedFile).replace(/^file:\/{3}/, '')
            console.log("Selected localPath（clearn file:///）:", localPath)
            // 发出信号
            updateRequest(root.stlUploadType, localPath)
        }
        onRejected: {
            console.log("File selection canceled")
        }
    }

    // 图片预览组件
    WImagePreview {
        id: previewer
        parent: Overlay.overlay
        source: "https://iknow-pic.cdn.bcebos.com/a71ea8d3fd1f4134679182fe371f95cad0c85e88"
        anchors.centerIn: parent
        width: 1000
        height: 700
    }


    MessageDialog {
        id: warnDialog
        title: "确认"
        text: "暂未上传模型文件，是否继续检测?"
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onButtonClicked: function(button) {
            if (button === MessageDialog.Ok) {
                sideMenu.jumpTo('detection/Index.qml')
            } else {
                console.log('取消检测。')
            }
        }
    }
}
