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
# 编译设备
#-----------------------------------------------------------------------------
set(CMAKE_BUILD_PARALLEL_LEVEL 8)  # 使用8个线程并行编译
# set(CMAKE_GENERATOR "Ninja")

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
# 引入qxorm库
#-----------------------------------------------------------------------------
set(QXORM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rd/qxorm/include")
set(QXORM_LIB_DIR "${CMAKE_SOURCE_DIR}/3rd/qxorm/lib")
include_directories(${QXORM_INCLUDE_DIR})
link_directories(${QXORM_LIB_DIR})

#-----------------------------------------------------------------------------
# QxORM 数据库映射类编译配置
#-----------------------------------------------------------------------------
add_definitions(-D_BUILDING_USER)
add_definitions(-D_BUILDING_DEVICE)
add_definitions(-D_BUILDING_PATIENT)
add_definitions(-D_BUILDING_MEDICAL_TESTING)
add_definitions(-D_BUILDING_MEDICAL_TESTING_STEP)
add_definitions(-D_BUILDING_MEDICAL_TESTING_DATA)
add_definitions(-D_BUILDING_HARDWARE)

#-----------------------------------------------------------------------------
# Qt模块查找（关键组件说明）
#-----------------------------------------------------------------------------
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Quick
    QuickWidgets
    Widgets
    Network
    Sql
    Multimedia
    Pdf
)

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

    # 通用函数
    src/common/EncryptUtils.h
    src/common/EncryptUtils.cpp

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
    src/user/User.h
    src/user/User.cpp
    src/user/UserDao.h
    src/user/UserDao.cpp

    # DB
     src/database/precompiled.h
    src/database/Database.h
    src/database/Database.cpp
    src/database/DBManager.h
    src/database/DBManager.cpp
    src/database/export.h
    src/database/BaseDao.h
    src/database/BaseDao.cpp
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

#-----------------------------------------------------------------------------
# 链接库 Qt 库
#-----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Quick
        Qt6::QuickWidgets
        Qt6::Sql
        Qt6::Multimedia
        Qt6::Pdf
        Qt6::Network
)

# ✅ 链接 QxOrm 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    QxOrm           # 静态库：QxOrm.lib；动态库也相同写法
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
