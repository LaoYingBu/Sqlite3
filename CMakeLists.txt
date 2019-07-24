# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(test)

# turn on C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(db.cmake)
include(cv.cmake)

add_executable(main main.cpp)

target_link_libraries(main db cv)   