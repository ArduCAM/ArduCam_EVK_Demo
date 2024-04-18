cmake_minimum_required(VERSION 3.0)

include(../CppOption.cmake)

file(GLOB CPP_FILES "${CMAKE_SOURCE_DIR}/*.cpp")

foreach(CPP_FILE ${CPP_FILES})
  get_filename_component(EXECUTABLE_NAME ${CPP_FILE} NAME_WE)
  
  message(STATUS "Add executable: ${EXECUTABLE_NAME}")
  add_executable(${EXECUTABLE_NAME} 
    ${CPP_FILE}
    ./argtable3/argtable3.c
  )

  target_link_libraries(${EXECUTABLE_NAME}
    ${CORE_LIBS}
  )

  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${EXECUTABLE_NAME} m)
  endif()

  copy_dll_to(${EXECUTABLE_NAME})
endforeach()
