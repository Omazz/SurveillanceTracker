cmake_minimum_required (VERSION 3.22)
 
project (SurveillanceTracker)

include(FindPCAP.cmake)

add_subdirectory(Programs/Analyzer)
add_subdirectory(Programs/Indicator)
add_subdirectory(Programs/Tracker)
add_subdirectory(Programs/Settings)
add_subdirectory(Programs/Radar)

add_subdirectory(Tests/TestAsterixHandler)
add_subdirectory(Tests/TestFilters)
add_subdirectory(Tests/TestSettingsTracker)
add_subdirectory(Tests/TestTracker)

add_subdirectory(Libs/AsterixHandler)
add_subdirectory(Libs/Filters)
add_subdirectory(Libs/GraphUI)
add_subdirectory(Libs/LoadingScreen)
add_subdirectory(Libs/SettingsTracker)


