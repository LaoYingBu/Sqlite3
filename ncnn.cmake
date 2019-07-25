if(MSVC)
    set(NCNN_DIR "E:\\InferenceCNN\\ncnn\\out\\install\\x64-Release")   
    include_directories(${NCNN_DIR}\\include)
    # add_library(ncnn STATIC IMPORTED)
    # set_target_properties(ncnn PROPERTIES IMPORTED_LOCATION ${NCNN_DIR}\\lib\\ncnn.lib)
    link_directories(${NCNN_DIR}\\lib)

    set(OpenCV_DIR "E:\\opencv_4100\\opencv\\build")
    include_directories(${OpenCV_DIR}/include)
    link_directories(${OpenCV_DIR}\\x64\\vc14\\lib)
    list(APPEND NCNN_LINKER_LIBS opencv_world410.lib ncnn)

endif()

file(GLOB NCNN_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include/ncnn.hpp)
file(GLOB NCNN_SRC ${CMAKE_CURRENT_LIST_DIR}/ncnn.cpp)
set(NCNN_COMPILE_CODE ${NCNN_INCLUDE} ${NCNN_SRC})

add_definitions(-DNCNN_EXPORT)
add_library(ncnnlib SHARED ${NCNN_COMPILE_CODE})
target_link_libraries(ncnnlib ${NCNN_LINKER_LIBS})