if (NOT DEFINED C_CMAKE_OPTION)
    set(C_CMAKE_OPTION ON)

    option(WITH_ADDRESS_SANITIZE "Use address sanitize" OFF)
    option(WITH_LEAK_SANITIZE "Use leak sanitize" OFF)

    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_C_STANDARD 17)
        message(STATUS "optional: C++17")
    else(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_STANDARD 20)
        set(CMAKE_C_STANDARD 17)
        message(STATUS "optional: C++20")
    endif(CMAKE_COMPILER_IS_GNUCXX)

    if(WITH_ADDRESS_SANITIZE)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    endif(WITH_ADDRESS_SANITIZE)
    if(WITH_LEAK_SANITIZE)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=leak")
    endif(WITH_LEAK_SANITIZE)

    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG /MTd")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    else(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g2 -ggdb")
    endif(MSVC)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")

    if(NOT DEFINED arducam_evk_sdk_FOUND)
        # TODO: download arducam_evk_sdk package when windows platform
        find_package(arducam_evk_sdk)
        message(STATUS "arducam_evk_sdk version: ${arducam_evk_sdk_VERSION}")
        if(WIN32)
            get_filename_component(EVK_BIN_DIR "${arducam_evk_sdk_DIR}" PATH)
            get_filename_component(EVK_BIN_DIR "${EVK_BIN_DIR}" PATH)
            get_filename_component(EVK_BIN_DIR "${EVK_BIN_DIR}" PATH)
            set(EVK_BIN_DIR ${EVK_BIN_DIR}/bin)
            set(EVK_BIN_DEP
                "${EVK_BIN_DIR}/arducam_config_parser.dll"
                "${EVK_BIN_DIR}/arducam_controller.dll"
                "${EVK_BIN_DIR}/arducam_evk_cpp_sdk.dll"
                "${EVK_BIN_DIR}/arducam_evk_sdk.dll"
            )
            function(copy_dll_to TARGET_NAME)
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
                        ${EVK_BIN_DEP}
                        $<TARGET_FILE_DIR:${TARGET_NAME}>)
            endfunction()
        else(WIN32)
            function(copy_dll_to TARGET_NAME)
            endfunction()
        endif(WIN32)
    endif()

    set(CORE_LIBS arducam_evk_sdk)
endif()
