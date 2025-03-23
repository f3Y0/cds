cmake_minimum_required(VERSION 3.12)
project(cds VERSION 0.1.0 LANGUAGES C)

enable_testing()
include(CTest)

# Set C standard (adjust as needed)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(WITH_COVERAGE "Enable code coverage reporting" OFF)

if(WITH_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        # Add coverage flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    endif()
endif()

# List your source files
set(SOURCES
    src/vec.c
    # Add more source files as they're created
)

add_subdirectory(tests)

# Create a library target
add_library(cds ${SOURCES})

# Set include directories for the library
target_include_directories(cds
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Installation rules
install(TARGETS cds
    EXPORT cdsTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/cds
    DESTINATION include
)

# Generate and install export file
install(EXPORT cdsTargets
    FILE cdsTargets.cmake
    NAMESPACE cds::
    DESTINATION lib/cmake/cds
)

# Create a package configuration file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/cdsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cdsConfig.cmake
    INSTALL_DESTINATION lib/cmake/cds
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cdsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cdsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cdsConfigVersion.cmake
    DESTINATION lib/cmake/cds
)

