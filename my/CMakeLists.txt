cmake_minimum_required(VERSION 3.9)
project(my)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

add_executable(my main.cpp File.cpp File.h)