cmake_minimum_required(VERSION 3.22)

project(AsterixHandler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
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
    AsterixReader.cpp
)

set(
    HEADER_FILES
    AsterixReader.h
    protocol.h
)


add_library(
    AsterixHandler
    STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_link_libraries(
    AsterixHandler
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${PCAP_LIBRARY}
)
