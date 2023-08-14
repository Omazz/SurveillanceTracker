cmake_minimum_required(VERSION 3.22)

project(AsterixHandler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCE_FILES
    AsterixReader.cpp
)

set(HEADER_FILES
    AsterixReader.h
    protocol.h
)


add_library(AsterixHandler STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets
    ${PCAP_LIBRARY}
    )
