cmake_minimum_required(VERSION 3.8)
project(HW05)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ConnectFourAbstract.h ConnectFourPlus.cpp ConnectFourPlus.h ConnectFourDiag.cpp ConnectFourDiag.h ConnectFourPlusUndo.cpp ConnectFourPlusUndo.h Cell.cpp Cell.h ConnectFourAbstract.cpp)
add_executable(HW05 ${SOURCE_FILES})