if(MSVC)
    set(OpenCV_DIR "E:\\opencv_3300\\source.build\\install")
    #set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${OpenCV_DIR})
    #set(CMAKE_PREFIX_PATH ${OpenCV_DIR})
    
    # add_library(opencv STATIC IMPORTED)
    # set_target_properties(opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/x64/vc12/lib/opencv_core2413.lib
    #                                         IMPORTED_LOCATION ${OpenCV_DIR}/x64/vc12/lib/opencv_highgui2413.lib)

    include_directories(${OpenCV_DIR}/include)
    link_directories(${OpenCV_DIR}\\x64\\vc14\\lib)
    list(APPEND CV_LINKER_LIBS opencv_world330.lib)

endif()

file(GLOB CV_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include/cv.hpp)
file(GLOB CV_SRC ${CMAKE_CURRENT_LIST_DIR}/cv.cpp)
set(CV_COMPILE_CODE ${CV_INCLUDE} ${CV_SRC})

add_definitions(-DCV_EXPORT)
add_library(cv SHARED ${CV_COMPILE_CODE})
target_link_libraries(cv ${CV_LINKER_LIBS})