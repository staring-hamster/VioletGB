cmake_minimum_required(VERSION 3.16)
project(VioletGB CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

add_compile_options(-Wall -Wextra -Wconversion)

add_executable(VioletGB
    src/main.cpp
    src/cpu.cpp
    src/memory_bus.cpp
    src/cartridge.cpp
)

target_include_directories(VioletGB PRIVATE include)