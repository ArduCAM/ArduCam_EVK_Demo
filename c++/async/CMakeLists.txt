set(APP_NAME demo_cpp_async)

cmake_minimum_required(VERSION 3.0)

project(${APP_NAME} C CXX)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_STANDARD 17)
  message(STATUS "optional: C++17")
else(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_STANDARD 20)
  message(STATUS "optional: C++20")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g2 -ggdb -fsanitize=address -fsanitize=leak")
if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")

if(NOT DEFINED OpenCV_FOUND)
  find_package(OpenCV REQUIRED)
  message(STATUS "OpenCV version: ${OpenCV_VERSION}")
endif()

if(NOT DEFINED arducam_evk_cpp_sdk_FOUND)
  find_package(arducam_evk_cpp_sdk)
  message(STATUS "arducam_evk_cpp_sdk version: ${arducam_evk_cpp_sdk_VERSION}")
endif()

include_directories(
  ${arducam_evk_cpp_sdk_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
)

set(WITH_OPENCV_WORLD OFF CACHE BOOL "with opencv_world")
set(CORE_LIBS)
if(WITH_OPENCV_WORLD)
  set(CORE_LIBS
    ${CORE_LIBS}
    opencv_world)
else()
  set(CORE_LIBS
    ${CORE_LIBS}
    ${OpenCV_LIBS})
endif()

add_executable(${APP_NAME}
  main.cpp
  utils.cpp
)

target_link_libraries(${APP_NAME}
  ${arducam_evk_cpp_sdk_LIBS}
  ${CORE_LIBS}
)
