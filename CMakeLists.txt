cmake_minimum_required(VERSION 3.17.3)
project(virus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(virus src/main.cpp src/antiVirus.cpp src/antiVirus.h src/grid.cpp src/grid.h src/field.cpp src/field.h src/country.cpp src/country.h src/district.cpp src/district.h src/iface.cpp src/iface.h src/logic.cpp src/logic.h src/graphics.cpp src/graphics.h src/protocol.cpp src/protocol.h src/fileio.cpp src/fileio.h src/random.h src/random.cpp src/examples.h testfiles/ex3.h)

include_directories(src)