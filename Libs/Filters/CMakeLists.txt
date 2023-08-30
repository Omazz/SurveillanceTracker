cmake_minimum_required(VERSION 3.22)

project(Filters)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
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
    AdaptiveKalmanConstVelocityFilter.cpp
    AlphaBetaFilter.cpp
    AlphaBetaLeastSquaresFilter.cpp
    AlphaBetaWLeastSquaresFilter.cpp
    KalmanConstAccelerationFilter.cpp
    KalmanConstVelocityFilter.cpp
    Matrix.cpp
)

set(
    HEADER_FILES
    AbstractFilter.h
    AdaptiveKalmanConstVelocityFilter.h
    AlphaBetaFilter.h
    AlphaBetaLeastSquaresFilter.h
    AlphaBetaWLeastSquaresFilter.h
    KalmanConstAccelerationFilter.h
    KalmanConstVelocityFilter.h
    Matrix.h
)


add_library(
    Filters
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(
    Filters
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
