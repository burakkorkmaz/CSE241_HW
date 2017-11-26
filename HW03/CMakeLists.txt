cmake_minimum_required(VERSION 3.7)
project(HW03)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ConnectFour.cpp ConnectFour.h)
add_executable(HW03 ${SOURCE_FILES})