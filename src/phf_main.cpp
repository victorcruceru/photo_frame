#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>


#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#endif



int main(int argc, char** argv){
  if (argc < 2){
    std::cout << "Usage: "<< argv[0] << " <image_file>\n";
    return (EXIT_FAILURE);
  }
  std::error_code ec {std::error_code{}}; 
  if (! std::filesystem::exists(argv[1], ec)){
    std::cerr <<"ERROR: '"<< argv[1] << "' does not exist. " << "\n";
    return(EXIT_FAILURE); 
  }

  if( !gl3wInit() ){
    std::cerr <<"ERROR: gl3wInit() failed\n";
    return(EXIT_FAILURE);
  }

  return(EXIT_SUCCESS);
}