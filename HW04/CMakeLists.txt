cmake_minimum_required(VERSION 3.8)
project(HW04)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES ConnectFour.cpp ConnectFour.h main.cpp)
add_executable(HW04 ${SOURCE_FILES})
