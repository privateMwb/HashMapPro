# Packaging: Local Verification

Notes for re-verifying the vcpkg port and Conan recipe after changes
(e.g. a new release, editing `portfile.cmake`, or editing
`recipes/hashmappro/all/conanfile.py`). Scoped to what isn't obvious
from the files themselves — not a general build tutorial.

## Termux-specific quirks

Both package managers assume a "normal" Linux/CI environment.
Termux's Android-flavored profile detection breaks several of their
default assumptions. None of this applies on real CI (GitHub Actions
Ubuntu runner, a real Linux box, etc.) — it's purely a Termux problem.

- **`vcpkg`** — not installed on Termux; bootstrapping it needs a full
  C++ toolchain build and has known portability issues on bionic libc.
  Skip it. `sha512sum` produces the identical hash `vcpkg hash` would,
  and a plain `cmake --install` reproduces what `vcpkg_cmake_configure`
  + `vcpkg_cmake_install` do under the hood — that's enough to verify
  the port without the binary.

- **`conan profile detect`** auto-detects `os=Android`, which makes
  `CMakeToolchain` demand `tools.android:ndk_path` — Termux isn't
  actually cross-compiling, so override: `-s os=Linux`.

- **`compiler.libcxx`** auto-detects as `libstdc++11`, but Termux's
  Clang actually ships `libc++`. Wrong value forces `-stdlib=libstdc++`,
  which doesn't exist here and breaks every standard header include.
  Override: `-s compiler.libcxx=libc++`.

- **Host/build profile mismatch** — overriding `os` on the host profile
  only (`-s os=Linux`) while the build profile still says `os=Android`
  confuses Conan into partial cross-compile behavior (breaks include
  path resolution even for basic headers like `<cassert>`). If sticking
  with `CMakeToolchain`, override both: add `-s:b os=Linux` too.

- **Simpler alternative** — if `CMakeToolchain` keeps fighting the
  Android detection, drop it entirely. Use `[generators]` with just
  `CMakeDeps` in the consumer `conanfile.txt`, then build with a plain
  `cmake -B build -DCMAKE_PREFIX_PATH=$(pwd)` instead of the generated
  preset. Trade-off: `CMakeToolchain` normally sets `CMAKE_CXX_STANDARD`
  automatically from `compiler.cppstd`; without it, the consumer's own
  `CMakeLists.txt` must set the C++ standard explicitly, or every
  C++23 concept/`contiguous_iterator_tag` construct in the headers
  fails to compile.

- **`CMAKE_BUILD_TYPE` matters even without a toolchain file** —
  Conan's `CMakeDeps`-generated `INTERFACE_INCLUDE_DIRECTORIES` are
  gated behind `$<$<CONFIG:Release>:...>` generator expressions. If
  `CMAKE_BUILD_TYPE` isn't set to match (e.g. left empty), the include
  directories silently evaluate to nothing — no error, just a
  "file not found" on every header. Always pass
  `-DCMAKE_BUILD_TYPE=Release` explicitly when testing.

## Verifying the vcpkg port

1. Extract the release tarball and install to a staging prefix with
   the same flags `portfile.cmake` uses:
   ```bash
   tar xzf v<version>.tar.gz && cd HashMapPro-<version>
   cmake -B build \
     -DCMAKE_INSTALL_PREFIX=$HOME/staging \
     -DHASHMAPPRO_BUILD_TESTS=OFF \
     -DHASHMAPPRO_BUILD_BENCHMARKS=OFF \
     -DHASHMAPPRO_BUILD_TOOLS=OFF \
     -DHASHMAPPRO_BUILD_EXAMPLES=OFF
   cmake --build build
   cmake --install build
   ```
2. Confirm the CMake package files exist:
   ```bash
   find $HOME/staging -name "*.cmake"
   ```
   Expect `HashMapProConfig.cmake`, `HashMapProConfigVersion.cmake`,
   `HashMapProTargets.cmake` under `lib/cmake/HashMapPro/`.
# Packaging: Local Verification

Notes for re-verifying the vcpkg port and Conan recipe after changes
(e.g. a new release, editing `portfile.cmake`, or editing
`recipes/hashmappro/all/conanfile.py`). Scoped to what isn't obvious
from the files themselves — not a general build tutorial.

## Termux-specific quirks

Both package managers assume a "normal" Linux/CI environment.
Termux's Android-flavored profile detection breaks several of their
default assumptions. None of this applies on real CI (GitHub Actions
Ubuntu runner, a real Linux box, etc.) — it's purely a Termux problem.

