cmake_minimum_required(VERSION 3.15)

project(combat_system)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/entity/*.cpp
    src/event/*.cpp
    src/combat/*.cpp
    src/skill/*.cpp
    src/buff/*.cpp
    src/core/*.cpp
    src/data/*.cpp
)

add_executable(combat_system
    main.cpp
    ${SRC_FILES}
)

target_include_directories(combat_system PRIVATE src)