#!/bin/bash

for device in yuga odin pollux pollux_windy dogo; do

DEFCONFIG_FILE=fusion3_${device}_defconfig

if [ ! -e arch/arm/configs/$DEFCONFIG_FILE ]; then
	echo "No such file: arch/arm/configs/$DEFCONFIG_FILE"
	exit -1
fi

# make .config
make ARCH=arm CROSS_COMPILE=arm-eabi- ${DEFCONFIG_FILE} -j8

mv .config arch/arm/configs/${DEFCONFIG_FILE}
done
make mrproper -j8
