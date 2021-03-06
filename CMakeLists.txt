cmake_minimum_required(VERSION 3.18)

# Define the project name
project(asm2048)

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Add all C source files under the src directory
file(GLOB_RECURSE C_SOURCES "src/*.c")

# Add all assembly source files under the src directory
option(USE_ASM "Use functions written in x86_64 assembly" ON)
if(USE_ASM)
  enable_language(ASM)
  add_compile_definitions(USE_ASM true)
  file(GLOB_RECURSE ASM_SOURCES "src/game/*.S")
endif(USE_ASM)

set(SOURCES ${C_SOURCES} ${ASM_SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

# Debug symbols
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)
# set(CMAKE_BUILD_TYPE Release)

# Add compiler errors/warnings flags
target_compile_options(${PROJECT_NAME} PRIVATE 
  -std=c18 -Wall -pedantic -Werror -Wshadow 
  -Wstrict-aliasing -Wstrict-overflow
  -Wunused-variable -Wunused-but-set-variable -Wunused-function
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