- **`vcpkg`** — not installed on Termux; bootstrapping it needs a full
  C++ toolchain build and has known portability issues on bionic libc.
  Skip it. `sha512sum` produces the identical hash `vcpkg hash` would,
  and a plain `cmake --install` reproduces what `vcpkg_cmake_configure`
  + `vcpkg_cmake_install` do under the hood — that's enough to verify
  the port without the binary.

- **`conan profile detect`** auto-detects `os=Android`, which makes
  `CMakeToolchain` demand `tools.android:ndk_path` — Termux isn't
  actually cross-compiling, so override: `-s os=Linux`. This applies
  to **every** `conan` invocation that touches settings — both
  `conan create` and the consumer's `conan install` — not just the
  recipe build.

- **`compiler.libcxx`** auto-detects as `libstdc++11`, but Termux's
  Clang actually ships `libc++`. Wrong value forces `-stdlib=libstdc++`,
  which doesn't exist here and breaks every standard header include.
  Override: `-s compiler.libcxx=libc++` if you hit this (in practice
  the header-only recipe hasn't needed it, since no compilation happens
  during `conan create` — but consumer builds compiling real code may).

- **Host/build profile mismatch** — overriding `os` on the host profile
  only (`-s os=Linux`) while the build profile still says `os=Android`
  confuses Conan into partial cross-compile behavior (breaks include
  path resolution even for basic headers like `<cassert>`). If sticking
  with `CMakeToolchain`, override both: add `-s:b os=Linux` too.

- **Simpler alternative** — if `CMakeToolchain` keeps fighting the
  Android detection, drop it entirely. Use `[generators]` with just
  `CMakeDeps` in the consumer `conanfile.txt`, then build with a plain
  `cmake -B build -DCMAKE_PREFIX_PATH=$(pwd)` instead of the generated
  preset. Trade-off: `CMakeToolchain` normally sets `CMAKE_CXX_STANDARD`
  automatically from `compiler.cppstd`; without it, the consumer's own
  `CMakeLists.txt` must set the C++ standard explicitly, or every
  C++23 concept/`contiguous_iterator_tag` construct in the headers
  fails to compile.

- **`CMAKE_BUILD_TYPE` matters even without a toolchain file** —
  Conan's `CMakeDeps`-generated `INTERFACE_INCLUDE_DIRECTORIES` are
  gated behind `$<$<CONFIG:Release>:...>` generator expressions. If
  `CMAKE_BUILD_TYPE` isn't set to match (e.g. left empty), the include
  directories silently evaluate to nothing — no error, just a
  "file not found" on every header. Always pass
  `-DCMAKE_BUILD_TYPE=Release` explicitly when testing.

- **Intermittent `EOFError` during `conan create`'s `source()` step** —
  `Uncompressing v1.0.0.tar.gz to .` sometimes fails with `EOFError:
  Compressed file ended before the end-of-stream marker was reached`,
  even though Conan already logged `Sources correctly downloaded`
  (i.e. the sha256 check on the raw bytes passed). This was confirmed
  to be flaky Termux networking, not a real bug: the identical tarball
  extracts cleanly via `tar tzf` and Python's own `tarfile` module every
  time, and a full `~/.conan2/p` cache wipe didn't change the outcome —
  only a plain retry did. If this happens, just rerun `conan create`
  again; don't chase it as a code or hash problem.

## Header layout note (affects `conanfile.py`'s `package()`)

Headers live at `include/HashMapPro/*.{h,tpp}` (namespaced
subdirectory), not flatly at `include/*.h`. The recipe uses
`strip_root=False` in `source()`, which means the extracted tree keeps
its GitHub-archive wrapper folder (`HashMapPro-<version>/`) — so
`package()` must join `self.source_folder` with that wrapper
explicitly before pointing at `include/`, e.g.:
```python
root = os.path.join(self.source_folder, f"HashMapPro-{self.version}")
copy(self, "*.h", src=os.path.join(root, "include"), dst=...)
```
Conan's `copy()` tool matches patterns like `"*.h"` against the
filename only (not the full relative path), so it already recurses
into subdirectories like `HashMapPro/` without needing `**/*.h`. If
`package()` reports `WARN: No files in this package!`, the `root`
path is wrong for the current `strip_root` setting — that's the
actual thing to check, not the glob pattern.

## Verifying the vcpkg port

