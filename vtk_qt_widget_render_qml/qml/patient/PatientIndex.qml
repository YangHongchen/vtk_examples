import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../_components"

Rectangle {
    id: patientPage
    color: "#eaeef1"

    property int _margin: 16 // 设置边距，相对于父元素
    property bool loaded: false

    // 查询条件（关键词）
    property alias keyword: txt_keyword.text

    property var currentPatiant
    property real currentPatientId

    // 检测记录 =========
    property int page:1
    property int pageSize: 12

    Component.onCompleted: {
        if (!patientPage.loaded) {
            patientPage.loaded = true
            PatientController.loadPatientsConditional(patientPage.keyword, 1,20)
        }
    }

    // 消息通知组件 =====
    WNotification {
        id: notify
    }

    RowLayout {
        anchors.fill: parent
        spacing: 2
        // 左侧: 病人列表 ---------------
        Rectangle {
            id: patient_left_rect
            width: 320
            Layout.fillHeight: true
            color: "#eaeef1"
            ColumnLayout {
                anchors.fill: parent
                spacing: 1

                // 顶部标题 & 新增按钮 ==========
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    RowLayout {
                        anchors.fill: parent
                        Item {
                            width: _margin
                        }
                        Text {
                            id: flag2
                            text: qsTr("病人列表")
                            font.pixelSize: 20
                            font.weight: 600
                            Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        WButton {
                            text: qsTr("新增病例")
                            theme: WButton.Primary
                            iconSource: "qrc:/assets/icons/plus.svg"
                            onClicked: {
                                patientEditPopup.visible = true
                            }
                        }
                        Item {
                            width: _margin
                        }
                    }
                }

                // 搜索框 ==========
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    // color: '#f4fce3'
                    WTextField {
                        id: txt_keyword
                        width: parent.width - _margin * 2
                        height: 36
                        fontSize: 14
                        x: _margin
                        y: (parent.height - height) / 2
                        placeholderText: qsTr("输入姓名或电话筛选")
                        onTextChanged: {
                            PatientController.loadPatientsConditional(patientPage.keyword, 1, 20)
                        }
                    }
                }

                // 病例列表 ==========
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ListView {
                        id: listView
                        width: parent.width
                        height: parent.height
                        model: PatientModel
                        delegate: PatientListItem {
                            _id: model.id
                            firstName: model.firstName
                            lastName: model.lastName
                            phone: model.phone
                            birthDay: model.birthday
                            sex: model.gender
                            selected: index === listView.currentIndex
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    console.log("选中当前病人信息：", model.fullName)
                                    listView.currentIndex = index
                                    PatientController.selectPatient(model.id)
                                }
                            }
                        }
                    }
                }
            }
        }

        // 中间: 分割线 ---------------
        Rectangle {
            id: border_center
            width: 1
            Layout.fillHeight: true
            color: "#eaeef1"
        }

        // 右侧: 病人详情 ---------------
        Item {
            id: patient_right_rect
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                Layout.margins: 16
                spacing: 1

                // 病例详情 =========
                PatientDetail {
                    id: dt
                    patientId: patientPage.currentPatiant ? patientPage.currentPatiant.id : "0"
                    fullName: patientPage.currentPatiant ? patientPage.currentPatiant.fullName : "-"
                    gender: patientPage.currentPatiant ? patientPage.currentPatiant.gender : 2
                    birthDay: patientPage.currentPatiant ? patientPage.currentPatiant.birthDay : "0000-00-00"
                    phone: patientPage.currentPatiant ? patientPage.currentPatiant.phone : "-"
                    lastTestingTime: patientPage.currentPatiant ? patientPage.currentPatiant.lastTestingTime : "0000-00-00"
                    // 上传模型(模型URL)
                    maxillaStlUrl: patientPage.currentPatiant ? patientPage.currentPatiant.maxillaStlUrl : "00"
                    mandibleStlUrl: patientPage.currentPatiant ? patientPage.currentPatiant.mandibleStlUrl : "11"
                    upperDentitionStlUrl: patientPage.currentPatiant ? patientPage.currentPatiant.upperDentitionStlUrl : "22"
                    lowerDentitionStlUrl: patientPage.currentPatiant ? patientPage.currentPatiant.lowerDentitionStlUrl : "33"
                    // 上传模型(缩略图URL)
                    mandibleStlThumbnailUrl: patientPage.currentPatiant ? patientPage.currentPatiant.mandibleStlThumbnailUrl : ""
                    maxillaStlThumbnailUrl: patientPage.currentPatiant ? patientPage.currentPatiant.maxillaStlThumbnailUrl : ""
                    upperDentitionStlThumbnailUrl: patientPage.currentPatiant ? patientPage.currentPatiant.upperDentitionStlThumbnailUrl : ""
                    lowerDentitionStlThumbnailUrl: patientPage.currentPatiant ? patientPage.currentPatiant.lowerDentitionStlThumbnailUrl : ""
                    x: 16
                    y: 16
                    Layout.fillWidth: true
                    height: 230
                    // 发起上传请求
                    onUpdateRequest: (type, url) => {
                        PatientFileTransferManager.copyFileToUploadDirectory(url, true, type)
                    }
                    // 开始检测：跳转到检测页面
                    onJumpToMesure:  {
                        EventBus.publish("jumpTo", "p")
                    }
                }

                // 检测记录 ==========
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Rectangle {
                        width:parent.width - 16 * 2
                        height:parent.height - 16*2
                        x:16
                        ColumnLayout {
                            anchors.fill: parent
                            // 表格组件 ========
                            Item  {
                                Layout.fillWidth: true
                                Layout.fillHeight:true
                                WTable {
                                    anchors.fill: parent
                                    model: MesureRecordModel
                                    columns: [
                                        { title: "编号", prop: "id", width: 100 },
                                        { title: "医生", prop: "doctor", width: 100 },
                                        { title: "状态", prop: "status" },
                                        { title: "备注", prop: "comments" },
                                        { title: "检测时间", prop: "createTime" },
                                        { title: "报告", prop: "reportUrl", width: 100 }
                                    ]
                                }
                            }
                            // 分页组件 ========
                            Rectangle {
                                Layout.fillWidth: true
                                height: 68
                                WPagination {
                                    pageCount: MesureRecordModel.pageCount
                                    totalCount: MesureRecordModel.totalCount
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 编辑病例 ==========
    PatientEditPopup {
        id: patientEditPopup
        anchors.centerIn: parent
        onAccepted: function (formDataObj) {
            console.log('病例编辑表单，提交的数据', JSON.stringify(formDataObj))
            PatientController.submitPatientFormData(formDataObj)
        }
        onCancelled: {
            console.log('表单取消提交！')
        }
    }

    // 病例模型 ==========
    Connections {
        target: PatientModel
        // 监听到病例id变化
        function onCurrentPatientIdChanged(id) {
            console.log('$$$$ 病例的id变化：', id)
            currentPatientId = id
            MesureRecordController.loadMesureRecords(id, patientPage.page, patientPage.pageSize)

            console.log("###  model.count =", MesureRecordModel.totalCount)
            if (MesureRecordModel.get && MesureRecordModel.totalCount > 0) {
                const row = MesureRecordModel.get(0)
                console.log("### First row doctor:", row.doctor)
            }

        }
        // 监听到病例变化
        function onCurrentPatientChanged(patientObj) {
            // console.log('病例发生变化：', JSON.stringify(patientObj))
            patientPage.currentPatiant = JSON.parse(JSON.stringify(patientObj))
        }
    }

    // stl模型上传事件监听 ==========
    Connections {
        target: PatientFileTransferManager
        // 文件上传完成
        function onFileUploadCompleted(filePath, stlType) {
            PatientController.updatePatientStl(filePath, stlType)
            notify.success('文件上传完成')
        }
        // 文件上传失败
        function onFileUploadFailed(errorMessage) {
            notify.error('文件上传失败:' + errorMessage)
        }
        // 上传状态变更
        function onUploadingStateChanged(uploading) {
            console.log('--> 上传状态变更:', uploading)
        }
    }
}
