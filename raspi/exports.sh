echo "preparing environment for this shell"
HOMEDIR=$(echo ~)
export ARCH=arm
export CROSS_COMPILE=arm-linux-
export PATH="${PATH}:${HOMEDIR}/Hausarbeit/embedded/raspi/buildroot/output/host/usr/bin/"
export KERNEL=kernel7l # fuer Raspberry Pi 4
