cmake_minimum_required(VERSION 3.23)
project(OCR C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(pre_processing)
include_directories(XOR)

add_executable(OCR
        pre_processing/load_image.c
        XOR/Maths.c
        XOR/Maths.h
        XOR/reseau_test.c)
