echo "preparing environment for this shell"
HOMEDIR=$(echo ~)
export ARCH=arm
export CROSS_COMPILE=arm-linux-
export PATH="${PATH}:/home/nemdos/Hausarbeit/embedded/raspi/buildroot/output/host/usr/bin/"
export KERNEL=kernel7l

export SYSROOT=/home/nemdos/Hausarbeit/embedded/raspi/buidroot/output/host/usr/arm-buildroot-linux-uclibcgnueabihf/sysroot/
export KDIR=/home/nemdos/Hausarbeit/embedded/raspi/linux/
