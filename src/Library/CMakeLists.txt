set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Task.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Mobile_Cloud_Computing.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Task_Graph.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.cpp"
)

set(LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Task.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Mobile_Cloud_Computing.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Task_Graph.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.hpp"
)

set(LIBRARY_INCLUDES
    "./" #when ever we use hpp files this ./ is needed and declared in this format
    "${CMAKE_BINARY_DIR}/configured_files/include") # Add include directory for config.hpp

    add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS})

    target_include_directories(${LIBRARY_NAME} PUBLIC ${LIBRARY_INCLUDES})

if(WARNINGS_ENABLED)
    target_set_warnings(${LIBRARY_NAME}
    WARNINGS_ENABLED.
    WARNINGS_ENABLED_AS_ERRORS)
endif()

if(ENABLE_ADDRESS_SANITIZER OR ENABLE_UNDEFINED_SANITIZER)
    Activate_Address_Sanitizer(${LIBRARY_NAME}
    ENABLE_ADDRESS_SANITIZER
    ENABLE_UNDEFINED_SANITIZER)
endif()

if(ENABLE_LTO_IPO)
    target_enable_lto(${EXECUTABLE_NAME}
    ENABLE)
endif()
