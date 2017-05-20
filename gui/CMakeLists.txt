cmake_minimum_required(VERSION 3.7)
project(gui)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.c)
add_executable(gui ${SOURCE_FILES})

include(FindGTK2)
if (GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
    link_directories(${GTK2_LIBRARY_DIRS})
    link_libraries(${GTK2_LIBRARIES})
endif (GTK2_FOUND)