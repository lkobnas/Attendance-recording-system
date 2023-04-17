cmake_minimum_required(VERSION 3.7.0)

project(pn532test VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(GTest REQUIRED)

include_directories(../)

enable_testing()
add_executable(pn532test test.cpp)

# Add wiringPi library
find_library(WIRINGPI_LIB wiringPi)

target_link_libraries(pn532test gtest)

