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

project(PhotoFrame CXX)
project(PhotoFrame VERSION ${FHP_VERSION})

# CONFIG directs CMake doesnt search for modules into the 
# default modules directory
#find_package(imgui CONFIG)
#find_package(glfw CONFIG)
#find_package(glew CONFIG)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Werror)

set (SDL2_CONFIG /usr/local/bin/sdl2-config)
execute_process(COMMAND 
    ${SDL2_CONFIG} --prefix 
    OUTPUT_VARIABLE SDL_INCLUDE_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "SDL_INCLUDE_PREFIX is ${SDL_INCLUDE_PREFIX}")

execute_process(COMMAND 
    ${SDL2_CONFIG} --cflags
    OUTPUT_VARIABLE SDL_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "SDL_CFLAGS is ${SDL_CFLAGS}")

execute_process(COMMAND 
    ${SDL2_CONFIG} --libs
    OUTPUT_VARIABLE SDL_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "SDL_LIBS is: ${SDL_LIBS}")



set(ENV{PKG_CONFIG_PATH} /opt/vc/lib/pkgconfig)
execute_process(COMMAND 
    pkg-config --cflags bcm_host brcmglesv2
    OUTPUT_VARIABLE BCM_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "BCM_CFLAGS is ${BCM_CFLAGS}")

execute_process(COMMAND 
    pkg-config --libs bcm_host brcmglesv2 brcmegl
    OUTPUT_VARIABLE BCM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "BCM_LIBS is: ${BCM_LIBS}")

add_library(dearimgui SHARED 
    third_party/imgui/examples/imgui_impl_sdl.cpp 
    third_party/imgui/examples/imgui_impl_opengl3.cpp
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp 
    third_party/imgui/examples/libs/gl3w/GL/gl3w.c                
)
set_target_properties(dearimgui PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(dearimgui PRIVATE
    third_party/imgui
    third_party/imgui/examples
    third_party/imgui/examples/libs/gl3w
    ${SDL_INCLUDE_PREFIX}/include
)

target_compile_definitions(dearimgui PUBLIC 
    IMGUI_IMPL_OPENGL_LOADER_GL3W 
    IMGUI_IMPL_OPENGL_ES2
    __RASPBIAN__)


add_executable(demo_imgui
    third_party/imgui/examples/example_sdl_opengl3/main.cpp
)


target_include_directories(demo_imgui PRIVATE
    third_party/imgui
    third_party/imgui/examples
    third_party/imgui/examples/libs/gl3w
    ${SDL_INCLUDE_PREFIX}/include
)


target_compile_definitions(demo_imgui PUBLIC 
    IMGUI_IMPL_OPENGL_LOADER_GL3W 
    IMGUI_IMPL_OPENGL_ES2
    __RASPBIAN__)

target_link_libraries(demo_imgui dearimgui -ldl ${SDL_LIBS} ${BCM_LIBS})

find_package( OpenCV REQUIRED )
add_executable( image_imgui src/phf_main.cpp )
target_link_libraries( image_imgui dearimgui -ldl -lstdc++fs
                       ${SDL_LIBS} 
                       ${BCM_LIBS} 
                       ${OpenCV_LIBS} )
target_include_directories(image_imgui PRIVATE
    third_party/imgui
    third_party/imgui/examples
    third_party/imgui/examples/libs/gl3w
    ${SDL_INCLUDE_PREFIX}/include
)


target_compile_definitions(image_imgui PUBLIC 
    IMGUI_IMPL_OPENGL_LOADER_GL3W 
    IMGUI_IMPL_OPENGL_ES2
    __RASPBIAN__)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SDL_CFLAGS} ${BCM_CFLAGS}")



