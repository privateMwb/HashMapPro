from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy, get
import os


class HashMapProConan(ConanFile):
    name = "hashmappro"
    version = "1.0.0"

    package_type = "header-library"

    license = "MIT"
    author = "privateMwb"
    url = "https://github.com/privateMwb/HashMapPro"
    description = (
        "High-performance std::unordered_map-compatible "
        "separate-chaining hash map."
    )
    topics = (
        "hashmap",
        "container",
        "header-only",
        "cpp",
        "data-structure",
    )

    settings = "os", "compiler", "build_type", "arch"

    no_copy_source = True

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, 23)

    def source(self):
        get(
            self,
            **self.conan_data["sources"][self.version],
            strip_root=False,
        )

    def package(self):
        root = os.path.join(
            self.source_folder,
            f"HashMapPro-{self.version}",
        )

        copy(
            self,
            "*.h",
            src=os.path.join(root, "include"),
            dst=os.path.join(self.package_folder, "include"),
        )

        copy(
            self,
            "*.hpp",
            src=os.path.join(root, "include"),
            dst=os.path.join(self.package_folder, "include"),
        )

        copy(
            self,
            "*.tpp",
            src=os.path.join(root, "include"),
            dst=os.path.join(self.package_folder, "include"),
        )

        copy(
            self,
            "LICENSE",
            src=root,
            dst=os.path.join(self.package_folder, "licenses"),
        )

    def package_info(self):
        self.cpp_info.set_property(
            "cmake_file_name",
            "HashMapPro",
        )

        self.cpp_info.set_property(
            "cmake_target_name",
            "HashMapPro::HashMapPro",
        )

        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

    def package_id(self):
        self.info.clear()