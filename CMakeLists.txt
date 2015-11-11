cmake_minimum_required(VERSION 3.3)
project(CSC541_hw)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    hw1/assn_1.c
    hw1/key.db
    hw1/out.txt
    hw1/output.txt
    hw1/seek.db hw2/main.c hw2/main.h hw3/main.c hw3/main.h hw3/testSort.c hw3/testSort.h hw3/heap_sort.c hw3/heap_sort.h hw2/random.c hw2/random.h hw4/main.c hw4/main.h)

add_executable(CSC541_hw ${SOURCE_FILES})