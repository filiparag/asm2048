cmake_minimum_required(VERSION 3.17)

# Define the project name
project(2048)

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Add all C source files under the src directory
file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

# Add compiler errors/warnings flags
target_compile_options(${PROJECT_NAME} PRIVATE 
  -std=c18 -Wall -pedantic -Werror -Wshadow 
  -Wstrict-aliasing -Wstrict-overflow -lm
  -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function
)

# Add SDL2 library
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main)

# Add SDL2_ttf library
find_package(SDL2_ttf REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::TTF)

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})