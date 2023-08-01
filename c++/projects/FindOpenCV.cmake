cmake_minimum_required(VERSION 3.0)

if (NOT DEFINED Find_OpenCV_CMAKE_OPTION)
    set(Find_OpenCV_CMAKE_OPTION ON)

    if(NOT DEFINED OpenCV_FOUND)
    find_package(OpenCV REQUIRED)
    message(STATUS "OpenCV version: ${OpenCV_VERSION}")
    endif()

    include_directories(
    ${OpenCV_INCLUDE_DIRS}
    )

    set(WITH_OPENCV_WORLD OFF CACHE BOOL "with opencv_world")
    if(WITH_OPENCV_WORLD)
    set(CORE_LIBS
        ${CORE_LIBS}
        opencv_world)
    else()
    set(CORE_LIBS
        ${CORE_LIBS}
        ${OpenCV_LIBS})
    endif()
endif()
