vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/HashMapPro
    REF v1.0.0
    SHA512 0732282c09dd7acf75494dc2b859103f77d11ddf76fb680bb72328ffd12bb5a2946d4bf7c98c8476839e7ca9704cd20eda93cf308929ed9c058b1a77549c2494
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