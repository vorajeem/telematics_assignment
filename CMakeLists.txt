cmake_minimum_required(VERSION 3.16)

project(telematics_assignment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(telematics 
    src/main.cpp
    src/Vehicle.cpp
)


target_include_directories(telematics
    PRIVATE 
    include
)

