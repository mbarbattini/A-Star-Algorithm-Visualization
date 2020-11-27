cmake_minimum_required(VERSION 3.17)
project(FinalProject)

set(CMAKE_CXX_STANDARD 14)

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_executable(FinalProject main.cpp Spot.h Spot.cpp)

target_link_libraries(FinalProject sfml-graphics sfml-system sfml-window)