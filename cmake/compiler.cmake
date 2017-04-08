include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)
include(CheckCXXSymbolExists)

function(CheckCompilerCXX11Flag)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 4.8)
            message(FATAL_ERROR "Unsupported GCC version. GCC >= 4.8 required.")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # cmake >= 3.0 compiler id "AppleClang" on Mac OS X, otherwise "Clang"
        # Apple Clang is a different compiler than upstream Clang which havs different version numbers.
        # https://gist.github.com/yamaya/2924292
        if(APPLE)  # cmake < 3.0 compiler id "Clang" on Mac OS X
            if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 5.1)
                message(FATAL_ERROR "Unsupported AppleClang version. AppleClang >= 5.1 required.")
            endif()
        else()
            if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.3)
                message(FATAL_ERROR "Unsupported Clang version. Clang >= 3.3 required.")
            endif()
        endif()   
    endif()
endfunction()

CheckCompilerCXX11Flag()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
