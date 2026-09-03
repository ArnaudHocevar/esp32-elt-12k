cmake_minimum_required(VERSION 3.15)
project(esphome_modbus_tracker CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Look for the hidden ESPHome compilation directory where esphome.h lives
# (Change "modbus-smart-poller" to match the exact 'name' inside your YAML)
set(ESPHOME_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.esphome/build/esp32-elt-12k/src")

# 2. Add the path to the compiler search tree
include_directories(include ${ESPHOME_BUILD_DIR})

# 3. Unit test target
enable_testing()
find_package(doctest REQUIRED)
include(doctest)
add_executable(test_device_string
        tests/test_device_string.cpp
    src/device_string.cpp
    include/device_string.h
)
target_link_libraries(test_device_string PRIVATE doctest::doctest)
doctest_discover_tests(test_device_string)