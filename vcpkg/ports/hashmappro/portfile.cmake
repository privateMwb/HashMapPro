vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/HashMapPro
    REF v1.0.0
    SHA512 038f3639f43f03e829fe3d9f14f715e41088cb16acbeeb0ceb6b7a9e05df05b946b727031839435d74a354a1e10db06e322ecd812cdcdf6be4cdbe912dd9dd60
)

# HashMapPro is header-only; consumers only need the library itself,
# not the tests, benchmarks, tools, or examples. tools/ also fetches
# nlohmann_json via FetchContent at configure time, which requires
# network access that vcpkg's build sandbox does not allow.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DVECTORPRO_BUILD_TESTS=OFF
        -DVECTORPRO_BUILD_BENCHMARKS=OFF
        -DVECTORPRO_BUILD_TOOLS=OFF
        -DVECTORPRO_BUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME HashMapPro
)

file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug"
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)