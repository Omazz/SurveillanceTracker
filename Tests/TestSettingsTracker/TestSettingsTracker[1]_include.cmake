if(EXISTS "/home/leonid/Рабочий стол/Projects/PetProjects/CppQt/SurveillanceTracker/Tests/TestSettingsTracker/TestSettingsTracker[1]_tests.cmake")
  include("/home/leonid/Рабочий стол/Projects/PetProjects/CppQt/SurveillanceTracker/Tests/TestSettingsTracker/TestSettingsTracker[1]_tests.cmake")
else()
  add_test(TestSettingsTracker_NOT_BUILT TestSettingsTracker_NOT_BUILT)
endif()
