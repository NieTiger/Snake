cmake_minimum_required(VERSION 3.15)
project(snake C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(snake
        definitions.h
        main.c
        utilities.c
        utilities.h)

target_link_libraries(snake
        ncurses)