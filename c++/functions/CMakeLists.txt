set(APP_NAME demo_cpp_functions)

cmake_minimum_required(VERSION 3.0)

project(${APP_NAME} 
    LANGUAGES C CXX
)

include(../CppOption.cmake)

aux_source_directory(. SRC_LIST)

add_executable(${APP_NAME}
  ${SRC_LIST}
)

target_link_libraries(${APP_NAME}
  ${CORE_LIBS}
)
