# Function to enable compiler warnings
function(target_set_warnings TARGET WARNINGS_ENABLED WARNINGS_ENABLED_AS_ERRORS)
    # If warnings are not enabled, return
    if(NOT ${WARNINGS_ENABLED})
        message(STATUS "Warnings are not enabled for target: ${TARGET}")
        return()
    endif()

    # Define warning options for different compilers
    set(MSVC_WARNINGS
        /W4
        /permissive-
    )

    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wpedantic
    )

    set(GCC_WARNINGS ${CLANG_WARNINGS})

    # Enable warnings as errors if specified
    if(${WARNINGS_AS_ERRORS})
        set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
        set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
        set(GCC_WARNINGS ${GCC_WARNINGS} -Werror)
    endif()

    # Determine the compiler and set appropriate warnings
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(STATUS "MSVC is enabled")
        set(WARNINGS ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "Clang is enabled")
        set(WARNINGS ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "GNU is enabled")
        set(WARNINGS ${GCC_WARNINGS})
    else()
        message(WARNING "Unsupported compiler for target: ${TARGET}")
        return()
    endif()

    # Apply the compile options to the target
    target_compile_options(${TARGET} PRIVATE ${WARNINGS})
    message(STATUS ${WARNINGS})
    message(STATUS "TARGET IS ${TARGET}")

    message(STATUS "Warnings set for target: ${TARGET}")
endfunction(target_set_warnings)
