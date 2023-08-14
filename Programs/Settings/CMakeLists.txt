cmake_minimum_required(VERSION 3.22)

project(Settings VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

add_executable(Settings
        main.cpp
        SettingsWindow.cpp
        SettingsWindow.h
        SettingsWindow.ui
)


include_directories(
        "../../Libs/SettingsTracker"
        "../../Libs/LoadingScreen"
)

target_link_libraries(Settings
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        SettingsTracker
        LoadingScreen
        ${PCAP_LIBRARY}
)
