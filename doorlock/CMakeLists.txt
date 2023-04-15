cmake_minimum_required(VERSION 3.10)
project(gpio_example)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

find_package(Threads REQUIRED)
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

add_executable(gpio_example main.cpp GpioController.cpp)
target_link_libraries(gpio_example ${WIRINGPI_LIBRARIES} Threads::Threads)
