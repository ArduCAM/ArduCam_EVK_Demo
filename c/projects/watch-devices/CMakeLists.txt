set(APP_NAME demo_c_watch_devices)

cmake_minimum_required(VERSION 3.0)

project(${APP_NAME} 
    LANGUAGES C CXX
)

include(../../COption.cmake)
# include(../FindOpenCV.cmake)

add_executable(${APP_NAME}
  main.c
)

target_link_libraries(${APP_NAME}
  ${CORE_LIBS}
)

copy_dll_to(${APP_NAME})
