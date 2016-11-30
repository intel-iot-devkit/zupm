## Synopsis

ZUPM is a UPM C source repo for use with the [Zephyr Project](http://zephyrproject.org).

## Requirements

- [Zephyr Project](https://gerrit.zephyrproject.org)
- [ZMRAA](https://github.com/intel-iot-devkit/zmraa)

## Zephyr Target Versions

- Zephyr v1.6.0 - [ZUPM version v1.6.0 - Complete](https://github.com/intel-iot-devkit/zupm/tree/master)
- Zephyr v1.5.0 - [ZUPM version v1.5.0 - Complete](https://github.com/intel-iot-devkit/upm/tree/v1.5.0)

## Installation

#### Clone Zephyr (requires Zephyr SDK)
```sh
git clone https://gerrit.zephyrproject.org/r/zephyr && cd zephyr && git checkout tags/v1.5.0

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
patch -p1 < $ZEPHYR_BASE/ext/lib/upm/zupm_2_zephyr.patch
ln -s $ZEPHYR_BASE/ext/lib/upm/samples/upm $ZEPHYR_BASE/samples/
```

## Code Example

#### AIO:
```sh
# DFRobot pH Sensor
cd $ZEPHYR_BASE/samples/upm/dfrph
make BOARD=arduino_101_sss
```

#### GPIO:
```sh
# Onboard LED (pin 13)
cd $ZEPHYR_BASE/samples/upm/led
make BOARD=arduino_101
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
