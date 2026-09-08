cmake_minimum_required(VERSION 3.15)
project(esphome_modbus_tracker CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ESPHOME_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.esphome/build/esp32-elt-12k/components")

include_directories(include ${ESPHOME_BUILD_DIR})
include_directories(components/device_string)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

enable_testing()
find_package(doctest REQUIRED)
include(doctest)
add_executable(test_device_string
        tests/test_device_string.cpp
    components/device_string/device_string_parser.cpp
    components/device_string/device_string_parser.h
)
target_link_libraries(test_device_string PRIVATE doctest::doctest)
doctest_discover_tests(test_device_string)
