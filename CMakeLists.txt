# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(test)

find_package(Sqlite3 REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main sqlite3)