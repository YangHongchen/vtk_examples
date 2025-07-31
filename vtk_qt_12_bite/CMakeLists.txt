cmake_minimum_required(VERSION 3.19)
project(demo_qt_widget LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)


# 设置 Qt 和 VTK 路径（按需修改）
set(CMAKE_PREFIX_PATH
    "C:/Qt/6.6.3/msvc2019_64"
    "C:/vtk_930/lib/cmake/vtk-9.3"
)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED)

qt_standard_project_setup()

qt_add_executable(demo_qt_widget
    WIN32 MACOSX_BUNDLE
    main.cpp

    resources.qrc
    MainWindow.h MainWindow.cpp
    VTKOcclusionSimulation.h VTKOcclusionSimulation.cpp

)

target_link_libraries(demo_qt_widget
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(demo_qt_widget PRIVATE Qt6::Widgets)
target_link_libraries(demo_qt_widget PRIVATE Qt6::Core)
target_link_libraries(demo_qt_widget PRIVATE Qt6::Widgets)

#-----------------------------------------------------------------------------
# 链接库配置（注意顺序依赖）
#-----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${VTK_INCLUDE_DIRS})

# 自动初始化 VTK 模块（必须）
vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)


include(GNUInstallDirs)

install(TARGETS demo_qt_widget
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET demo_qt_widget
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

