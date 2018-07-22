cmake_minimum_required(VERSION 3.10)
project(amp)

set(LV2_URI_PREFIX https://github.com/jkbd/)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# LV2 dependency
find_package(LV2 REQUIRED)
include_directories(${LV2_INCLUDE_DIRS})

# LV2 bundle directory
set(LV2_BUNDLE_DIR "${PROJECT_NAME}.lv2")
add_custom_target(bundle
    COMMAND ${CMAKE_COMMAND} -E
        make_directory ${LV2_BUNDLE_DIR})

# Position independed code is set automatically
add_library(${PROJECT_NAME} MODULE
    src/Amp.hpp src/Amp.cpp)
add_dependencies(${PROJECT_NAME} bundle)
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${LV2_BUNDLE_DIR})

configure_file(data/manifest.ttl.in ${PROJECT_BINARY_DIR}/${LV2_BUNDLE_DIR}/manifest.ttl)
configure_file(data/seealso.ttl.in ${PROJECT_BINARY_DIR}/${LV2_BUNDLE_DIR}/${PROJECT_NAME}.ttl)
