cmake_minimum_required(VERSION 3.22)

project(GraphUI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(
    Qt5 COMPONENTS
    Core
    Gui
    Widgets
    Charts
    REQUIRED
)

set(
    SOURCE_FILES
    GraphicsBuilderWidget.cpp
    GraphicsBuilderWidget.ui
)

set(
    HEADER_FILES
    GraphicsBuilderWidget.h
)

add_library(
    GraphUI
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(
    GraphUI
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt::Charts
)
