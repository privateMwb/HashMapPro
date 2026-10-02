# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:0b571c53b7180e7266de67665bc3ccef35618593af5714fee0750d611dc8b1f6

COPY . $SRC/HashMapPro
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/HashMapPro
