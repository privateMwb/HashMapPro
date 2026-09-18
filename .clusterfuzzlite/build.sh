#!/bin/bash -eu
# ============================================================
# .clusterfuzzlite/build.sh
#
# HashMapPro is header-only, so unlike a harness that needs to compile
# separate .cpp translation units first, this just compiles the fuzz
# target directly against the headers under include/.
#
# Add more `${SRC}/HashMapPro/fuzz/fuzz_*.cpp` harnesses here as
# they're added; each becomes its own $OUT binary.
# ============================================================

cd "${SRC}/HashMapPro"

$CXX $CXXFLAGS -std=c++20 \
  -I"${SRC}/HashMapPro/include" \
  fuzz/fuzz_hashmap.cpp \
  $LIB_FUZZING_ENGINE \
  -o "${OUT}/fuzz_hashmap"
