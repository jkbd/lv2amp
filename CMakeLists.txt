cmake_minimum_required(VERSION 3.10)
project(amp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# LV2 dependency
find_package(LV2 REQUIRED)
include_directories(${LV2_INCLUDE_DIRS})

# Position independed code is set automatically
add_library(${PROJECT_NAME} MODULE
    src/Amp.cpp src/Amp.h)
