cmake_minimum_required(VERSION 3.7.0)

project(RFID VERSION 1.0.0 LANGUAGES CXX)

#file(REMOVE_RECURSE ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(RFID
  pn532.cpp
  pn532_rpi.cpp
  rpi_get_uid.cpp
  main.cpp
)

target_link_libraries(RFID wiringPi)

