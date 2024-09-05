HOMEDIR=`echo ~`

echo $HOMEDIR

export ARCH=arm
export CROSS_COMPILE=arm-linux-

# Nachfolgende Variablen muessen angepasst werden
export PATH="${PATH}:${HOMEDIR}/Hausarbeit/embedded/raspi/buildroot/output/host/usr/bin/"
export SYSROOT=${HOMEDIR}/Hausarbeit/embedded/raspi/buildroot/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot


export KDIR=${HOMEDIR}/Hausarbeit/embedded/raspi/linux/

# Raspberry Pi 4
export KERNEL=kernel7l
