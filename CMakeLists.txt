cmake_minimum_required(VERSION 3.14)
project(Artemisa C)

set(CMAKE_C_STANDARD 99)
include_directories(./Artemisa/Core)
add_executable(Artemisa main.c Core/InlineASM_thumb.h Core/inlineASM_thumb.c Core/helper_types.h Core/helper_memAlloc.h Core/compileParse.h Core/coreConfig.h)