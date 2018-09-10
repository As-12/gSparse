# C++ Skeleton Header-only library 
# Building Project Demo/Examples

# Requires CMake 3.5
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (Skeleton-Demo)

# Add a demo executable and link it against the library
add_executable(demo-1 demo-1.cpp)
target_link_libraries(demo-1
    lib::myLibrary  # Header-only library
)

# Add a demo executable and link it against the library
add_executable(demo-2 demo-2.cpp)
target_link_libraries(demo-2
    lib::myLibrary  # Header-only library
)

