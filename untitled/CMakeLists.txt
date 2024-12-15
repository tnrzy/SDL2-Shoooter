cmake_minimum_required(VERSION 3.26)
project(untitled C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_subdirectory(src)

target_link_libraries(untitled mingw32)
target_link_libraries(untitled SDL2main)
target_link_libraries(untitled SDL2)
target_link_libraries(untitled SDL2_image)
target_link_libraries(untitled SDL2_ttf)