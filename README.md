## Synopsis

ZUPM is a UPM C source repo for use with the [Zephyr Project](http://zephyrproject.org).

## Requirements

- [Zephyr Project](https://gerrit.zephyrproject.org)
- [ZMRAA](https://github.com/intel-iot-devkit/zmraa)

## Installation

1. Clone Zephyr and Zephyr SDK

2. Add ZMRAA to Zephyr - [Instructions](https://github.com/intel-iot-devkit/zmraa/blob/master/README.md)

3. Setup ZUPM
```sh
source zephyr-env.sh
cd $ZEPHYR_BASE
git clone https://github.com/intel-iot-devkit/zupm.git $ZEPHYR_BASE/ext/lib/upm
patch -p1 < $ZEPHYR_BASE/ext/lib/upm/zupm_2_zephyr.patch
ln -s $ZEPHYR_BASE/ext/lib/upm/samples/upm $ZEPHYR_BASE/samples/
```

## Code Example

```sh
cd $ZEPHYR_BASE/samples/upm/dfrph
make BOARD=arduino_101_sss_factory
```
