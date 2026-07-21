# Raspberry Pi 3 Wi-Fi and SSH Setup

## Overview

This document explains how to connect a Raspberry Pi 3 to a Wi-Fi network using `nmcli` and then access it remotely via SSH.

---

# 1. Verify the Wi-Fi Interface

Check that the wireless interface is detected:

```bash
iw dev
```

Example:

```text
phy#0
    Interface wlan0
        type managed
```

If `wlan0` appears, the Wi-Fi hardware is correctly detected.

---

# 2. Check the Current IP Address

```bash
hostname -I
```

If the command returns nothing:

```text
```

the Raspberry Pi is **not connected** to a Wi-Fi network.

---

# 3. Find the Wi-Fi Network Name (SSID)

On your Ubuntu laptop, display the connected Wi-Fi network:

```bash
iwgetid -r
```

Example:

```text
SFR_3E60_EXT
```

The output is the Wi-Fi **SSID**.

---

# 4. Connect the Raspberry Pi to Wi-Fi

Use NetworkManager (`nmcli`) to connect.

Syntax:

```bash
sudo nmcli device wifi connect "<SSID>" password "<PASSWORD>"
```

Example:

```bash
sudo nmcli device wifi connect "SFR_3E60_EXT" password "pggi9mjm96ntvz8ehs34"
```

If the connection succeeds, NetworkManager will report that the device has been successfully activated.

---

# 5. Verify the Connection

Display the assigned IP address:

```bash
hostname -I
```

Example:

```text
192.168.1.45
```

If an IP address is displayed, the Raspberry Pi is connected to the network.

---

# 6. Display Available Wi-Fi Networks

If the connection fails, list all visible Wi-Fi networks:

```bash
nmcli device wifi list
```

Example:

```text
IN-USE  SSID
*       SFR_3E60_EXT
        Guest_WiFi
        Mobile_Hotspot
```

The network marked with `*` is the currently connected network.

---

# 7. Verify Active Network Connections

```bash
nmcli connection show --active
```

Example:

```text
NAME          TYPE      DEVICE
SFR_3E60_EXT  wifi      wlan0
lo            loopback  lo
```

This confirms that `wlan0` is connected to the Wi-Fi network.

---

# 8. Enable SSH

If the OpenSSH server is not installed:

```bash
sudo apt update
sudo apt install openssh-server
```

Enable and start the SSH service:

```bash
sudo systemctl enable ssh
sudo systemctl start ssh
```

Verify the service status:

```bash
systemctl status ssh
```

Expected output:

```text
Active: active (running)
```

---

# 9. Connect to the Raspberry Pi via SSH

From the Ubuntu laptop:

```bash
ssh roukaya@192.168.1.45
```

Replace:

* `roukaya` with your Raspberry Pi username.
* `192.168.1.45` with the IP address returned by `hostname -I`.

During the first connection, accept the SSH host key:

```text
Are you sure you want to continue connecting (yes/no)?
```

Answer:

```text
yes
```

Then enter your password.

---

# Troubleshooting

## No IP Address

```bash
hostname -I
```

returns nothing.

Possible causes:

* Wi-Fi is not connected.
* Incorrect SSID.
* Incorrect password.
* Wi-Fi network is out of range.

---

## Verify the Wireless Interface

```bash
iw dev
```

The interface `wlan0` should be present.

---

## Display Network Status

```bash
nmcli device status
```

This command shows whether `wlan0` is connected or disconnected.

---

# Useful Commands

```bash
# Show Wi-Fi interface
iw dev

# Show current IP address
hostname -I

# List available Wi-Fi networks
nmcli device wifi list

# Connect to Wi-Fi
sudo nmcli device wifi connect "SFR_3E60_EXT" password "pggi9mjm96ntvz8ehs34"

# Show active connections
nmcli connection show --active

# Enable SSH
sudo systemctl enable ssh
sudo systemctl start ssh

# Check SSH status
systemctl status ssh

# Connect from the laptop
ssh roukaya@<RASPBERRY_PI_IP>
```

---

# Summary

The workflow is:

```text
Ubuntu Laptop
      │
      ├── Find SSID
      │      │
      │      └── iwgetid -r
      │
      ▼
Raspberry Pi
      │
      ├── Connect to Wi-Fi using nmcli
      │
      ├── Verify IP address
      │
      ├── Enable SSH
      │
      ▼
Ubuntu Laptop
      │
      └── ssh roukaya@<RASPBERRY_PI_IP>
```
