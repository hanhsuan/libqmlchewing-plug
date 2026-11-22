#!/bin/bash

set -ex

LIBCHEWING_VERSION="${LIBCHEWING_VERSION:-0.8.5}"
LIBCHEWING_TAG="${INPUT_RELEASE}-v${LIBCHEWING_VERSION}"
BASE_URL="https://github.com/hanhsuan/SailfishOS-libchewing/releases/download/${LIBCHEWING_TAG}"

download() {
  if command -v curl >/dev/null 2>&1; then
    curl -fsSL -o "$2" "$1"
  else
    wget -q -O "$2" "$1"
  fi
}

# Install libchewing (runtime + devel) for the target arch
for pkg in libchewing libchewing-devel; do
  rpm_file="${pkg}-${LIBCHEWING_VERSION}-0.${INPUT_ARCH}.rpm"
  download "${BASE_URL}/${rpm_file}" "/tmp/${rpm_file}"
  rpm -Uvh --nodeps "/tmp/${rpm_file}"
done

mkdir -p build
cd build
cp -r /workspace/* .
mb2 -t SailfishOS-${INPUT_RELEASE}-${INPUT_ARCH} build -- --with unittest
mkdir -p /workspace/RPMS
cp -r RPMS/*.rpm /workspace/RPMS