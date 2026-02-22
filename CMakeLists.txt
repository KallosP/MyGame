cmake_minimum_required(VERSION 3.0.0)
project(my_game VERSION 1.0.0)

find_package(OpenGL REQUIRED)

add_executable(my_game 
    src/config.h 
    src/main.cpp 
    src/glad.c
    src/triangle_mesh.cpp
    src/triangle_mesh.h
)

target_include_directories(my_game PRIVATE dependencies) # PRIVATE keyword = only my_game gets to include items from dependencies directory, it's not propogated to targets that link to my_game

target_link_libraries(my_game glfw3 OpenGL::GL) # NOTE: installed glfw3 with 'pacman -S mingw-w64-x86_64-glfw' in msys terminal, so that install of glfw is being referenced