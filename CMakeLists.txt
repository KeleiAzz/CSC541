cmake_minimum_required(VERSION 3.3)
project(CSC541_hw)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    hw1/assn_1.c
    hw1/key.db
    hw1/out.txt
    hw1/output.txt
    hw1/seek.db hw2/main.c hw2/main.h)

add_executable(CSC541_hw ${SOURCE_FILES})