vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/HashMapPro
    REF v1.0.0
    SHA512 1a63ad09d2c62b619d6d977b27f7f295be1def5178ccc8d6b98e2d7f3d01005ac6914ecf1eebe65bcee4a2e06f1ba33dbe36a14cc36ce0aecba99758255560f1
)

# HashMapPro is header-only; consumers only need the library itself,
# not the tests, benchmarks, tools, or examples. tools/ also fetches
# nlohmann_json via FetchContent at configure time, which requires
# network access that vcpkg's build sandbox does not allow.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DHASHMAPPRO_BUILD_TESTS=OFF
        -DHASHMAPPRO_BUILD_BENCHMARKS=OFF
        -DHASHMAPPRO_BUILD_TOOLS=OFF
        -DHASHMAPPRO_BUILD_EXAMPLES=OFF
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
