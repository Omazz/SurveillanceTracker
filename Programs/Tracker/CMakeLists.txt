cmake_minimum_required(VERSION 3.22)

project(Tracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(
    Qt5 COMPONENTS
    Core
    Network
    REQUIRED
)

add_executable(
    Tracker
    main.cpp
    Airplane.cpp
    AirplaneHandler.cpp
    MessageHandler.cpp
    Plot.cpp
    Airplane.h
    AirplaneHandler.h
    MessageHandler.h
    Plot.h
)

include_directories(
    "../../Libs/AsterixHandler"
    "../../Libs/SettingsTracker"
)

target_link_libraries(
    Tracker
    Qt5::Core
    Qt5::Network
    AsterixHandler
    SettingsTracker
    ${PCAP_LIBRARY}
)
