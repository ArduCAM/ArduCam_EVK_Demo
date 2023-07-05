cmake_minimum_required(VERSION 2.8.12)

if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif(POLICY CMP0048)

project(demo
    VERSION 0.1.0
    LANGUAGES C CXX
)

# option SDK_PATH "Path to the Arducam EVK SDK" ${CMAKE_CURRENT_SOURCE_DIR}/../install
# get_filename_component(ARDUCAM_EVK_SDK_PATH_DEF ${CMAKE_CURRENT_SOURCE_DIR}/../install ABSOLUTE)
# set(ARDUCAM_EVK_SDK_PATH ${ARDUCAM_EVK_SDK_PATH_DEF} CACHE PATH "Path to the Arducam EVK SDK")

# message(STATUS "ARDUCAM_EVK_SDK_PATH: ${ARDUCAM_EVK_SDK_PATH}")

# set(arducam_evk_sdk_DIR ${ARDUCAM_EVK_SDK_PATH}/lib/cmake)
# set(arducam_evk_cpp_sdk_DIR ${ARDUCAM_EVK_SDK_PATH}/lib/cmake)

# find_package( OpenSSL REQUIRED )

# if(NOT DEFINED spdlog_FOUND)
#     find_package(spdlog REQUIRED)
#     message(STATUS "spdlog_VERSION: ${spdlog_VERSION}")
# endif()

if(NOT DEFINED ZLIB_FOUND)
    find_package(ZLIB REQUIRED)
    message(STATUS "ZLIB_VERSION_STRING: ${ZLIB_VERSION_STRING}")
endif()

if(NOT DEFINED OpenCV_FOUND)
  find_package(OpenCV REQUIRED)
  message(STATUS "OpenCV version: ${OpenCV_VERSION}")
endif()

if(NOT DEFINED arducam_evk_sdk_FOUND)
  find_package(arducam_evk_sdk)
  message(STATUS "arducam_evk_sdk version: ${arducam_evk_sdk_VERSION}")
endif()

if(NOT DEFINED arducam_evk_cpp_sdk_FOUND)
  find_package(arducam_evk_cpp_sdk)
  message(STATUS "arducam_evk_cpp_sdk version: ${arducam_evk_cpp_sdk_VERSION}")
endif()


# include_directories(
#     ${arducam_evk_sdk_INCLUDE_DIR}
#     ${arducam_evk_cpp_sdk_INCLUDE_DIR}
# )
# link_directories(
#     ${arducam_evk_sdk_LIB_DIR}
#     ${arducam_evk_cpp_sdk_LIB_DIR}
# )

add_subdirectory(c-async)
add_subdirectory(c-sync)
add_subdirectory(c++-async)
add_subdirectory(c++-sync)
# add_subdirectory(python-async)
# add_subdirectory(python-sync)
