# Function to enable Sanitizer for a specific target
function(Activate_Address_Sanitizer TARGET ENABLE_ADDRESS_SANITIZER ENABLE_UNDEFINED_SANITIZER)
    # Check if sanitizers are enabled
    if(NOT ENABLE_ADDRESS_SANITIZER AND NOT ENABLE_UNDEFINED_SANITIZER)
        message(STATUS "Sanitizers are not enabled for target: ${TARGET}")
        return()
    endif()

    # Determine the compiler and set appropriate options
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        if(ENABLE_SANITIZE_ADDR)
            message(STATUS "Activating Address Sanitizer")
            add_compile_options("/fsanitize=address")
        endif()

        if(ENABLE_SANITIZE_UNDEF)
            message(STATUS "sanitize=undefined not avail. for MSVC")
        endif()

        if(ENABLE_SANITIZE_LEAK)
            message(STATUS "sanitize=leak not avail. for MSVC")
        endif()

        if(ENABLE_SANITIZE_THREAD)
            message(STATUS "sanitize=thread not avail. for MSVC")
        endif()

        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            add_compile_options("-fno-omit-frame-pointer")
            add_link_options("-fno-omit-frame-pointer")

        if(ENABLE_SANITIZE_ADDR)
            message(STATUS "Activating Address Sanitizer")
            add_compile_options("-fsanitize=address")
            add_link_options("-fsanitize=address")
        endif()

        if(ENABLE_SANITIZE_UNDEF)
            message(STATUS "Activating Undefined Sanitizer")
            add_compile_options("-fsanitize=undefined")
            add_link_options("-fsanitize=undefined")
        endif()

        if(ENABLE_SANITIZE_LEAK)
            add_compile_options("-fsanitize=leak")
            add_link_options("-fsanitize=leak")
        endif()

        if(ENABLE_SANITIZE_THREAD)
            if(ENABLE_SANITIZE_ADDR OR ENABLE_SANITIZE_LEAK)
                message(WARNING "thread does not work with: address and leak")
            endif()
            message(STATUS "Activating Thread Sanitizer")
            add_compile_options("-fsanitize=thread")
            add_link_options("-fsanitize=thread")
        endif()
    else()
        message(WARNING "Sanitizers are not supported for the compiler: ${CMAKE_CXX_COMPILER_ID}")
        return()
    endif()

    message(STATUS "Sanitizers applied to target: ${TARGET}")
endfunction()
