cmake_minimum_required(VERSION 3.10)
project(TaskManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(task_manager
    src/main.cpp
    src/Trackable.cpp
    src/TaskManager.cpp
)

target_include_directories(task_manager PRIVATE src)