1. Extract the release tarball and install to a staging prefix with
   the same flags `portfile.cmake` uses:
   ```bash
   tar xzf v<version>.tar.gz && cd HashMapPro-<version>
   cmake -B build \
     -DCMAKE_INSTALL_PREFIX=$HOME/staging \
     -DHASHMAPPRO_BUILD_TESTS=OFF \
     -DHASHMAPPRO_BUILD_BENCHMARKS=OFF \
     -DHASHMAPPRO_BUILD_TOOLS=OFF \
     -DHASHMAPPRO_BUILD_EXAMPLES=OFF
   cmake --build build
   cmake --install build
   ```
   No "manually-specified variables were not used" warning should
   appear — `portfile.cmake` now passes `HASHMAPPRO_BUILD_*`, matching
   the option names actually declared in `CMakeLists.txt`.
2. Confirm the CMake package files exist:
   ```bash
   find $HOME/staging -name "*.cmake"
   ```
   Expect `HashMapProConfig.cmake`, `HashMapProConfigVersion.cmake`,
   `HashMapProTargets.cmake` under `lib/cmake/HashMapPro/`. If testing
   in a `$HOME/staging` prefix shared with other libraries (e.g.
   VectorPro), leftover `lib/cmake/<OtherLib>/` entries from previous
   runs are harmless — just noise, not a bug.
3. In a **separate** directory, with a minimal consumer project
   (`find_package(HashMapPro CONFIG REQUIRED)` +
   `target_link_libraries(app PRIVATE HashMapPro::HashMapPro)`):
   ```bash
   cmake -B build -DCMAKE_PREFIX_PATH=$HOME/staging
   cmake --build build
   ./build/app
   ```

## Verifying the Conan recipe

1. From repo root:
   ```bash
   conan create recipes/hashmappro/all --version <version> \
     -s compiler.cppstd=23 -s os=Linux
   ```
   Watch for: `validate()` passing (C++23 floor check), the `.h`/`.tpp`
   files actually listed in `package(): Packaged N '.h' files: ...`
   (not `WARN: No files in this package!`), `LICENSE` landing in
   `package(): Packaged ... LICENSE`, and a single `package_id`
   regardless of settings (confirms `self.info.clear()` is working for
   the header-only package). If it fails with the `EOFError` described
   above, just retry — see Termux quirks.
2. In a **separate** directory, with `conanfile.txt`:
   ```
   [requires]
   hashmappro/<version>

   [generators]
   CMakeDeps
   ```
   plus the same minimal consumer `CMakeLists.txt`/`main.cpp` as above:
   ```bash
   conan install . -s compiler.cppstd=23 -s os=Linux --build=missing
   cmake -B build -DCMAKE_PREFIX_PATH=$(pwd) -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ./build/app
   ```
   `-s os=Linux` is needed here too, same as the `conan create` step —
   easy to forget since it's a separate command in a separate directory.

A simple consumer `main.cpp` is enough — insert a couple of keys and
print a lookup:
```cpp
#include <HashMapPro/HashMap.h>
#include <iostream>

int main() {
    HashMapPro::HashMap<int, std::string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    std::cout << map.at(1) << " " << map.at(2) << "\n";
}
```
Both should print `one two` — that confirms the package is actually
consumable, not just that it builds.


## Verifying the Conan recipe

1. From repo root:
   ```bash
   conan create recipes/hashmappro/all --version <version> \
     -s compiler.cppstd=23 -s os=Linux
   ```
   Watch for: `validate()` passing (C++23 floor check), `LICENSE`
   landing in `package(): Packaged ... LICENSE`, and a single
   `package_id` regardless of settings (confirms `self.info.clear()`
   is working for the header-only package).
2. In a **separate** directory, with `conanfile.txt`:
   ```
   [requires]
   hashmappro/<version>

   [generators]
   CMakeDeps
   ```
   plus the same minimal consumer `CMakeLists.txt`/`main.cpp` as above:
   ```bash
   conan install . -s compiler.cppstd=23 --build=missing
   cmake -B build -DCMAKE_PREFIX_PATH=$(pwd) -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ./build/app
   ```

A simple consumer `main.cpp` is enough — insert a couple of keys and
print a lookup:
```cpp
#include <HashMapPro/HashMap.h>
#include <iostream>

int main() {
    HashMapPro::HashMap<int, std::string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    std::cout << map.at(1) << " " << map.at(2) << "\n";
}
```
Both should print `one two` — that confirms the package is actually
consumable, not just that it builds.
