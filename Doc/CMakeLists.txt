# C++ Skeleton Header-only library 
# Generating Doxygen Documentation

# Set the project name
project (Skeleton-Doc)

# Require CMake 3.5
cmake_minimum_required (VERSION 3.5)

# Requires a pre-installed Doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxygen.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/DoxygenConfig)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

