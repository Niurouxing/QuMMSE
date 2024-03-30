cmake_minimum_required(VERSION 3.14)
project(QuMMSE VERSION 0.1.0 LANGUAGES C CXX)

 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_subdirectory(QuBLAS)

add_executable(QuMMSE main.cpp)

target_link_libraries(QuMMSE PUBLIC QuBLAS)

 