cmake_minimum_required(VERSION 3.22)

project(Indicator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED
)

add_executable(Indicator
        main.cpp
        IndicatorWindow.cpp
        IndicatorWindow.h
        IndicatorWindow.ui
        RadarView.cpp
        RadarView.h
        TargetItem.cpp
        TargetItem.h
        MessageHandler.cpp
        MessageHandler.h
)

include_directories(
        "../../Libs/SettingsTracker"
        "../../Libs/AsterixHandler"
        "../../Libs/LoadingScreen"
)

target_link_libraries(Indicator
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        SettingsTracker
        AsterixHandler
        LoadingScreen
        ${PCAP_LIBRARY}
)
