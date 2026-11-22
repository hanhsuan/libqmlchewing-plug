#!/bin/bash

set -ex

LIBCHEWING_VERSION="${LIBCHEWING_VERSION:-0.8.5}"
LIBCHEWING_TAG="${INPUT_RELEASE}-v${LIBCHEWING_VERSION}"
BASE_URL="https://github.com/hanhsuan/SailfishOS-libchewing/releases/download/${LIBCHEWING_TAG}"
TARGET="SailfishOS-${INPUT_RELEASE}-${INPUT_ARCH}"

# 1. Ensure a shared build directory under /home/mersdk (visible inside sb2/mb2)
sudo mkdir -p /home/mersdk/build /home/mersdk/deps
sudo chown -R "$(id -u):$(id -g)" /home/mersdk

# 2. Download RPMs and install into the target AND its .default snapshot
for pkg in libchewing libchewing-devel; do
  rpm_file="${pkg}-${LIBCHEWING_VERSION}-0.${INPUT_ARCH:-aarch64}.rpm"
  curl -fsSL -o "/home/mersdk/deps/${rpm_file}" "${BASE_URL}/${rpm_file}"

  # Copy into /root inside both the target and its snapshot so snapshot reset keeps them
  for t in "${TARGET}" "${TARGET}.default"; do
    if [ -d "/srv/mer/targets/${t}" ]; then
      sudo cp "/home/mersdk/deps/${rpm_file}" "/srv/mer/targets/${t}/root/${rpm_file}"
      (cd / && sb2 -t "${t}" -m sdk-install -R rpm -Uvh --nodeps "/root/${rpm_file}")
    fi
  done
done

# 3. Copy source to /home/mersdk/build and run mb2
cd /home/mersdk/build
cp -r /workspace/* .
mb2 -t "${TARGET}" build

# 4. Copy RPMS back to /workspace/RPMS
sudo mkdir -p /workspace/RPMS
sudo cp -r RPMS/*.rpm /workspace/RPMS
