# Prevent in-source builds to keep the source directory clean
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "In-source builds are not allowed. Please use a separate build directory (e.g., mkdir build && cd build && cmake ..).")
endif()

# Ensure the CMAKE_BUILD_TYPE variable is set, with a default fallback to Debug
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected. Defaulting to Debug.")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Output a message for the selected build type to give feedback to the user
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Build type is Release: Optimized for performance.")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Build type is Debug: Includes debug symbols.")

elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "Build type is RelWithDebInfo: Debug symbols with some optimizations.")

elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "Build type is MinSizeRel: Optimized for minimal size.")

else()
    message(WARNING "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

# Provide an example usage in the comments
# Example: Change the Makefile like this
# prepare:
# 	rm -rf build
# 	mkdir build
# 	(cd build && cmake .. &&  cmake -DCMAKE_BUILD_TYPE = Release .. && cmake --build .)

# Build type options:
#   Debug       : Includes debug symbols, no optimizations.
#   Release     : Fully optimized, no debug symbols.
#   RelWithDebInfo : Debug symbols with some optimizations.
#   MinSizeRel  : Optimized for minimal size.
