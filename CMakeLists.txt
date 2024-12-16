cmake_minimum_required(VERSION 3.26)
project(SDL2-Shoooter C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_subdirectory(src)

target_link_libraries(SDL2-Shoooter mingw32)
target_link_libraries(SDL2-Shoooter SDL2main)
target_link_libraries(SDL2-Shoooter SDL2)
target_link_libraries(SDL2-Shoooter SDL2_image)
target_link_libraries(SDL2-Shoooter SDL2_ttf)