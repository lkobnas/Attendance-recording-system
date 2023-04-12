cmake_minimum_required(VERSION 3.10)
project(CameraCapture)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} /home/punchin/cppcodec)

add_executable(CameraCapture main.cpp camera_capture.cpp)

target_link_libraries(CameraCapture ${OpenCV_LIBS} pthread)
