cmake_minimum_required(VERSION 3.17.3)
project(virus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(virus src/main.cpp src/antiVirus.cpp src/antiVirus.h src/grid.cpp src/grid.h src/field.cpp src/field.h src/country.cpp src/country.h src/district.cpp src/district.h src/iface.h src/logic.cpp src/logic.h src/graphics.cpp src/graphics.h src/protocol.cpp src/protocol.h testfiles/ex3.h src/debug.h src/iface.cpp src/utils.cpp src/utils.h)

include_directories(src)