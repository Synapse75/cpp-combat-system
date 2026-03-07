cmake_minimum_required(VERSION 3.15)
project(combat_system)

set(CMAKE_CXX_STANDARD 17)

# 递归添加 src 目录下所有 cpp 文件
file(GLOB_RECURSE SRC_FILES
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

# 如果后续有第三方库，可以在这里添加 include_directories 或 target_link_libraries
# include_directories(third_party/nlohmann)
# target_link_libraries(combat_system ...)