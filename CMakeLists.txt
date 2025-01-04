cmake_minimum_required(VERSION 3.22)

project(Project_MobileCloudComputing
    VERSION 1.0.0
    LANGUAGES C CXX) # Specifies the project uses C and C++ compilers

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Ensure the compiler supports the standard
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

option(WARNINGS_ENABLED "Warnings Enabled : " ON)
option(WARNINGS_ENABLED_AS_ERRORS "Warnings As Errors Enabled : " OFF)

option(ENABLE_ADDRESS_SANITIZER "Enabled Address Sanitizer : " ON)
option(ENABLE_UNDEFINED_SANITIZER "Enabled Undefined Sanitizer : " ON)
option(ENABLE_SANITIZE_LEAK "Enabled Leak Sanitizer : " ON)
option(ENABLE_SANITIZE_THREAD "Enabled Thread Sanitizer : " ON)

option(ENABLE_LTO_IPO "Enabled LTO/IPO : " ON)

# Define variables for the executable and library names
set(EXECUTABLE_NAME Executable)
set(LIBRARY_NAME Library)

# Using list(APPEND ...) for CMAKE_MODULE_PATH is a safer practice, as it avoids overwriting existing paths
# set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)

if(WARNINGS_ENABLED)
    include(Warnings)
endif()

if(ENABLE_ADDRESS_SANITIZER OR ENABLE_UNDEFINED_SANITIZER)
    include(Sanitizers)
endif()

if(ENABLE_LTO_IPO)
    include(LTO)
endif()

#Always Remember to put the add_subdirectory after defining all the include() and find_package() addiotionals flags

# Add subdirectories for modular structure
add_subdirectory(app)  # Contains the main executable
add_subdirectory(src)  # Contains the library
add_subdirectory(configured)

# Print debugging messages
message(STATUS "Executable Name: ${EXECUTABLE_NAME}")
message(STATUS "Library Name: ${LIBRARY_NAME}")
