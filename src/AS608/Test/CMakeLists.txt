cmake_minimum_required(VERSION 3.7.0)

project(as608test VERSION 1.0.0 LANGUAGES CXX)

#file(REMOVE_RECURSE ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(GTest REQUIRED)

include_directories(../)

enable_testing()
add_executable(as608test test.cpp)

# Add wiringPi library
find_library(WIRINGPI_LIB wiringPi)

target_link_libraries(as608test gtest)

#install(TARGETS test)