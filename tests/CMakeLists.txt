# Tests CMakeLists.txt

# Find required packages for testing
# If you're using a testing framework like Check, cmocka, or Unity:
# find_package(Check REQUIRED)  # Uncomment if using Check

find_package(cmocka REQUIRED)

# Find all test files
file(GLOB_RECURSE TEST_SOURCES "*.c")

# Create an executable for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract the test name from the file path
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    # Create the test executable
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    # Link against your library and cmocka
    target_link_libraries(${TEST_NAME} PRIVATE cds cmocka)
    
    # Include directories
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    
    # Register with CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

