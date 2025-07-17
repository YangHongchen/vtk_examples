# 定义CMake最低版本要求（VTK 9.3需要至少3.16）
cmake_minimum_required(VERSION 3.19)

#-----------------------------------------------------------------------------
# 项目名称和基础配置
#-----------------------------------------------------------------------------
project(qt_widget_qml_intergration_demo
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "QtWidget QML VTK集成"
)

#-----------------------------------------------------------------------------
# 编译器标准设置
#-----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)           # VTK 9.x 需要C++14及以上
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准

#-----------------------------------------------------------------------------
# 依赖库路径配置（根据实际安装路径修改）
#-----------------------------------------------------------------------------
# Windows平台典型路径配置（注意使用正斜杠）
set(CMAKE_PREFIX_PATH
    "C:/Qt/6.6.3/msvc2019_64"           # Qt安装路径
    "C:/vtk_930/lib/cmake/vtk-9.3"      # VTK安装路径
)

#-----------------------------------------------------------------------------
# Qt自动处理工具（MOC/UIC/RCC）
#-----------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#-----------------------------------------------------------------------------
# Qt模块查找（关键组件说明）
#-----------------------------------------------------------------------------
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

#-----------------------------------------------------------------------------
# VTK模块查找（关键组件说明）
#-----------------------------------------------------------------------------
find_package(VTK REQUIRED)

qt_standard_project_setup()

add_executable(${PROJECT_NAME}
    # 入口文件
    main.cpp
    # 主窗口（负责登录后页面管理）
    MainWindow.h
    MainWindow.cpp

    # Qml Context注册类



    # 自定义组件（目前是vtk效果测试组件）
    CustomWidget.h
    CustomWidget.cpp

    # 登录窗口
    LoginWidget.h
    LoginWidget.cpp

    # 病例管理
    src/patient/PatientController.h
    src/patient/PatientController.cpp

    # 登录
    src/user/LoginController.h
    src/user/LoginController.cpp

)

#-----------------------------------------------------------------------------
# QML模块配置（关键修改）
#-----------------------------------------------------------------------------
set(QML_MODULE_NAME "AppUI")  # 自定义模块名避免冲突
qt_add_qml_module(${PROJECT_NAME}
    URI ${QML_MODULE_NAME}  # 统一模块命名空间
    VERSION 1.0
    QML_FILES
        qml/NavigationMenu.qml
        qml/Dashboard.qml
        qml/Settings.qml
        qml/Analysis.qml
    RESOURCE_PREFIX "/qml"  # 显式指定资源前缀
)

#-----------------------------------------------------------------------------
# 资源文件处理（替代qt_add_resources）
#-----------------------------------------------------------------------------
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc
)
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES})


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Quick
        Qt6::QuickWidgets
)

#-----------------------------------------------------------------------------
# 链接库配置（注意顺序依赖）
#-----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

#-----------------------------------------------------------------------------
# 自动初始化 VTK 模块（必须）
#-----------------------------------------------------------------------------
vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
