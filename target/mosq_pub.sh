#!/bin/sh


while true
do
    mosquitto_pub -t system/hostname -m "$(hostname -s)"
    mosquitto_pub -t system/ipaddress/eth0 -m "$(ifconfig eth0 | grep 'inet addr:' | cut -d: -f2| cut -d ' ' -f1)"
    mosquitto_pub -t system/ipaddress/wlan0 -m "$(ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2| cut -d ' ' -f1)"
    mosquitto_pub -t system/uptime -m "$(uptime)"
    mosquitto_pub -t system/date -m "$(date)"
    mosquitto_pub -t system/Panagou -m "Panagou"
    mosquitto_pub -t appl/frequency -m "$(cat /dev/led_onoff_an)"
    sleep 1
done
