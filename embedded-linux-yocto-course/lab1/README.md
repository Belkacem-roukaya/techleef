# Lab 1 — Raspberry Pi Setup, Shell and POSIX Programming

This directory contains my practical work for the first week of the **TechLeef Embedded Linux & Yocto course**.

The workshop covers Raspberry Pi setup, Linux shell programming, Debian package management, and POSIX network programming with TCP sockets.

## Hardware Adaptation

> **Important:** the official workshop material uses a **Raspberry Pi Zero 2 W**, but I am performing and validating these exercises on my personal **Raspberry Pi 3**.

The two boards share many Linux configuration concepts, but some details—such as boot files, UART configuration, hardware interfaces, and device names—may differ. My notes document the commands and adjustments used on the Raspberry Pi 3.

## Workshop Objectives

- flash and configure a Raspberry Pi OS image;
- enable UART and monitor the boot process through a serial console;
- configure Wi-Fi and remote access with SSH;
- understand shell scripting and Debian package-management commands;
- develop a small Bash helper for Debian packages;
- understand POSIX inter-process communication;
- create a TCP server and client in C;
- extend the TCP server to support multiple clients with POSIX threads.

## Workshop Parts

### 1. Raspberry Pi Setup

This part covers:

- downloading and using Raspberry Pi Imager;
- flashing the operating-system image to an SD card;
- configuring the hostname, user account, Wi-Fi, and SSH;
- enabling UART in the boot configuration;
- connecting an FTDI USB-to-UART adapter;
- monitoring Linux boot messages with Minicom.

Related notes:

- [Boot with UART](setup-guides/boot-with-uart.md)
- [Enable UART on Raspberry Pi 3](setup-guides/enable-uart-rpi3.md)
- [Wi-Fi and SSH setup](setup-guides/wifi-and-ssh-setup.md)

### 2. Shell Package Manager

The objective is to develop a Bash script capable of:

- displaying information about a Debian package;
- installing a local or remote package;
- removing an installed package;
- creating a `.deb` package from a prepared directory;
- displaying a command-line help menu.

The package-manager [script](shell-package-manager/debmgr.sh) implements information, installation, removal, package creation, argument validation, and help actions. Operations that modify the system still require practical validation on a Debian-based test machine.

### 3. POSIX and TCP Sockets

This part introduces TCP communication in C through the following system calls:

```text
socket() → bind() → listen() → accept() → recv()/send() → close()
```

Current files:

- [server.c](ipc-tcp/server/server.c): implementation of a single-client TCP server;
- [server documentation](ipc-tcp/server/README.md): explanation of the server workflow;
- [client.c](ipc-tcp/client/client.c): implementation of the TCP client;
- [client documentation](ipc-tcp/client/client.md): explanation of the client workflow.

Future exercises from the workshop include:

- supporting multiple clients with POSIX threads;
- broadcasting messages to connected clients;
- sending and receiving data simultaneously;
- experimenting with processes, signals, shared memory, UNIX sockets, and UDP.

## Current Progress

| Task | Status |
|---|---|
| Raspberry Pi image preparation | Completed |
| UART configuration on Raspberry Pi 3 | Completed |
| Serial boot monitoring | Completed |
| Wi-Fi and SSH setup | Completed |
| Shell package-manager script | Implemented; system tests pending |
| Single-client TCP server | Completed |
| TCP client | Completed |
| Multithreaded TCP server | Planned |

## Current Structure

```text
lab1/
├── README.md
├── course-material/
│   └── EmbeddedLinuxYocto_RPi02W_W1.pdf
├── setup-guides/
│   ├── boot-with-uart.md
│   ├── enable-uart-rpi3.md
│   ├── sd-card-detection.md
│   └── wifi-and-ssh-setup.md
├── shell-package-manager/
│   └── debmgr.sh
├── ipc-tcp/
│   ├── README.md
│   ├── client/
│   │   ├── README.md
│   │   ├── client.c
│   │   └── client.md
│   └── server/
│       ├── README.md
│       └── server.c
└── tools/
    └── raspberry-pi-imager/
        └── imager_2.0.10_amd64.AppImage
```

## Course Material

The original workshop presentation is available in [EmbeddedLinuxYocto_RPi02W_W1.pdf](course-material/EmbeddedLinuxYocto_RPi02W_W1.pdf).

## Disclaimer

This repository contains my personal implementation, notes, and explanations based on the TechLeef workshop. It is not the official TechLeef course repository.
