#!/bin/sh
while true
do
    echo "waiting for input ..."
    read
    echo "switching led to ${REPLY}"
    echo -e -n "\x${REPLY}" > /dev/led_onoff_an
done
