cmake_minimum_required(VERSION 3.12)
project(trunk C)

set(CMAKE_C_STANDARD 99)

add_executable(trunk test/main/test_main.h test/main/test_main.c src/main/soucet.c src/main/soucet.h)