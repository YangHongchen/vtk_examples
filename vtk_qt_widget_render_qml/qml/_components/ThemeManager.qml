// ThemeManager.qml
pragma Singleton

import QtQuick

QtObject {
    id: root

    // 当前主题类型
    property string currentTheme: "default"

    // 颜色绑定
    property color primary: root.themeConfig[currentTheme].primary
    property color primaryLight: root.themeConfig[currentTheme].primary_light
    property color success: root.themeConfig[currentTheme].success
    property color warning: root.themeConfig[currentTheme].warning
    property color danger: root.themeConfig[currentTheme].danger
    property color textColor: root.themeConfig[currentTheme].textNormarl
    property color textWhite: "#FFFFFF"
    property color colorWhite: "#FFFFFF"
    property color textNormal: root.themeConfig[currentTheme].textNormarl
    property color textGray: root.themeConfig[currentTheme].textGray
    property color borderColor: root.themeConfig[currentTheme].border

    // 主题切换信号
    signal themeChanged(string themeName)
    // 主题色配置
    property var themeConfig: {
        "default": {
            "primary": "#0B4DA2",
            "primary_light": "#E5F3FF",
            "success": "#27ae60",
            "warning": "#e67e22",
            "danger": "#ff4d4f",
            "textPrimary": "#FFFFFF",
            "textNormarl": "#1A2133",
            "textRegular": "#606266",
            "textGray": "#9FA7BD",
            "border": "#D8DDEB"
        },
        "dark": {
            "primary": "#00C38B",
            "primary_light": "#E6F9F4",
            "success": "#3E8641",
            "warning": "#A6793D",
            "danger": "#A94442",
            "textPrimary": "#FFFFFF",
            "textNormarl": "#333333",
            "textRegular": "#B0B0B0",
            "textGray": "#9FA7BD",
            "border": "#D8DDEB"
        }
    }
}
