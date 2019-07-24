#设置项目的第三方库的inculde和lib
if(MSVC)
    set(sqlite3 "E:\\Vcpkg\\vcpkg\\packages\\sqlite3_x64-windows" CACHE SRRING "include path")
    include_directories(${sqlite3}/include)
    link_directories(${sqlite3}/lib)
    list(APPEND SQLITE3_LINKER_LIBS sqlite3)
endif()


#整理源文件结构
file(GLOB SQLITE3_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
                          ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB SQLITE3_SRC ${CMAKE_CURRENT_LIST_DIR}/db.cpp)
set(SQLITE3_COMPILE_CODE ${SQLITE3_INCLUDE} ${SQLITE3_SRC})

#添加预编译宏
add_definitions(-DSQLITE3_EXPORT)

add_library(db SHARED ${SQLITE3_COMPILE_CODE})
target_link_libraries(db ${SQLITE3_LINKER_LIBS})