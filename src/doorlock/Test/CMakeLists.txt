cmake_minimum_required(VERSION 3.0)

project(doorlockTest)

# Compile fingerprint.cpp and utils.c as C++ files
set(SOURCES 
    main.cpp
    ../doorlock.cpp

)

set(CMAKE_CXX_STANDARD 11)

# Add wiringPi library
find_library(WIRINGPI_LIB wiringPi)

# Add executable and link with wiringPi
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${WIRINGPI_LIB})
