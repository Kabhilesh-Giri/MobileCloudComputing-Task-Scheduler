# Function to enable Link Time Optimization (LTO) or Inter-Procedural Optimization (IPO) for a target
# This function checks if the compiler supports IPO/LTO and applies it conditionally.
# Arguments:
#   - TARGET: The name of the build target (e.g., executable or library).
#   - ENABLE: A boolean value (TRUE/FALSE or ON/OFF) indicating whether LTO/IPO should be enabled.

function(target_enable_lto TARGET ENABLE)
    # If LTO/IPO is not requested (ENABLE is FALSE), return immediately and do nothing.
    if(NOT ${ENABLE})
        return()
    endif()

    # Include the CheckIPOSupported module, which provides the `check_ipo_supported` macro.
    include(CheckIPOSupported)

    # Check if IPO/LTO is supported by the current compiler and environment.
    # Syntax check_ipo_supported(RESULT <result_var> OUTPUT <output_var>)
    # The macro outputs:
    #   - RESULT: A boolean indicating support (TRUE if supported, FALSE otherwise).
    #   - OUTPUT: Diagnostic or error messages from the check process.
    #   - result: This will store "YES" if it supported
    check_ipo_supported(RESULT result OUTPUT output)

    # If IPO/LTO is supported, enable it for the specified target.
    if(result)
        # Inform the user that IPO/LTO is supported.
        message(STATUS "IPO/LTO is supported!")

        # Set the INTERPROCEDURAL_OPTIMIZATION property for the target to ENABLE (TRUE).
        # This enables IPO/LTO for the build process of the target.
        set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION ${ENABLE})
    else()
        # If IPO/LTO is not supported, issue a warning to the user.
        message(WARNING "IPO/LTO is not supported!")
    endif()
endfunction(target_enable_lto)

# Example Usage:
# target_enable_lto(my_target TRUE)
# This will enable IPO/LTO for the build target `my_target` if supported by the compiler.
