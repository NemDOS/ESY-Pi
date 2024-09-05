#!/bin/bash
# Orginalscript starten
echo "${TARGET_DIR}/../../board/raspberrypi4/post-image.sh $*"
echo
${TARGET_DIR}/../../board/raspberrypi4/post-image.sh $*
sudo cp output/images/rootfs.cpio /srv/tftp/
echo "Copy rootfs.cpio into tftp directory - Done"
