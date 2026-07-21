# Embedded Linux & Yocto — Course Workspace

This directory contains my notes, labs, source code, and practical work completed during the **TechLeef Embedded Linux & Yocto course**.

TechLeef is a free Embedded Systems Academy that makes high-quality technical education accessible to everyone. I am an active member of its community, and this workspace documents the course concepts in my own words and through my own experiments.

> This is a personal learning repository. It is not the official TechLeef course repository.

## Course Information

| Item | Details |
|---|---|
| **Topic** | Embedded Linux and Yocto Project |
| **Instructor** | Talel Belhaj Salem — Yocto Ambassador |
| **Period** | 1 July 2026 to 28 August 2026 |
| **Schedule** | Monday, Wednesday, and Friday |
| **Time** | 9:00 PM–11:00 PM |
| **Duration** | 26 sessions — approximately 52 hours |
| **Target board** | Raspberry Pi Zero 2 W |

The original course presentation is available in [EmbeddedLinux_&Yocto_GoldenCourse.pdf](<./EmbeddedLinux_&Yocto_GoldenCourse.pdf>).

## Learning Objectives

By the end of this course, the learner should be able to:

- understand Linux userspace, kernel space, system calls, and the root filesystem;
- write POSIX shell scripts and Linux system programs;
- build and use native and cross-compilation toolchains;
- create static and shared libraries with Make, CMake, and pkg-config support;
- configure and build the Linux kernel and U-Boot;
- develop kernel modules, character drivers, and Device Tree overlays;
- control GPIO, UART, SPI, and I²C peripherals;
- understand BitBake metadata and write Yocto recipes;
- create custom Yocto images, distributions, machines, and BSP layers;
- integrate initramfs images and kernel configuration fragments;
- secure an embedded Linux system with dm-crypt and dm-verity;
- implement reliable A/B OTA updates with RAUC.

## Course Roadmap

### Part 1 — Linux Architecture, Shell, and POSIX

- Linux architecture and CPU privilege levels
- Userspace and kernel-space separation
- Root filesystem structure
- Shell commands, permissions, redirections, and pipes
- POSIX shell scripting
- Processes, threads, signals, files, and IPC

**Lab 1:** prepare and boot the target image, create an SDK, write system-management scripts, and cross-compile POSIX programs.

### Part 2 — Toolchains, Libraries, and Build Systems

- Compilation stages, compiler flags, and linker flags
- Native, cross, and Canadian cross-compilation
- Toolchain creation with crosstool-ng
- Static and shared libraries
- Runtime linker and loader cache
- QEMU-based execution
- Make, CMake, and pkg-config

**Lab 2:** develop a library in static and shared forms, use it from an application, cross-compile the project, and deploy it to the target.

### Part 3 — Kernel, U-Boot, and Hardware Interfaces

- Linux kernel and U-Boot source trees
- Kbuild and kernel configuration
- Device Trees and Device Tree Overlays
- Kernel modules, sysfs, and character devices
- GPIO with sysfs and libgpiod
- SPI and I²C from userspace and kernel space

**Lab 3:** develop a character driver, enable sensor drivers, create Device Tree overlays, and load them through U-Boot.

### Part 4 — Yocto Fundamentals

- BitBake execution model
- Recipes, classes, configuration files, and patches
- Layer organization and metadata
- Application recipes and package generation
- Custom images and distributions
- Image package selection and optimization

**Lab 4:** produce the first Yocto build, create custom recipes and layers, build a custom distribution, and optimize it for production.

### Part 5 — BSP and Machine Development

- Yocto Board Support Packages
- Kernel and bootloader providers
- Custom machine configurations
- Overrides, recipe variants, and upstream sources
- Kernel fragments and Device Trees
- Initramfs development and integration

**Lab 5:** create a board-specific machine, integrate an initramfs, and add sensor-related kernel fragments and machine features.

### Part 6 — Security and OTA Updates

- Storage encryption with dm-crypt
- Data integrity with dm-verity
- Initramfs security scripts
- SquashFS and production image design
- A/B update architecture
- RAUC bundles and adaptive OTA updates

