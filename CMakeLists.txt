# BSD 2-Clause License
#
# Copyright (c) 2020, Victor Cruceru
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

cmake_minimum_required(VERSION 3.4)

set(FHP_VERSION_MAJOR 0)
set(FHP_VERSION_MINOR 0)
set(FHP_VERSION_PATCH 1)
set(FHP_VERSION ${FHP_VERSION_MAJOR}.${FHP_VERSION_MINOR}.${FHP_VERSION_PATCH})

project(PhotoFrame VERSION ${FHP_VERSION} CXX)

# CONFIG directs CMake doesnt search for modules into the 
# default modules directory
find_package(imgui CONFIG)
find_package(glfw CONFIG)
find_package(glew CONFIG)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wpedantic -Wall -Werror)

add_executable(photo_frame
    src/phf_main.cpp
    third_party/imgui/examples/imgui_impl_sdl.cpp 
    third_party/imgui/examples/imgui_impl_opengl3.cpp
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp 
    third_party/imgui/examples/libs/gl3w/GL/gl3w.c                
)

target_include_directories(photo_frame PRIVATE
    third_party/imgui
    third_party/imgui/examples
    third_party/imgui/examples/libs/gl3w
)

target_compile_definitions(photo_frame PUBLIC IMGUI_IMPL_OPENGL_LOADER_GL3W)


