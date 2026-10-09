# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:3cf598908df31d5bcc8013f9040e7689daef8cbeedd0401789158fb1abd42cf4

COPY . $SRC/HashMapPro
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/HashMapPro
