cmake_minimum_required(VERSION 3.25)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)

#find_library(OpenCV)
find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 23)

add_executable(untitled1 main.cpp main.h algoritm.cpp)

target_link_libraries(untitled1 ${OpenCV_LIBS})