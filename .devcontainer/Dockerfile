FROM ubuntu:22.04

RUN apt-get update && apt-get -y install --no-install-recommends make cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib build-essential gdb-multiarch

RUN ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb
