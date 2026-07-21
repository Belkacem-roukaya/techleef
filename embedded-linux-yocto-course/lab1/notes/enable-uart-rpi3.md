# Enable UART on Raspberry Pi 3 from Ubuntu

This guide explains how to enable UART on a Raspberry Pi 3 by modifying the boot configuration file directly from Ubuntu.

UART is not enabled by default. Before removing the SD card from the computer, we need to add the following configuration:

```text
enable_uart=1
```

> **Note:** In this example, the SD card is detected as `/dev/sde`.
> The device name may be different on another computer. Always verify the device name before running the commands.

## 1. Insert the SD Card

Insert the flashed Raspberry Pi SD card into the Ubuntu computer.

Check the latest kernel messages:

```bash
sudo dmesg | tail -30
```

The output should show that the SD card was detected.

Example:

```text
sd 0:0:0:0: [sde] Attached SCSI removable disk
sde: sde1 sde2
```

In this example:

* `/dev/sde` is the complete SD card.
* `/dev/sde1` is the Raspberry Pi boot partition.
* `/dev/sde2` is the Linux root filesystem partition.

## 2. Check the Block Devices

Display the available disks and partitions:

```bash
lsblk
```

Example output:

```text
NAME        SIZE TYPE MOUNTPOINTS
nvme0n1     512G disk
├─nvme0n1p1 512M part /boot/efi
└─nvme0n1p2 511G part /

sde          32G disk
├─sde1      512M part
└─sde2        5G part
```

Make sure that `/dev/sde` is the SD card by checking its storage capacity.

> ⚠️ **Warning:** Do not use the computer system disk, such as `/dev/sda` or `/dev/nvme0n1`.

## 3. Create a Mount Point

Create a directory that will be used to access the Raspberry Pi boot partition:

```bash
sudo mkdir -p /mnt/bootfs
```

The `-p` option prevents an error if the directory already exists.

## 4. Mount the Boot Partition

Mount the first SD card partition:

```bash
sudo mount /dev/sde1 /mnt/bootfs
```

The content of `/dev/sde1` is now accessible from:

```text
/mnt/bootfs
```

Verify that the partition is correctly mounted:

```bash
mountpoint /mnt/bootfs
```

Expected output:

```text
/mnt/bootfs is a mountpoint
```

## 5. Check the Boot Configuration Files

Go to the mounted boot partition:

```bash
cd /mnt/bootfs
```

Display the available text configuration files:

```bash
ls *.txt
```

Expected output:

```text
cmdline.txt
config.txt
issue.txt
```

The UART configuration will be added to `config.txt`.

## 6. Enable UART

Append the UART configuration to `config.txt`:

```bash
echo "enable_uart=1" | sudo tee -a config.txt > /dev/null
```

Command explanation:

* `echo "enable_uart=1"` generates the configuration line.
* `|` sends the output of `echo` to the next command.
* `sudo tee` writes the configuration into `config.txt`.
* `-a` means **append**, so the existing content is preserved.
* `> /dev/null` hides the output printed by `tee`.

The following line is added at the end of `config.txt`:

```text
enable_uart=1
```

> Use standard quotation marks `" "` in the terminal. Do not copy typographic quotation marks such as `“ ”`.

## 7. Verify the UART Configuration

Search for the UART configuration:

```bash
grep "enable_uart" config.txt
```

Expected output:

```text
enable_uart=1
```

The UART interface is now enabled.

## 8. Safely Unmount the SD Card

Leave the mounted directory:

```bash
cd ~
```

Synchronize pending write operations:

```bash
sync
```

Unmount the boot partition:

```bash
sudo umount /mnt/bootfs
```

Verify that the partition is no longer mounted:

```bash
mountpoint /mnt/bootfs
```

Expected output:

```text
/mnt/bootfs is not a mountpoint
```

The SD card can now be safely removed from the Ubuntu computer and inserted into the Raspberry Pi 3.

## UART Connection

To view the Raspberry Pi boot logs, connect a 3.3 V USB-to-UART adapter.

| USB-to-UART Adapter | Raspberry Pi 3 | Physical Pin |
| ------------------- | -------------- | -----------: |
| GND                 | GND            |        Pin 6 |
| RX                  | GPIO14 / TXD   |        Pin 8 |
| TX                  | GPIO15 / RXD   |       Pin 10 |

> ⚠️ Use 3.3 V UART logic. Do not connect a 5 V UART signal directly to the Raspberry Pi GPIO pins.

The TX and RX connections must be crossed:

```text
USB-UART RX  ───── Raspberry Pi TX
USB-UART TX  ───── Raspberry Pi RX
USB-UART GND ───── Raspberry Pi GND
```

## Read the UART Logs from Ubuntu

Connect the USB-to-UART adapter to the Ubuntu computer.

Find the serial device:

```bash
ls /dev/ttyUSB*
```

Example:

```text
/dev/ttyUSB0
```

Open the serial console using Minicom:

```bash
sudo minicom -D /dev/ttyUSB0 -b 115200
```

The Raspberry Pi boot logs should appear after powering on or rebooting the board.

## Summary

The complete command sequence is:

```bash
lsblk

sudo mkdir -p /mnt/bootfs
sudo mount /dev/sde1 /mnt/bootfs

cd /mnt/bootfs

echo "enable_uart=1" | sudo tee -a config.txt > /dev/null

grep "enable_uart" config.txt

cd ~

sync
sudo umount /mnt/bootfs
```
