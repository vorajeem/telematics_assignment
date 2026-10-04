cmake_minimum_required(VERSION 3.16)

project(telematics_assignment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third-party/simdjson)


find_package(SQLite3 REQUIRED)

add_executable(telematics 
    src/main.cpp
    src/Vehicle.cpp
    src/TelemetryEvent.cpp
)

target_link_libraries(telematics
	PRIVATE
	simdjson::simdjson
    SQLite::SQLite3
)

target_include_directories(telematics
    PRIVATE 
    include
)

