# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(test)

# turn on C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(db.cmake)

# set(sqlite3_path "E:\\Vcpkg\\vcpkg\\packages\\sqlite3_x86-windows")
# include_directories("${sqlite3_path}\\include")
# link_directories("${sqlite3_path}\\lib")
if(MSVC)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
    link_directories(${CMAKE_BINARY_DIR})
    list(APPEND MAIN_LIBS db)
endif()


add_executable(main main.cpp)
target_link_libraries(main ${MAIN_LIBS})   