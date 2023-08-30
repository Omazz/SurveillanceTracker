cmake_minimum_required(VERSION 3.22)

project(LoadingScreen)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(
    Qt5 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

set(
    SOURCE_FILES
    AppLoadScreen.cpp
)

set(
    HEADER_FILES
    AppLoadScreen.h
)

add_library(
    LoadingScreen
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(
    LoadingScreen
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
