cmake_minimum_required(VERSION 3.10)
project(3002_2)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)


# 查找 SDL2 库
find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
# 查找 SDL_image 库
set(SDL2_IMAGE_FRAMEWORK_PATH "/Library/Frameworks/SDL2_image.framework")

# 添加包含目录
include_directories(${SDL2_INCLUDE_DIRS}
                    ${SDL2_IMAGE_FRAMEWORK_PATH}/Headers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
# 添加可执行文件
add_executable(MySDL2Project main.cpp
        defs.h
        struct.h
        init.cpp
        input.cpp
        draw.cpp
        draw.h
        common.h
        init.h
        input.h
        stage.cpp
        stage.h)

# 链接 SDL2 和 SDL_image 库
target_link_libraries(MySDL2Project ${SDL2_LIBRARIES}
                                    "-framework SDL2_image")
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_IMAGE_FRAMEWORK_PATH: ${SDL2_IMAGE_FRAMEWORK_PATH}")

