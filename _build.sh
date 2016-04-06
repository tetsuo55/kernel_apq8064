#!/bin/sh

### checkout correct prima sources if needed
PRIMA_PATH="drivers/staging/prima"
PRIMA_GIT="https://github.com/adrian-bl-yuga/caf_prima.git"

if [ ! -d ${PRIMA_PATH}/.git ] ; then
	echo "found in-kernel prima sources, replacing them"
	echo "with a copy of $PRIMA_GIT in 2 sec..."
	sleep 2

	echo "-> removing old source tree..."
	rm -rf $PRIMA_PATH

	echo "-> cloning $PRIMA_GIT"
	git clone $PRIMA_GIT $PRIMA_PATH
fi
### end prima

export ARCH=arm
export PATH=../../../prebuilts/gcc/linux-x86/arm/arm-eabi-5.3/bin/:$PATH
export CROSS_COMPILE=arm-eabi-

# build kernel
make fusion3_yuga_defconfig
make -j8

export TCHAIN=arm-eabi-

# add new kernel
cp arch/arm/boot/zImage ../../../device/sony/c6603/kernel 

# copy modules
cp ./drivers/staging/prima/wlan.ko  ../../../vendor/sony/yuga_blobs/system/lib/modules/wlan.ko
cp ./arch/arm/mach-msm/reset_modem.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/reset_modem.ko
cp ./arch/arm/mach-msm/msm-buspm-dev.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/msm-buspm-dev.ko
cp ./crypto/ansi_cprng.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/ansi_cprng.ko
cp ./drivers/video/backlight/lcd.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/lcd.ko
cp ./drivers/scsi/scsi_wait_scan.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/scsi_wait_scan.ko
cp ./drivers/char/adsprpc.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/adsprpc.ko
cp ./drivers/media/video/gspca/gspca_main.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/gspca_main.ko
cp ./drivers/media/radio/radio-iris-transport.ko ../../../vendor/sony/yuga_blobs/system/lib/modules/radio-iris-transport.ko

# refresh kernel headers
( cd ../../../device/sony/lagan/ && sh update_kernel_headers.sh )
