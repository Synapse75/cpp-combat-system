cmake_minimum_required(VERSION 3.15)

project(combat_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/battle/*.cpp
    src/entity/*.cpp
    src/event/*.cpp
    src/combat/*.cpp
    src/skill/*.cpp
    src/buff/*.cpp
    src/core/*.cpp
    src/data/*.cpp
)

# Build library from sources so tests can link it (avoids duplicate compilation)
add_library(combat_system_lib STATIC ${SRC_FILES})
target_include_directories(combat_system_lib PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/third_party
)

# Main executable links the library
add_executable(combat_system main.cpp)
target_link_libraries(combat_system PRIVATE combat_system_lib)

# Warnings: use appropriate flags per compiler
if(MSVC)
  target_compile_options(combat_system_lib PRIVATE /W4)
  target_compile_options(combat_system PRIVATE /W4)
else()
  target_compile_options(combat_system_lib PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(combat_system PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Add tests subdirectory
add_subdirectory(test)