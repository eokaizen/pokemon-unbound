cmake_minimum_required(VERSION 3.20)
project(PokemonUnboundLauncher)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.8
)
FetchContent_MakeAvailable(SDL2)
add_executable(PokemonUnboundLauncher WIN32 src/main.cpp)
target_link_libraries(PokemonUnboundLauncher PRIVATE SDL2::SDL2 SDL2::SDL2main)
