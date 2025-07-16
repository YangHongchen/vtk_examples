cmake_minimum_required(VERSION 3.19)
project(qt_widget_qml_intergration_demo LANGUAGES CXX)

set(CMAKE_AUTORCC ON)  # 自动处理资源编译

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(qt_widget_qml_intergration_demo
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

qt_add_qml_module(qt_widget_qml_intergration_demo
    URI qt_widget_qml_intergration_demo
    VERSION 1.0
    QML_FILES
        qml/Analysis.qml
        qml/Dashboard.qml
        qml/Settings.qml
    RESOURCES
       qml.qrc
)

target_link_libraries(qt_widget_qml_intergration_demo
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Quick
        Qt6::QuickWidgets
)

include(GNUInstallDirs)

install(TARGETS qt_widget_qml_intergration_demo
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET qt_widget_qml_intergration_demo
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
