#!/bin/sh
# Orginalskript starten 

echo "Starting original post-build script"
echo "${TARGET_DIR}/../../board/raspberrypi4/post-build.sh $*"
${TARGET_DIR}/../../board/raspberrypi4/post-build.sh $*

# Netzwerk 
echo "Configuring Network - LAN ..."
cp ../target/interfaces ${TARGET_DIR}/etc/network

echo "Configuring Network - WIFI ..."
install ${TARGET_DIR}/../../../target/dnsmasq.conf ${TARGET_DIR}/etc/
install ${TARGET_DIR}/../../../target/wpa_supplicant.conf ${TARGET_DIR}/etc/
install ${TARGET_DIR}/../../../target/resolv.conf ${TARGET_DIR}/etc/
install ${TARGET_DIR}/../../../target/S61wpa ${TARGET_DIR}/etc/init.d/
install ${TARGET_DIR}/../../../target/S99firewall ${TARGET_DIR}/etc/init.d/
install ${TARGET_DIR}/../../../target/sysctl.conf ${TARGET_DIR}/etc/

echo "copying modules ..."
cp -r ../target/lib/modules/6.11.0-rc6-v7l+/ ${TARGET_DIR}/lib/modules/
# MARK C: Zeit setzen 
# ntpd -g wont work not on network
# MARK D: SD-Karte einhaengen 
# mount -a
# MARK F: Firewall 
