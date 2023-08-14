cmake_minimum_required(VERSION 3.22)

project(TestTracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    "../../Programs/Tracker"
    "../../Libs/AsterixHandler"
    "../../Libs/SettingsTracker"
)

find_package(Qt5 COMPONENTS
        Core
        Network
        REQUIRED
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(MAIN_PROJECT_DIR ../../Programs/Tracker)

add_executable(
  TestTracker
  main.cpp
  ${MAIN_PROJECT_DIR}/Airplane.cpp
  ${MAIN_PROJECT_DIR}/AirplaneHandler.cpp
  ${MAIN_PROJECT_DIR}/MessageHandler.cpp
  ${MAIN_PROJECT_DIR}/Plot.cpp
  ${MAIN_PROJECT_DIR}/Airplane.h
  ${MAIN_PROJECT_DIR}/AirplaneHandler.h
  ${MAIN_PROJECT_DIR}/MessageHandler.h
  ${MAIN_PROJECT_DIR}/Plot.h
)

target_link_libraries(
  TestTracker
  Qt5::Core
  Qt5::Network
  AsterixHandler
  SettingsTracker
  ${PCAP_LIBRARY}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestTracker)
