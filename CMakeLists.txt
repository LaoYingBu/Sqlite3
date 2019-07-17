# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(test)

set(sqlite3_path "E:\\Vcpkg\\vcpkg\\packages\\sqlite3_x86-windows")
include_directories("${sqlite3_path}\\include")
link_directories("${sqlite3_path}\\lib")
add_executable(main main.cpp)
target_link_libraries(main sqlite3)