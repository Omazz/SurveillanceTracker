cmake_minimum_required(VERSION 3.22)

project(Radar VERSION 0.1 LANGUAGES CXX)

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
    Network
    REQUIRED
)

add_executable(
    Radar
    main.cpp
    RadarWindow.cpp
    RadarWindow.h
    RadarWindow.ui
)

include_directories(
    "../../Libs/AsterixHandler"
    "../../Libs/LoadingScreen"
)

target_link_libraries(
    Radar
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    AsterixHandler
    LoadingScreen
    ${PCAP_LIBRARY}
)

