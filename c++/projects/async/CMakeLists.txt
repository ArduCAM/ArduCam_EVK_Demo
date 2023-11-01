set(APP_NAME demo_cpp_async)

cmake_minimum_required(VERSION 3.0)

project(${APP_NAME} 
    LANGUAGES C CXX
)

include(../../CppOption.cmake)
include(../FindOpenCV.cmake)

find_package(Threads REQUIRED)

add_executable(${APP_NAME}
  main.cpp
  utils.cpp
)

target_link_libraries(${APP_NAME}
  ${CORE_LIBS}
  Threads::Threads
)
