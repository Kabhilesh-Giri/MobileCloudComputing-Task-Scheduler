set(EXE_SOURCES "main.cpp")
set(EXE_INCLUDES "./")

add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${EXE_INCLUDES})

# Link with libraries
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${LIBRARY_NAME})

if(${WARNINGS_ENABLED})
    message(STATUS "COMPILER_WARNINGS function is called for ${EXECUTABLE_NAME}")
    target_set_warnings(
        ${EXECUTABLE_NAME}
        WARNINGS_ENABLED
        WARNINGS_ENABLED_AS_ERRORS)
endif()

if(${ENABLE_ADDRESS_SANITIZER} OR ${ENABLE_UNDEFINED_SANITIZER})
    Activate_Address_Sanitizer(${EXECUTABLE_NAME}
    ENABLE_ADDRESS_SANITIZER
    ENABLE_UNDEFINED_SANITIZER)
endif()

if(ENABLE_LTO_IPO)
    target_enable_lto(${EXECUTABLE_NAME}
    ENABLE)
endif()
