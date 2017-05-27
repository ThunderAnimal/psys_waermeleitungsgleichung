cmake_minimum_required(VERSION 3.7)
project(serial)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        core.c
        core.h
        core_test.c
        heat_conduction.c)

add_executable(serial ${SOURCE_FILES})