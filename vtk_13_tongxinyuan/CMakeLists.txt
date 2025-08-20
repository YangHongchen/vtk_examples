# 定义CMake最低版本要求（VTK 9.3需要至少3.16）
cmake_minimum_required(VERSION 3.16)

#-----------------------------------------------------------------------------
# 项目名称和基础配置
#-----------------------------------------------------------------------------
project(VTKQtTongxinyuan
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "VTK三维场景纹理"
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
# VTK模块查找（关键组件说明）
#-----------------------------------------------------------------------------
find_package(VTK REQUIRED)

#-----------------------------------------------------------------------------
# 可执行目标配置
#-----------------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets WebSockets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME}
    main.cpp
    resources.qrc
    CircularTrajectoryWidget.h CircularTrajectoryWidget.cpp

    # 可添加其他源文件...
)

target_link_libraries(VTKQtTongxinyuan PRIVATE
    Qt6::Widgets
    Qt6::Network
)
target_link_libraries(VTKQtTongxinyuan PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::WebSockets
)
target_link_libraries(VTKQtTongxinyuan PRIVATE Qt6::Core)
target_link_libraries(VTKQtTongxinyuan PRIVATE Qt6::Widgets)


#-----------------------------------------------------------------------------
# 链接库配置（注意顺序依赖）
#-----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

# 自动初始化 VTK 模块（必须）
vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)
