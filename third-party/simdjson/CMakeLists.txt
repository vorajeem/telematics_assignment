add_library(simdjson
    src/simdjson.cpp
)

add_library(simdjson::simdjson ALIAS simdjson)

target_include_directories(simdjson
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)