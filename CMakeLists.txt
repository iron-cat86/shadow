cmake_minimum_required(VERSION 3.26)
project(shadow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(${PROJECT_NAME} "main.cpp")

set(headers
    calculating.h
    mainwindow.h)

set(sources
    calculating.cpp
    mainwindow.cpp)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtWidgets)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtCore)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtGui)
include_directories(/usr/include)
include_directories(/usr/include/GL)

SET(CMAKE_CXX_COMPILER /usr/bin/g++)
SET(CMAKE_C_COMPILER /usr/bin/gcc)
SET(LD_LIBRARY_PATH /usr/lib/x86_64-linux-gnu)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)
SET(Qt5Core_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5Core)
SET(Qt5Gui_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5Gui)
SET(Qt5Widgets_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5Widgets)
SET(Qt5Svg_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5Svg)
SET(Qt5PrintSupport_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5PrintSupport)
SET(Qt5Concurrent_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5Concurrent)

link_directories(/usr/lib/x86_64-linux-gnu)

set(CMAKE_AUTOMOC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

target_sources(
    shadow
    PUBLIC ${headers} ${sources}
)

target_include_directories(
    shadow
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
        
target_link_libraries(shadow PUBLIC 
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)


target_compile_definitions(
    shadow
    PUBLIC _USE_MATH_DEFINES
)

set_target_properties(
    shadow
    PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)
