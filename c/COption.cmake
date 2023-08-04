cmake_minimum_required(VERSION 3.0)

if (NOT DEFINED C_CMAKE_OPTION)
    set(C_CMAKE_OPTION ON)

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

    if(NOT DEFINED arducam_evk_sdk_FOUND)
        find_package(arducam_evk_sdk)
        message(STATUS "arducam_evk_sdk version: ${arducam_evk_sdk_VERSION}")
    endif()

    include_directories(
        ${arducam_evk_sdk_INCLUDE_DIR}
    )

    set(CORE_LIBS ${arducam_evk_sdk_LIBS})
endif()
