#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>


#include "SDL_video.h"
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

  cv::Mat image = cv::imread( argv[1], cv::IMREAD_COLOR );
  if( image.empty() ){
    std::cerr << "ERROR: failed to load image from file '"<< argv[1] <<"'\n";
    return (EXIT_FAILURE);
  }
  cv::cvtColor( image, image, cv::COLOR_BGR2RGBA );

  const char* glsl_version = "#version 100";
  

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | 
                                                   SDL_WINDOW_RESIZABLE | 
                                                   SDL_WINDOW_ALLOW_HIGHDPI|
                                                   SDL_WINDOW_FULLSCREEN);
  SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", 
                                                   SDL_WINDOWPOS_CENTERED, 
                                                   SDL_WINDOWPOS_CENTERED, 
                                                   1280, 720, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  std::cout << "DEBUG: GLSL version = " << glsl_version << "\n";
  if( (gl3wInit() != 0) ){
    std::cerr <<"ERROR: gl3wInit() failed\n";
    return(EXIT_FAILURE);
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); 
  (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);


  GLuint img_texture = 0;


  glGenTextures(1, &img_texture);
  glBindTexture(GL_TEXTURE_2D, img_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, 
               GL_RGBA, GL_UNSIGNED_BYTE, image.data);

  // Our state
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  bool done = false;
  while (!done){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        done = true;
      if (event.type == SDL_WINDOWEVENT && 
          event.window.event == SDL_WINDOWEVENT_CLOSE && 
          event.window.windowID == SDL_GetWindowID(window)) 
        done = true;
    }
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();    

    ImGui::Begin(argv[1]);
    ImGui::Text("pointer = %p", 
      reinterpret_cast<void*>(static_cast<intptr_t>(img_texture)));
    ImGui::Text("size = %d x %d", image.cols, image.rows);
    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(img_texture)), 
                ImVec2(image.cols, image.rows));
    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);    

  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();    
  return(EXIT_SUCCESS);
}