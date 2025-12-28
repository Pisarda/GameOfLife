cmake_minimum_required(VERSION 3.14)

project(GameOfLife)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(SDL3)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")

add_executable(GameOfLife ${SOURCES})

target_include_directories(GameOfLife PRIVATE
    ${SDL3_SOURCE_DIR}/include
    "src"
)

target_link_libraries(GameOfLife PRIVATE SDL3::SDL3)
