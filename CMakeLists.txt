cmake_minimum_required(VERSION 3.20)

project(
    HashMapPro
    VERSION 1.0.9
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
# FunctionPro (Header-only Library)
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

# Build the test executable.
add_executable(
    tests
    tests/test_all.cpp
    tests/test_constructor.cpp
    tests/test_access.cpp
    tests/test_modifiers.cpp
    tests/test_lookup.cpp
    tests/test_iterator.cpp
    tests/test_capacity.cpp
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

# Build the benchmarks executable
add_executable(
    benchmarks
    benchmarks/bench_all.cpp
    benchmarks/bench_constructor.cpp
    benchmarks/bench_modifiers.cpp
    benchmarks/bench_lookup.cpp
    benchmarks/bench_access.cpp
    benchmarks/bench_iterator.cpp
)

# Link HashMapPro to the benchmarks executable
target_link_libraries(
    benchmarks
    PRIVATE
    HashMapPro
)


# ============================================================================
# Examples
# ============================================================================

# Build every example in the examples/ directory.
file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp"
)

foreach(example ${EXAMPLE_SOURCES})
    get_filename_component(target "${example}" NAME_WE)

    add_executable(${target} "${example}")

    target_link_libraries(
        ${target}
        PRIVATE
        HashMapPro
    )
endforeach()


# ============================================================================
# Installation
# ============================================================================

# Install the HashMapPro target.
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

# Generate the package file.
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