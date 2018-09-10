# Requires at least CMake 3.5
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (gSparse-Test-Suite)

# Download and build Google Test framework from github
add_subdirectory(google-test)

# enable CTest testing
enable_testing()

#####################################
# Add CSV Reader Test 
#####################################
add_executable(test-GraphCSVReader Test-GraphCSVReader.cpp)
# Link the test executable
target_link_libraries(test-GraphCSVReader
    GTest::GTest 
    GTest::Main
    Eigen3::Eigen
    gSparse::gSparse  # Header-only library
   # --coverage
)
#target_compile_options(test-GraphCSVReader PRIVATE --coverage)
add_test(NAME Test-CSVReader COMMAND test-GraphCSVReader)

#####################################
# Add CSV Writer Test 
#####################################
add_executable(test-GraphCSVWriter Test-GraphCSVWriter.cpp)
# Link the test executable
target_link_libraries(test-GraphCSVWriter
    GTest::GTest 
    GTest::Main
    Eigen3::Eigen
    gSparse::gSparse  # Header-only library
   # --coverage
)
#target_compile_options(test-GraphCSVWriter PRIVATE --coverage)
add_test(NAME Test-CSVWriter COMMAND test-GraphCSVWriter)

# Transfer files

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test-edges.csv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test-weight.csv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)


