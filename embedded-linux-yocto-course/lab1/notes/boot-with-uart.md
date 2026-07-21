# Raspberry Pi 3 Boot with UART

## Introduction

A Raspberry Pi 3 normally boots directly from the SD card. UART is **not used as a boot source** and does not replace the SD card.

UART is a serial communication interface that can be used to monitor Linux boot logs, debug boot issues, and access the Raspberry Pi console from another computer.

Therefore, the expression **“boot with UART”** means:

> The Raspberry Pi boots from the SD card while the boot logs are monitored through the UART serial interface.

---

## Booting Without UART

When the Raspberry Pi 3 is powered on, it reads the boot files and operating system from the SD card.

The normal boot process is:

```text
Power ON
    │
    ▼
Raspberry Pi Firmware
    │
    ▼
Read Boot Files from the SD Card
    │
    ▼
Load the Linux Kernel
    │
    ▼
Mount the Root Filesystem
    │
    ▼
Start Raspberry Pi OS
```

The SD card contains the files required to boot the Raspberry Pi, including:

* Raspberry Pi firmware
* `config.txt`
* `cmdline.txt`
* Linux kernel
* Device Tree files
* Linux root filesystem (`rootfs`)

Without UART, the Raspberry Pi can be used with:

```text
SD Card ───────► Raspberry Pi 3
Power Supply ──► Raspberry Pi 3
HDMI ──────────► Monitor
USB ───────────► Keyboard and Mouse
```

The boot messages can be displayed through HDMI, followed by the Linux terminal or the Raspberry Pi OS desktop.

---

## Booting with UART

When UART is enabled, the Raspberry Pi still boots from the SD card.

UART does not load the firmware, Linux kernel, or root filesystem. It only provides a serial communication interface that can send boot messages from the Raspberry Pi to another computer.

The boot process remains unchanged:

```text
                         ┌────────► HDMI Monitor
                         │
SD Card ──► Boot ──► Linux
                         │
                         └────────► UART ──► Ubuntu PC ──► Minicom
```

The Raspberry Pi operating system is loaded from the SD card, while the UART interface sends boot logs to the Ubuntu computer.

The logs can be displayed using a serial terminal application such as Minicom.

Example boot messages:

```text
Booting Linux on physical CPU 0x0
Linux version 6.x
Machine model: Raspberry Pi 3 Model B
Kernel command line: ...
Mounting root filesystem...
Starting system services...
Raspberry Pi login:
```

---

## Difference Between Booting With and Without UART

| Without UART                                          | With UART                                           |
| ----------------------------------------------------- | --------------------------------------------------- |
| The Raspberry Pi boots from the SD card               | The Raspberry Pi boots from the SD card             |
| UART is not used                                      | UART is used to transmit serial logs                |
| Boot information can be viewed through HDMI           | Boot information can be viewed through Minicom      |
| A monitor may be required to observe the boot process | The boot process can be monitored without a monitor |
| Boot problems may be more difficult to diagnose       | Boot logs are useful for debugging                  |
| The SD card contains the operating system             | The SD card still contains the operating system     |
| The SD card is required                               | The SD card is still required                       |

---

## Enabling the UART Hardware

To enable the UART hardware interface, add the following line to the Raspberry Pi boot configuration file:

```ini
enable_uart=1
```

This configuration is added to:

```text
config.txt
```

The `enable_uart=1` parameter enables the UART hardware on the Raspberry Pi.

However, enabling the UART hardware does not always mean that the Linux kernel boot logs will automatically be sent through UART.

---

## Enabling the Linux Serial Console

To send Linux kernel messages to the UART interface, add the following kernel parameter to `cmdline.txt`:

```text
console=serial0,115200
```

The `cmdline.txt` file must remain on **one single line**.

Example:

```text
console=serial0,115200 console=tty1 root=PARTUUID=xxxx rootfstype=ext4 fsck.repair=yes rootwait
```

Parameter explanation:

| Parameter                | Description                                                                     |
| ------------------------ | ------------------------------------------------------------------------------- |
| `console=serial0,115200` | Sends the Linux console and kernel messages to UART using a baud rate of 115200 |
| `console=tty1`           | Keeps the local Linux console available through HDMI                            |
| `root=PARTUUID=xxxx`     | Identifies the Linux root filesystem partition                                  |
| `rootfstype=ext4`        | Specifies the root filesystem format                                            |
| `fsck.repair=yes`        | Enables automatic filesystem repair when required                               |
| `rootwait`               | Tells the kernel to wait until the root filesystem is available                 |

> Do not add a new line to `cmdline.txt`. All parameters must remain on the same line and must be separated by spaces.

---

## UART Connection

To monitor the Raspberry Pi boot logs from an Ubuntu computer, use a **3.3 V USB-to-UART adapter**.

| USB-to-UART Adapter | Raspberry Pi 3 | Physical Pin |
| ------------------- | -------------- | -----------: |
| GND                 | GND            |        Pin 6 |
| RX                  | GPIO14 / TXD   |        Pin 8 |
| TX                  | GPIO15 / RXD   |       Pin 10 |

The TX and RX connections must be crossed:

```text
USB-to-UART RX  ◄──────── Raspberry Pi TX
USB-to-UART TX  ────────► Raspberry Pi RX
USB-to-UART GND ───────── Raspberry Pi GND
```

> **Warning:** Raspberry Pi GPIO pins use 3.3 V logic. Do not connect a 5 V UART signal directly to the Raspberry Pi GPIO pins.

The Raspberry Pi should be powered normally through its power connector. Do not use the USB-to-UART adapter to power the Raspberry Pi unless the hardware configuration has been specifically designed for it.

---

## Reading UART Boot Logs on Ubuntu

Connect the USB-to-UART adapter to the Ubuntu computer.

Check the available serial ports:

```bash
ls /dev/ttyUSB*
```

Example output:

```text
/dev/ttyUSB0
```

Open the UART serial console using Minicom:

```bash
sudo minicom -D /dev/ttyUSB0 -b 115200
```

Then power on or reboot the Raspberry Pi.

The boot logs should appear in the Minicom terminal.

---

## Complete Boot Flow

```text
                    ┌──────────────────────────┐
                    │         SD Card          │
                    │                          │
                    │ Firmware                 │
                    │ config.txt               │
                    │ cmdline.txt              │
                    │ Linux Kernel             │
                    │ Device Tree              │
                    │ Root Filesystem          │
                    └────────────┬─────────────┘
                                 │
                                 ▼
                    ┌──────────────────────────┐
                    │      Raspberry Pi 3      │
                    │                          │
                    │ Firmware Initialization  │
                    │ Kernel Loading           │
                    │ Root Filesystem Mount    │
                    │ Linux Services Startup   │
                    └───────┬─────────┬────────┘
                            │         │
                            │         │
                            ▼         ▼
                      HDMI Output   UART Output
                            │         │
                            ▼         ▼
                         Monitor   USB-to-UART
                                      │
                                      ▼
                                  Ubuntu PC
                                      │
                                      ▼
                                   Minicom
```

---

## Conclusion

The Raspberry Pi 3 does not boot from UART in this configuration.

The Raspberry Pi:

1. Reads the firmware from the SD card.
2. Loads the Linux kernel from the SD card.
3. Mounts the Linux root filesystem from the SD card.
4. Starts the operating system.
5. Sends boot logs through UART when the serial console is enabled.

UART is therefore a **debugging and monitoring interface**, not a boot source.

In short:

```text
The Raspberry Pi boots from the SD card
                    +
UART allows the boot process to be monitored from another computer
```
