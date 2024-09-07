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

echo "copying blinking-led-thingy ..."
cp ../driver/signalan.ko ${TARGET_DIR}/lib/modules/
install ${TARGET_DIR}/../../../target/S70signalan ${TARGET_DIR}/etc/init.d/

echo "mosquitto bzzzzzzz ..." 
install ${TARGET_DIR}/../../../target/S85mosquitto_pub ${TARGET_DIR}/etc/init.d/
install ${TARGET_DIR}/../../../target/S90mosquitto_sub ${TARGET_DIR}/etc/init.d/
install ${TARGET_DIR}/../../../target/mosq_pub.sh ${TARGET_DIR}/etc/init.d/
install ${TARGET_DIR}/../../../target/sw_led_an.sh ${TARGET_DIR}/etc/
install ${TARGET_DIR}/../../../target/mosquitto.conf ${TARGET_DIR}/etc/mosquitto/
