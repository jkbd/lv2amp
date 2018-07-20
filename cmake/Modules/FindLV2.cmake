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

# CMake find_package() Module for the LV2 audio plugin standard
# http://lv2plug.in/
#
# If successful the following variables will be defined
#   LV2_FOUND
#   LV2_INCLUDE_DIRS
#
# Example usage:
#   find_package(LV2)
#   include_directories(${LV2_INCLUDE_DIRS})

if (LV2_INCLUDE_DIRS)
  # in cache already
  set(LV2_FOUND TRUE)
else (LV2_INCLUDE_DIRS)
  find_path(LV2_INCLUDE_DIR
    NAMES
      lv2.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
  )
  set(LV2_INCLUDE_DIRS ${LV2_INCLUDE_DIR})

  if (LV2_INCLUDE_DIRS)
    set(LV2_FOUND TRUE)
  endif (LV2_INCLUDE_DIRS)

  mark_as_advanced(LV2_INCLUDE_DIRS)

  if (LV2_FOUND)
    if (NOT LV2_FIND_QUIETLY)
      message(STATUS "Found LV2: ${LV2_INCLUDE_DIR}")
    endif (NOT LV2_FIND_QUIETLY)
  else (LV2_FOUND)
    if (LV2_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find LV2")
    endif (LV2_FIND_REQUIRED)
  endif (LV2_FOUND)
endif (LV2_INCLUDE_DIRS)
