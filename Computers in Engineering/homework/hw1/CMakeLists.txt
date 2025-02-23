cmake_minimum_required(VERSION 3.19)
project(hw1)

set(CMAKE_CXX_STANDARD 11)

configure_file(cbb.csv cbb.csv COPYONLY)

add_executable(hw1 main.cpp)