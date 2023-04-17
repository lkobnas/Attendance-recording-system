cmake_minimum_required(VERSION 3.7.0)

project(database VERSION 1.0.0 LANGUAGES CXX)

#file(REMOVE_RECURSE ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Qt5 COMPONENTS Core Gui Widgets Sql REQUIRED)
find_package(GTest REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(../)

enable_testing()
add_executable(main test.cpp)

target_link_libraries(main Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql sqlite3 gtest)
target_link_libraries(main qwt-qt5)

#install(TARGETS test)
