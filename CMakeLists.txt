# ============================================================
# CMakeLists.txt
# Build configuration for the HashMapPro library.
# ============================================================
#
# Sections:
#   1. C++ Configuration
#   2. HashMapPro Library
#   3. Subdirectories
#   4. Installation
#   5. Packaging
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

# Place all executables directly in the build directory, regardless of
# which subdirectory defines them (e.g. build/tests, build/compare).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

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
# Subdirectories
# ============================================================================

# Consumers (e.g. vcpkg) should disable these — only the library itself
# needs to be configured/installed for downstream projects. Defaults to
# ON so local development and existing CI workflows are unaffected.
option(VECTORPRO_BUILD_TESTS "Build the HashMapPro test suite" ON)
option(VECTORPRO_BUILD_BENCHMARKS "Build the HashMapPro benchmark suite" ON)
option(VECTORPRO_BUILD_TOOLS "Build the compare/regression benchmark tools" ON)
option(VECTORPRO_BUILD_EXAMPLES "Build the HashMapPro examples" ON)

enable_testing()

if(VECTORPRO_BUILD_TESTS)
    add_subdirectory(tests tests_build)
endif()

if(VECTORPRO_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks benchmarks_build)
endif()

if(VECTORPRO_BUILD_TOOLS)
    add_subdirectory(tools tools_build)
endif()

if(VECTORPRO_BUILD_EXAMPLES)
    add_subdirectory(examples examples_build)
endif()

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