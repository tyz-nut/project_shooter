# 标题
cmake_minimum_required(VERSION 3.21.0)
project(project_shooter VERSION 0.1.0 LANGUAGES C CXX)

# 设置c++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译选项
add_compile_options(-Wall -Wextra -Werror -pedantic)

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 查找并载入cmake预设
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

# 添加可执行文件
add_executable(${TARGET} 
                src/main.cpp 
                src/game.cpp 
                src/scene.cpp 
                src/sceneMain.cpp
                src/sceneTitle.cpp 
                src/sceneEnd.cpp 
                src/object.h )

# 链接库
target_link_libraries(${TARGET} 
                        SDL3::SDL3
                        SDL3_image::SDL3_image
                        SDL3_mixer::SDL3_mixer
                        SDL3_ttf::SDL3_ttf)