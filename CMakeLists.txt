cmake_minimum_required(VERSION 3.17.3)
project(virus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#add_compile_options(-Wall -Wextra -pedantic -Werror)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(virus src/main.cpp src/antiVirus.cpp src/antiVirus.h src/grid.cpp src/grid.h src/field.cpp src/field.h src/country.cpp src/country.h src/district.cpp src/district.h src/iface.h src/logic.cpp src/logic.h src/graphics.cpp src/graphics.h src/protocol.cpp src/protocol.h testfiles/round1/ex3.h src/debug.h src/iface.cpp src/utils.cpp src/utils.h src/visualization.cpp src/visualization.h src/window.cpp src/window.h src/ai.cpp src/ai.h)
target_link_libraries(virus ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2::TTF)

include_directories(src)