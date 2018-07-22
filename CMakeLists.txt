# MIT License
#
# Copyright (c) 2018 Jakob Dübel
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.10)

# For versioning read https://semver.org/ and
# http://lv2plug.in/ns/lv2core/lv2core.html#minorVersion
project(amp
    VERSION 0.1.0
    LANGUAGES CXX C)

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

install(DIRECTORY ${PROJECT_BINARY_DIR}/${LV2_BUNDLE_DIR} DESTINATION $ENV{HOME}/.lv2)
