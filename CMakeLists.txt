cmake_minimum_required(VERSION 3.18)

# Define the project name
project(asm2048)

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Add all C source files under the src directory
file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

# Debug symbols
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)
# set(CMAKE_BUILD_TYPE Release)

# Add compiler errors/warnings flags
target_compile_options(${PROJECT_NAME} PRIVATE 
  -std=c18 -Wall -pedantic -Werror -Wshadow 
  -Wstrict-aliasing -Wstrict-overflow
  # -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function
)

# Add SDL2 library
find_package(SDL2 REQUIRED)

# Add SDL2_gfx library
find_package(SDL2_gfx REQUIRED)

# Add SDL2_ttf library
find_package(SDL2_ttf REQUIRED)

target_link_libraries(${PROJECT_NAME} m SDL2::Main SDL2::GFX SDL2::TTF)

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})