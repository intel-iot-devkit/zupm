## Synopsis

ZUPM is a UPM C source repo for use with the [Zephyr Project](http://zephyrproject.org).

## Requirements

- [Zephyr Project](https://gerrit.zephyrproject.org)
- [ZMRAA](https://github.com/intel-iot-devkit/zmraa)

## Releases

- [ZUPM v1.2.0](https://github.com/intel-iot-devkit/zupm/tree/v1.2.0) - supports [Zephyr Kernel 1.8](https://github.com/zephyrproject-rtos/zephyr/tree/zephyr-v1.8.0)
- [ZUPM v1.1.0](https://github.com/intel-iot-devkit/zupm/tree/v1.1.0) - supports [Zephyr Kernel 1.7](https://github.com/zephyrproject-rtos/zephyr/tree/zephyr-v1.7.0)
- [ZUPM v1.0.0](https://github.com/intel-iot-devkit/zupm/tree/v1.0.0) - supports [Zephyr Kernel 1.6](https://github.com/zephyrproject-rtos/zephyr/tree/zephyr-v1.6.0)
- [ZUPM v0.9.0](https://github.com/intel-iot-devkit/zupm/tree/v0.9.0) - supports [Zephyr Kernel 1.5](https://github.com/zephyrproject-rtos/zephyr/tree/zephyr-v1.5.0)

## Installation (clone from zephyr/zmraa/zupm and patch)

#### Clone Zephyr Kernel (requires corresponding Zephyr SDK)
```sh
git clone https://github.com/zephyrproject-rtos/zephyr.git && cd zephyr && git checkout tags/v1.7.0

export ZEPHYR_GCC_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=<path to your sdk installation directory>
source zephyr-env.sh
```

#### Clone ZMRAA
```sh
git clone https://github.com/intel-iot-devkit/zmraa.git $ZEPHYR_BASE/ext/lib/mraa
```

#### Clone ZUPM
```sh
git clone https://github.com/intel-iot-devkit/zupm.git $ZEPHYR_BASE/ext/lib/upm
```

#### Setup Kbuild to include MRAA and UPM, link UPM examples to Zephry samples
```sh
cd $ZEPHYR_BASE
patch -p1 < $ZEPHYR_BASE/ext/lib/mraa/install.patch
patch -p1 < $ZEPHYR_BASE/ext/lib/upm/install.patch
ln -s $ZEPHYR_BASE/ext/lib/upm/samples/upm $ZEPHYR_BASE/samples/
```

```
NOTE: ZUPM HAS A HARD DEPENDENCY ON ZMRAA. PLEASE CLONE AND PATCH ZMRAA FIRST AND THEN PROCEED TO ZUPM.
```

## Code Examples

#### AIO:
```sh
# DFRobot pH Sensor
cd $ZEPHYR_BASE/samples/upm/dfrph
make BOARD=arduino_101_sss
```

#### SPI:
```sh
# Onboard Accel, Gyro, Magno
cd $ZEPHYR_BASE/samples/upm/bmi160
make BOARD=arduino_101_sss
```

#### I2C:
```sh
# LCD display
cd $ZEPHYR_BASE/samples/upm/jhd1313m1
make BOARD=arduino_101_sss
```

#### GPIO - Onboard LED (BOARD=arduino_101):
```sh
# Onboard LED (pin 13)
cd $ZEPHYR_BASE/samples/upm/led
make BOARD=arduino_101
```
[Flash the target](https://www.zephyrproject.org/doc/boards/x86/arduino_101/doc/board.html#flashing).  After resetting the target, the onboard LED should flash.

#### GPIO - Onboard LED (BOARD=quark_d2000_crb):
```sh
# Onboard LED (pin 9)
cd $ZEPHYR_BASE/samples/upm/led
make BOARD=quark_d2000_crb
```
[Flash the target](https://www.zephyrproject.org/doc/boards/x86/quark_d2000_crb/doc/quark_d2000_crb.html#flashing).  After resetting the target, the onboard LED should flash.

#### GPIO - Onboard LED (BOARD=quark_se_c1000_devboard):
```sh
# Onboard LED (pin 64)
cd $ZEPHYR_BASE/samples/upm/led
make BOARD=quark_se_c1000_devboard
```
Flash the target
```sh
make BOARD=quark_se_c1000_devboard flash
```
After resetting the target, the onboard LED should flash.
