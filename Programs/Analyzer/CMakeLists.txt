cmake_minimum_required(VERSION 3.22)

project(Analyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    Qt5 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

add_executable(Analyzer
    main.cpp
    RadarView.cpp
    RadarView.h
    TargetItem.cpp
    TargetItem.h
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    TrackHandler.cpp
    TrackHandler.h
)

include_directories(
    "../../Libs/Filters"
    "../../Libs/GraphUI"
    "../../Libs/LoadingScreen"
)

target_link_libraries(
    Analyzer
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Filters
    GraphUI
    LoadingScreen
)

