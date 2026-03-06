cmake_minimum_required(VERSION 3.15)
project(combat_system)

set(CMAKE_CXX_STANDARD 17)

add_executable(combat_system
    main.cpp
    src/entity/Entity.cpp
    src/entity/EntityFactory.cpp
)