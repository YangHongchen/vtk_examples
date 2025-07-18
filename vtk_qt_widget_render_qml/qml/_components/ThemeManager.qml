// ThemeManager.qml
pragma Singleton
import QtQuick

QtObject {
    id: root

    // ==== 主题定义 ====
    enum Theme { Default, Dark }
    property int currentTheme: Theme.Default

    // ==== 主题切换信号 ====
    signal themeChanged(int theme)

    // ==== 主题配置 ====
    readonly property var _themes: ({
        [Theme.Default]: {
            primary: "#0B4DA2",
            primaryLight: "#E5F3FF",
            success: "#27ae60",
            warning: "#e67e22",
            danger: "#ff4d4f",
            textPrimary: "#FFFFFF",
            textNormal: "#1A2133",
            textRegular: "#606266",
            textGray: "#9FA7BD",
            border: "#D8DDEB",
            background: "#FFFFFF",
            cardBackground: "#F8FAFD"
        },
        [Theme.Dark]: {
            primary: "#00C38B",
            primaryLight: "#E6F9F4",
            success: "#3E8641",
            warning: "#A6793D",
            danger: "#A94442",
            textPrimary: "#FFFFFF",
            textNormal: "#E0E0E0",
            textRegular: "#B0B0B0",
            textGray: "#9FA7BD",
            border: "#4A4A4A",
            background: "#1E1E1E",
            cardBackground: "#2D2D2D"
        }
    })

    // ==== 颜色属性 ====
    readonly property color primary: _themes[currentTheme].primary
    readonly property color primaryLight: _themes[currentTheme].primaryLight
    readonly property color success: _themes[currentTheme].success
    readonly property color warning: _themes[currentTheme].warning
    readonly property color danger: _themes[currentTheme].danger
    readonly property color textPrimary: _themes[currentTheme].textPrimary
    readonly property color textNormal: _themes[currentTheme].textNormal
    readonly property color textRegular: _themes[currentTheme].textRegular
    readonly property color textGray: _themes[currentTheme].textGray
    readonly property color border: _themes[currentTheme].border
    readonly property color background: _themes[currentTheme].background
    readonly property color cardBackground: _themes[currentTheme].cardBackground

    // ==== 辅助颜色 ====
    readonly property color white: "#FFFFFF"
    readonly property color black: "#000000"
    readonly property color transparent: "transparent"

    // ==== 功能方法 ====
    function switchTheme(theme) {
        if (theme !== currentTheme && _themes[theme] !== undefined) {
            currentTheme = theme
            themeChanged(theme)
        }
    }

    // ==== 便捷访问 ====
    function color(name) {
        return _themes[currentTheme][name] || transparent
    }
}
