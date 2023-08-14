cmake_minimum_required(VERSION 3.22)

project(TestAsterixHandler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    "../../Libs/AsterixHandler"
)

find_package(Qt5 COMPONENTS
        Core
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

add_executable(
  TestAsterixHandler
  main.cpp
)

target_link_libraries(
  TestAsterixHandler
  Qt5::Core
  AsterixHandler
  ${PCAP_LIBRARY}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestAsterixHandler)
