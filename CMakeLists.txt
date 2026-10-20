cmake_minimum_required(VERSION 3.20)
project(PokemonUnboundLauncher)

set(CMAKE_CXX_STANDARD 17)

# SDL2 e libmgba vão ser linkados estáticos
find_package(SDL2 REQUIRED)

# Baixa o core do mGBA automaticamente
include(FetchContent)
FetchContent_Declare(mgba GIT_REPOSITORY https://github.com/mgba-emu/mgba.git GIT_TAG 0.10.3)
FetchContent_MakeAvailable(mgba)

add_executable(PokemonUnboundLauncher WIN32 src/main.cpp)
target_link_libraries(PokemonUnboundLauncher PRIVATE SDL2::SDL2 SDL2::SDL2main mgba)
target_include_directories(PokemonUnboundLauncher PRIVATE ${mgba_SOURCE_DIR}/include)
