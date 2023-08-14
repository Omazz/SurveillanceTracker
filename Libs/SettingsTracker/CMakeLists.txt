cmake_minimum_required(VERSION 3.10)

project(SettingsTracker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core)

set(SOURCE_FILES
    SettingsTracker.cpp
)

set(HEADER_FILES
    SettingsTracker.h
)

add_library(SettingsTracker STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