**Lab 6:** create data partitions, integrate encryption and integrity checks, then deploy and test RAUC updates using an HTTP server.

## Proposed 26-Session Plan

This detailed session breakdown is a practical proposal derived from the official course roadmap.

| Session | Date | Main topic |
|---:|---|---|
| 1 | Wed, 1 Jul | Kickoff, course environment, hardware, and workflow |
| 2 | Fri, 3 Jul | Linux architecture: userspace and kernel space |
| 3 | Mon, 6 Jul | Root filesystem, shell, commands, and permissions |
| 4 | Wed, 8 Jul | POSIX shell scripting and system programming |
| 5 | Fri, 10 Jul | Lab 1: image, SDK, scripts, and cross-compilation |
| 6 | Mon, 13 Jul | Compilation process, GCC, binutils, and linking |
| 7 | Wed, 15 Jul | Cross-toolchains and crosstool-ng |
| 8 | Fri, 17 Jul | Static/shared libraries, Make, CMake, and pkg-config |
| 9 | Mon, 20 Jul | Lab 2: library, application, build, and deployment |
| 10 | Wed, 22 Jul | Linux kernel and U-Boot architecture |
| 11 | Fri, 24 Jul | Kbuild, kernel configuration, and compilation |
| 12 | Mon, 27 Jul | Kernel modules, sysfs, and character drivers |
| 13 | Wed, 29 Jul | Device Trees, GPIO, SPI, and I²C |
| 14 | Fri, 31 Jul | Lab 3: drivers, overlays, sensors, and U-Boot |
| 15 | Mon, 3 Aug | Yocto architecture, Poky, BitBake, and metadata |
| 16 | Wed, 5 Aug | Recipe development, sources, patches, and packages |
| 17 | Fri, 7 Aug | Images, distributions, features, and optimization |
| 18 | Mon, 10 Aug | Lab 4: first build, custom layer, recipe, and distro |
| 19 | Wed, 12 Aug | BSP layers, kernel recipes, and bootloader recipes |
| 20 | Fri, 14 Aug | Machine configuration, providers, and overrides |
| 21 | Mon, 17 Aug | Initramfs, kernel fragments, and optimization |
| 22 | Wed, 19 Aug | Lab 5: custom machine, initramfs, and features |
| 23 | Fri, 21 Aug | Embedded Linux security, dm-crypt, and dm-verity |
| 24 | Mon, 24 Aug | A/B update design, RAUC, and SquashFS |
| 25 | Wed, 26 Aug | Lab 6: secure partitions and OTA integration |
| 26 | Fri, 28 Aug | Final integration, validation, review, and next steps |

## Hardware

The course uses the following equipment:

- Raspberry Pi Zero 2 W;
- microSD card of at least 16 GB;
- breadboard and jumper wires;
- USB-to-TTL FTDI adapter;
- ADXL345 I²C/SPI accelerometer;
- MCP23017 I²C GPIO expander;
- micro-USB cable.

## Repository Organization

```text
embedded-linux-yocto-course/
├── README.md                    # Course overview and learning plan
├── EmbeddedLinux_&Yocto_GoldenCourse.pdf
├── lab1/                        # First practical lab
│   ├── docs/                    # Lab documentation
│   ├── notes/                   # Setup and troubleshooting notes
│   ├── socket_server.c          # TCP server exercise
│   └── server.md                # TCP server explanation
└── future-labs/                 # Additional labs as the course progresses
```

The structure will evolve as new sessions and labs are completed.

## Skills Developed

`Embedded Linux` · `Yocto Project` · `BitBake` · `C/C++` · `POSIX` · `Bash` · `Linux Kernel` · `U-Boot` · `Device Tree` · `GPIO` · `UART` · `SPI` · `I²C` · `CMake` · `Make` · `QEMU` · `RAUC` · `Git`

## Acknowledgements

Thanks to **TechLeef Academy**, the instructor **Talel Belhaj Salem**, and everyone who helped make this course freely accessible.

> Learn freely. Build practically. Share knowledge.
