# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:b1f0fce811b848e117a4809f8ccf1bdf94d1b5d5406d19e8512ac3f0ce958b3c

COPY . $SRC/HashMapPro
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/HashMapPro
