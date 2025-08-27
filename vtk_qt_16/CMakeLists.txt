# 定义CMake最低版本要求（VTK 9.3需要至少3.16）
cmake_minimum_required(VERSION 3.16)

#-----------------------------------------------------------------------------
# 项目名称和基础配置
#-----------------------------------------------------------------------------
project(VTKQtIntergrationDemo
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Qt + VTK 集成演示项目"
)

#-----------------------------------------------------------------------------
# 编译器标准设置
#-----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)           # VTK 9.x 需要C++14及以上
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准

#-----------------------------------------------------------------------------
# Qt自动处理工具（MOC/UIC/RCC）
#-----------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)  # 自动处理Qt元对象编译器
set(CMAKE_AUTOUIC ON)  # 自动处理UI文件编译器
set(CMAKE_AUTORCC ON)  # 自动处理资源文件编译器

#-----------------------------------------------------------------------------
# 依赖库路径配置（根据实际安装路径修改）
#-----------------------------------------------------------------------------
# Windows平台典型路径配置（注意使用正斜杠）
set(CMAKE_PREFIX_PATH
    "C:/Qt/6.6.3/msvc2019_64"           # Qt安装路径
    "C:/vtk_930/lib/cmake/vtk-9.3"      # VTK安装路径
)

#-----------------------------------------------------------------------------
# Qt6模块查找（按需添加组件）
#-----------------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Widgets        # QWidget支持（必须）
    Core           # Qt核心模块（必须）
    Gui            # GUI基础模块（必须）
    Quick          # QML支持
    Qml            # QML引擎
    QuickControls2 # QtQuick控件
    OpenGLWidgets  # OpenGL集成支持
)

#-----------------------------------------------------------------------------
# VTK模块查找（关键组件说明）
#-----------------------------------------------------------------------------
find_package(VTK REQUIRED
    GUISupportQt            # Qt窗口集成
    RenderingQt             # Qt渲染支持
    RenderingOpenGL2        # OpenGL渲染后端
    InteractionStyle        # 交互样式
)

#-----------------------------------------------------------------------------
# 可执行目标配置
#-----------------------------------------------------------------------------
add_executable(${PROJECT_NAME}
    main.cpp
    # 可添加其他源文件...
)

#-----------------------------------------------------------------------------
# 链接库配置（注意顺序依赖）
#-----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    # Qt核心模块
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets

    # Qt Quick相关
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickControls2

    # OpenGL支持
    Qt6::OpenGLWidgets

    # VTK模块（按依赖顺序排列）
    VTK::GUISupportQt            # Qt窗口集成
    VTK::RenderingQt             # Qt渲染支持
    VTK::RenderingOpenGL2        # OpenGL渲染后端
    VTK::InteractionStyle        # 交互样式
)

# 自动初始化 VTK 模块（必须）
vtk_module_autoinit(
    TARGETS VTKQtIntergrationDemo
    MODULES ${VTK_LIBRARIES}
)


