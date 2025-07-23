import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../_components"

// 左侧菜单
Item {
    id: sideMenu
    implicitWidth: 240
    implicitHeight: 800

    // 定义页面切换信号
    signal pageChanged(int page)

    // 当前菜单所在页面
    property string currentPage: "patient/PatientIndex.qml"

    // 正确的订阅
    Component.onCompleted: {
        EventBus.subscribeJS("jumpTo", function(eventName, payload) {
            console.log("收到登录事件:", eventName)
            console.log("数据:", payload)
        })
    }

    // 数据模型
    ListModel {
        id: menuModel
        ListElement {
            name: "病例管理"
            page: "patient/PatientIndex.qml"
            pageIndex: 0
            isParent: true
            expanded: false
            isActive: true
            icon: "qrc:/assets/menu/chart.svg"
            hasChild: false
        }
        // 检测
        ListElement {
            name: "检测"
            page: "mesure/MesureIndex.qml"
            pageIndex: 1
            isParent: true
            expanded: false
            isActive: false
            icon: "qrc:/assets/menu/pencil.svg"
            hasChild: false
        }
        // 分析
        ListElement {
            name: "分析"
            isParent: true
            expanded: false
            isActive: false
            icon: "qrc:/assets/menu/chart.svg"
            hasChild: true
        }
        ListElement {
            name: "演示"
            page: "analysis/Model.qml"
            pageIndex: 3
            isParent: false
            parentName: "分析"
            isActive: false
            hasChild: false
        }
        ListElement {
            name: "轨迹"
            page: "analysis/Tracks.qml"
            pageIndex: 4
            isParent: false
            parentName: "分析"
            isActive: false
            hasChild: false
        }
        ListElement {
            name: "虚拟人"
            page: "analysis/3D.qml"
            pageIndex: 5
            isParent: false
            parentName: "分析"
            isActive: false
            hasChild: false
        }
        ListElement {
            name: "碰撞"
            page: "analysis/Collision.qml"
            pageIndex: 6
            isParent: false
            parentName: "碰撞"
            isActive: false
            hasChild: false
        }
        // 报告
        ListElement {
            name: "报告"
            page: "report/Index.qml"
            pageIndex: 7
            isParent: true
            expanded: false
            isActive: false
            icon: "qrc:/assets/menu/chat-fill.svg"
            hasChild: false
        }
    }

    ListView {
        id: listView
        width: 240
        height: parent.height
        y: 24
        clip: true // 防止内容溢出渲染
        interactive: false
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
        model: menuModel
        delegate: Item {
            width: ListView.view.width
            height: 40
            visible: true
            Rectangle {
                id: menu_item
                width: 216
                height: 36
                x: 12
                color: model.isActive ? '#E5F3FF' : 'transparent'
                radius: 4
                // Behavior on color { ColorAnimation { duration: 100 } } // 颜色过渡动画
                Image {
                    id: menu_icon
                    source: model.icon
                    width: 20
                    height: model.isParent ? 20 : 0
                    anchors.verticalCenter: parent.verticalCenter
                    x: 12
                }
                Text {
                    id: menu_text
                    text: model.name
                    anchors.verticalCenter: parent.verticalCenter
                    x: menu_icon.x + menu_icon.width + 8
                    font.pixelSize: 14
                    color: model.isActive ? '#0B4DA2' : "#1A2133"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true // 启用悬停检测
                    onClicked: {
                        if (model.isParent && model.hasChild) {
                            activeMenuAndParent(index + 1,
                                                model.page) // 默认选中第一个子菜单
                            return
                        } else {
                            // 默认选中子菜单或无子菜单的一级菜单
                            activeMenuAndParent(index)
                        }
                    }
                    onEntered: {
                        menu_text.font.bold = true // 鼠标进入时, 字体加粗
                    }
                    onExited: {
                        menu_text.font.bold = false // 鼠标进入时, 字体恢复
                    }
                }
            }
        }
    }

    // 激活当前选中的菜单和其父菜单
    function activeMenuAndParent(index, page) {
        // 取消所有激活状态
        for (var i = 0; i < menuModel.count; i++) {
            menuModel.setProperty(i, "isActive", false)
        }
        // 设置当前项激活
        menuModel.setProperty(index, "isActive", true)
        // 设置 当前页面路径 && 发出pageChanged信号
        let childMenu = menuModel.get(index)
        if (childMenu) {
            let pageIndex = childMenu.pageIndex
            let pageUrl = childMenu.page
            console.log('切换page:', pageIndex, pageUrl)
            if (pageIndex >= 0) {
                sideMenu.currentPage = pageUrl
                sideMenu.pageChanged(pageIndex)
            }
            console.log("要切换的页面：", childMenu.page)
        } else {
            console.warn('未配置正确的页面URL')
        }
    }

    // 跳转到页面
    function jumpTo(pagePath) {
        console.log('跳转到页面：', pagePath)
        let activeIndex = 0
        // 取消所有激活状态
        for (var i = 0; i < menuModel.count; i++) {
            let childMenu = menuModel.get(i)
            let pageUrl = childMenu.page
            if (pagePath === pageUrl) {
                activeIndex = i
                break
            }
        }
        console.log('跳转到页面 activeIndex=', activeIndex, ',pagePath=', pagePath)
        activeMenuAndParent(activeIndex, pagePath)
    }
}
