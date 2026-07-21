# TechLeef — My Embedded Systems Learning Journey

> **Learn freely. Build practically. Share knowledge.**

This repository documents my learning journey in **Embedded Systems**, **Embedded Linux**, and the **Yocto Project** through courses, workshops, personal notes, and practical projects.

## About TechLeef

**TechLeef** is a 100% free Embedded Systems Academy dedicated to making technical education accessible to everyone.

I am an active member of the TechLeef community. In this repository, I reformulate the concepts taught during the academy's courses in my own way and apply them through practical exercises.

## About This Repository

This is my personal learning workspace. It contains:

- course notes and technical explanations;
- practical laboratories and workshop implementations;
- Embedded Linux configuration guides;
- C and POSIX programming exercises;
- Yocto layers, images, distributions, and BSP experiments;
- security, system integration, and OTA update projects.

## Repository Overview

| Directory | Description | Status |
|---|---|---|
| [`embedded-linux-yocto-course`](embedded-linux-yocto-course/) | TechLeef Embedded Linux and Yocto course, including the course roadmap, workshops, and personal notes | In progress |
| [`project`](project/) | Custom Yocto-based Linux distribution for the Raspberry Pi Zero 2 W | In progress |
| [`projet_indus`](projet_indus/) | EdgeForge industrial gateway study and practical project | In progress |

## Embedded Linux & Yocto Course

The [`embedded-linux-yocto-course`](embedded-linux-yocto-course/) directory follows the TechLeef course held from **1 July to 28 August 2026**.

The course covers:

1. Linux architecture, shell, and POSIX programming;
2. toolchains, libraries, Make, CMake, and pkg-config;
3. Linux kernel, U-Boot, modules, and hardware interfaces;
4. BitBake, Yocto recipes, images, and distributions;
5. BSP layers, machine development, kernel, and initramfs;
6. embedded security, dm-crypt, dm-verity, and RAUC OTA updates.

The complete roadmap and proposed 26-session plan are available in the [course README](embedded-linux-yocto-course/README.md).

### Lab 1 — Raspberry Pi Setup, Shell, and POSIX

The first workshop includes:

- Raspberry Pi image preparation;
- UART configuration and serial boot monitoring;
- Wi-Fi and SSH configuration;
- development of a Debian package-manager helper in Bash;
- POSIX IPC and TCP socket programming in C;
- TCP server, client, and multithreading exercises.

> The official workshop uses a **Raspberry Pi Zero 2 W**, but I perform and validate the Lab 1 exercises on my personal **Raspberry Pi 3**. My setup guides document the required adaptations.

See the [Lab 1 README](embedded-linux-yocto-course/lab1/README.md) for its documentation, current progress, and source code.

## Main Skills and Technologies

### Embedded Linux

- Linux architecture, userspace, and kernel space
- root filesystem and boot process
- Bash and POSIX shell scripting
- processes, threads, signals, and IPC
- TCP/IP and socket programming

### Build Systems and Toolchains

- GCC and binutils
- native and cross-compilation
- crosstool-ng
- Make and CMake
- static and shared libraries
- pkg-config

### Kernel and Hardware

- Linux kernel and Kbuild
- U-Boot
- kernel modules and character drivers
- Device Trees and overlays
- GPIO, UART, SPI, and I²C
- Raspberry Pi 3 and Raspberry Pi Zero 2 W

### Yocto Project

- BitBake and metadata
- recipes, classes, and layers
- custom images and distributions
- BSP and machine configurations
- kernel fragments and initramfs
- WIC partitioning
- RAUC and A/B OTA updates
- dm-crypt and dm-verity

## Learning Approach

For each subject, I aim to:

1. understand the underlying concepts;
2. reformulate them in clear technical notes;
3. implement a practical example;
4. test it on real hardware when possible;
5. document problems, solutions, and hardware-specific adaptations;
6. track improvements using Git and GitHub.

## Progress

This repository evolves throughout the course. Some exercises and scripts are intentionally marked as **in progress** and will be completed as the related sessions advance.

## Acknowledgements

Thanks to **TechLeef Academy**, its instructors, contributors, and community members for making Embedded Systems education freely accessible.

## Disclaimer

This is a personal educational repository created as part of my participation in the TechLeef community. It contains my own notes, interpretations, implementations, and experiments. It is **not the official TechLeef repository**.
