cmake_minimum_required(VERSION 3.16)

project(qt_quick_widget_intergration_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Widgets)

qt_add_executable(appqt_quick_widget_intergration_demo
    main.cpp
)

qt_add_qml_module(appqt_quick_widget_intergration_demo
    URI qt_quick_widget_intergration_demo
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        widgetcontainer.h
        widgetcontainer.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqt_quick_widget_intergration_demo PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqt_quick_widget_intergration_demo
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqt_quick_widget_intergration_demo
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS appqt_quick_widget_intergration_demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
