FROM ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive

# Configure Build Arguments
ARG BOARD

# Configure Build Environment
ENV ZEPHYR_BASE=/opt/zephyr
ENV ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk
ENV ZEPHYR_GCC_VARIANT=zephyr

# Update apt-get
RUN apt-get -y update && \
  # Install apt-utils
  apt-get -y --no-install-recommends install apt-utils ca-certificates && \
  # Main Build Dependencies
  apt-get -y --no-install-recommends install git build-essential g++-4.8 wget git python python3 findutils

# Zephyr SDK
RUN wget -q https://github.com/zephyrproject-rtos/meta-zephyr-sdk/releases/download/0.9.1/zephyr-sdk-0.9.1-setup.run && bash zephyr-sdk-0.9.1-setup.run -- -d $ZEPHYR_SDK_INSTALL_DIR -y

# Zephyr kernel
RUN git clone http://github.com/zephyrproject-rtos/zephyr $ZEPHYR_BASE && cd $ZEPHYR_BASE && git checkout tags/v1.7.0

# ZMRAA
RUN git clone https://github.com/intel-iot-devkit/zmraa.git $ZEPHYR_BASE/ext/lib/mraa

# Set Workdir
WORKDIR /work/zupm

# Copy sources
COPY . .

# Link ZUPM under Zephyr kernel
RUN ln -s /work/zupm $ZEPHYR_BASE/ext/lib/upm

# Patch Zephyr ext Kbuild to include ZMRAA
RUN patch -d $ZEPHYR_BASE -p1 < $ZEPHYR_BASE/ext/lib/mraa/install.patch

# Patch Zephyr ext Kbuild to include ZUPM
RUN patch -d $ZEPHYR_BASE -p1 < install.patch
