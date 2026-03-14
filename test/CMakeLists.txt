# test/CMakeLists.txt - placed in project/test/

# Create test executable(s)
add_executable(unit_tests
  combat_test.cpp
  event_test.cpp
  buff_test.cpp
  skill_test.cpp
)

# Ensure tests can include project headers and third_party libs
target_include_directories(unit_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(unit_tests
  PRIVATE
    GTest::gtest_main
    combat_system_lib
)

include(GoogleTest)
gtest_discover_tests(unit_tests)