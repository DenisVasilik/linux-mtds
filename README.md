# How to use Digilent's MTDS from Linux User Space

This sample code shows how to use Digilent's MTDS from Linux applications.

## General Architecture

The following diagram shows a logical view of how a Linux application talks to the MTDS.

```
+-----------------------------------+
| Linux User Space Application      |
+-----------------------------------+
| MTDS Library (User Space)         |
+-----------------------------------+         +-----------------------------------+   SPI   +-----------------------------------+
| MTDS Device Driver (Kernel Space) | <-----> | FPGA Digilent PMOD IP for MTDS    | <-----> | Multitouch Display (MTDS)         |
+-----------------------------------+         +-----------------------------------+         +-----------------------------------+
```

# Source Code

The source code is separated in to the following directories.

* example - Linux user application
* libmtds - MTDS Library adjusted to use the MTDS driver
* mtds - MTDS device driver

# Build

## Prerequisites

* Xilinx Yocto SDK for ZYNQ
* Xilinx PetaLinux
* Digilent PMOD IP for MTDS
* CMake

## Linux User Application

The Linux user application can be built using the following commands.

```
source /opt/poky/2.1.1/environment-setup-cortexa9hf-neon-poky-linux-gnueabi
mkdir build && cd build
cmake ..
make
```

## MTDS Library

The MTDS library can be built using the following commands.

```
source /opt/poky/2.1.1/environment-setup-cortexa9hf-neon-poky-linux-gnueabi
mkdir build && cd build
cmake ..
make
```

## MTDS Device Driver

The ´KERNELDIR´ variable in the Makefile must be adjusted to the kernel path the module should be built for. I use the PetaLinux that Xilinx provides, so 
my path points to the PetaLinux source provided by PetaLinux.

```
mtds$ make all
```
