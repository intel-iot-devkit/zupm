## Synopsis

ZUPM is a UPM C source repo for use with the [Zephyr Project](http://zephyrproject.org).

## Requirements

- [Zephyr Project](https://gerrit.zephyrproject.org)
- [ZMRAA](https://github.com/intel-iot-devkit/zmraa)

## Installation

1. Clone Zephyr and Zephyr SDK
```sh
git clone https://gerrit.zephyrproject.org/r/zephyr && cd zephyr && git checkout tags/v1.5.0
source zephyr-env.sh
```

2. Clone ZMRAA
```sh
git clone https://github.com/intel-iot-devkit/zmraa.git $ZEPHYR_BASE/ext/lib/mraa
```

3. Clone ZUPM
```sh
git clone https://github.com/intel-iot-devkit/zupm.git $ZEPHYR_BASE/ext/lib/upm
```

3. Setup Kbuild to include MRAA and UPM, link UPM examples to Zephry samples
```sh
cd $ZEPHYR_BASE
patch -p1 < $ZEPHYR_BASE/ext/lib/upm/zupm_2_zephyr.patch
ln -s $ZEPHYR_BASE/ext/lib/upm/samples/upm $ZEPHYR_BASE/samples/
```

## Code Example

```sh
cd $ZEPHYR_BASE/samples/upm/dfrph
make BOARD=arduino_101_sss_factory
```
