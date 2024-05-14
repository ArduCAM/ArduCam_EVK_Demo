cmake_minimum_required(VERSION 3.0)

include(../COption.cmake)

file(GLOB C_FILES "${CMAKE_SOURCE_DIR}/*.c")

foreach(C_FILE ${C_FILES})
  get_filename_component(EXECUTABLE_NAME ${C_FILE} NAME_WE)

  message(STATUS "Add executable: ${EXECUTABLE_NAME}")
  add_executable(${EXECUTABLE_NAME}
    ${C_FILE}
    main.c
  )

  target_link_libraries(${EXECUTABLE_NAME}
    ${CORE_LIBS}
  )

  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${EXECUTABLE_NAME} m)
  endif()

  copy_dll_to(${EXECUTABLE_NAME})
endforeach()
