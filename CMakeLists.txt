# ============================================================
# CMakeLists.txt
# Build configuration for the HashMapPro library.
# ============================================================
#
# Sections:
#   1. C++ Configuration
#   2. HashMapPro Library
#   3. Tests
#   4. Benchmarks
#   5. Examples
#   6. Installation
#   7. Packaging
#
# ============================================================

cmake_minimum_required(VERSION 3.20)

project(
    HashMapPro
    VERSION 1.0.0
    LANGUAGES CXX
)

# ============================================================================
# C++ Configuration
# ============================================================================

# Configure the C++ language standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# HashMapPro (Header-only Library)
# ============================================================================

# Define the header-only HashMapPro library.
add_library(HashMapPro INTERFACE)

# Expose the public include directory to consumers.
target_include_directories(
    HashMapPro
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Require C++23 for all targets that consume HashMapPro.
target_compile_features(
    HashMapPro
    INTERFACE
    cxx_std_23
)

# ============================================================================
# Tests
# ============================================================================

# Collect all test source files.
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

# Build the test executable.
add_executable(
    tests
    ${TEST_SOURCES}
)

# Allow simple test includes.
target_include_directories(
    tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# Link HashMapPro to the test executable.
target_link_libraries(
    tests
    PRIVATE
    HashMapPro
)

# ============================================================================
# Benchmarks
# ============================================================================

# Collect all benchmark source files.
file(GLOB_RECURSE BENCH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cpp"
)

# Build the benchmark executable.
add_executable(
    benchmarks
    ${BENCH_SOURCES}
)

# Allow simple benchmark includes.
target_include_directories(
    benchmarks
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks
)

# Link HashMapPro to the benchmark executable.
target_link_libraries(
    benchmarks
    PRIVATE
    HashMapPro
)

# ============================================================================
# Examples
# ============================================================================

# Collect all example source files.
file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp"
)

# Build the example executable.
add_executable(
    examples
    ${EXAMPLE_SOURCES}
)

# Allow simple example includes.
target_include_directories(
    examples
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/examples
)

# Link HashMapPro to the example executable.
target_link_libraries(
    examples
    PRIVATE
    HashMapPro
)

# ============================================================================
# Installation
# ============================================================================

# Install the HashMapPro target and export it for downstream projects.
install(
    TARGETS HashMapPro
    EXPORT HashMapProTargets
)

# Install the exported CMake targets.
install(
    EXPORT HashMapProTargets
    FILE HashMapProTargets.cmake
    NAMESPACE HashMapPro::
    DESTINATION lib/cmake/HashMapPro
)

# Install the public header files.
install(
    DIRECTORY include/
    DESTINATION include
)

# ============================================================================
# Packaging
# ============================================================================

include(CMakePackageConfigHelpers)

# Generate the package configuration file.
configure_package_config_file(
    cmake/HashMapProConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/HashMapProConfig.cmake
    INSTALL_DESTINATION lib/cmake/HashMapPro
)

# Generate the package version file.
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/HashMapProConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install the package configuration files.
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/HashMapProConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/HashMapProConfigVersion.cmake
    DESTINATION lib/cmake/HashMapPro
)