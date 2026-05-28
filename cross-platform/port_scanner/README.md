# 🔍 Port Scanner — TCP Port Scanner

A Python-based TCP port scanner built with **Scapy** and **sockets**. Supports three scan types and optional service/banner detection. Runs on both **Windows** and **Linux**.

---

## 📄 Files

| File | Description |
|---|---|
| `portscanner.py` | Multi-mode TCP port scanner with service and banner detection |

---

## 🔍 Scan Types

| Scan | Flag | Description |
|---|---|---|
| **SYN Scan** | `--scan syn` | Stealth scan — sends SYN, reads SYN-ACK or RST without completing the handshake |
| **Connect Scan** | `--scan connect` | Full TCP handshake — slower but requires no special privileges |
| **ACK Scan** | `--scan ack` | Firewall analysis — determines if ports are filtered or unfiltered |

---

## 🛠️ Installation

### 1. Install Python 3

- **Linux:** `sudo apt install python3 python3-pip -y`
- **Windows:** Download from [python.org](https://www.python.org/downloads/)

### 2. Install Dependencies

```bash
pip install scapy
```

> **Windows users:** Also install [Npcap](https://npcap.com/#download) — required by Scapy for raw packet capture.

> **Linux users:** SYN and ACK scans require root privileges (`sudo`).

---

## 🚀 Usage

```bash
python3 port_scanner.py --target <IP> --scan <type> [--ports <range>] [--service-detection]
```

### Arguments

| Argument | Required | Description |
|---|---|---|
| `--target` | ✅ | Target IP address |
| `--scan` | ✅ | Scan type: `syn`, `connect`, or `ack` |
| `--ports` | ❌ | Port range (e.g. `1-1024`). Scans all 65535 ports if omitted |
| `--service-detection` | ❌ | Enables banner grabbing on open ports |

### Examples

```bash
# SYN scan on ports 1-1000
sudo python3 portscanner.py --target 192.168.1.1 --scan syn --ports 1-1000

# Full connect scan with service detection
python3 portscanner.py --target 192.168.1.1 --scan connect --ports 20-443 --service-detection

# ACK scan to probe firewall rules
sudo python3 portscanner.py --target 192.168.1.1 --scan ack --ports 80-443
```

> **Note:** Change the default interface in `portscanner.py` if needed:
> ```python
> conf.iface = "eth0"  # Linux example → change to your interface name
> ```

---

## 📊 Sample Output

```
[*] Target: 192.168.1.1
[*] Scan type: syn
[*] Interface: eth0
[*] Scan started...

[+] Port 22/tcp OPEN   | Service: SSH    | Banner: OpenSSH_8.9p1 Ubuntu
[+] Port 80/tcp OPEN   | Service: HTTP   | Banner: Apache/2.4.52 (Ubuntu)
[+] Port 443/tcp OPEN  | Service: HTTPS  | No banner detected
```

---

## ⚙️ Detected Services

The scanner identifies the following common services automatically:

`FTP` · `SSH` · `Telnet` · `SMTP` · `DNS` · `HTTP` · `POP3` · `IMAP` · `HTTPS` · `SMB` · `MySQL` · `RDP` · `HTTP-Alt`

Any port outside this list will be labeled `unknown`.

---

## ⚠️ Disclaimer

This tool is intended for **educational and authorized security testing only**. Only scan systems and networks you own or have explicit written permission to test. Unauthorized port scanning may be illegal in your jurisdiction.

---

## 📄 License

MIT License — see the root [`LICENSE`](../../LICENSE) file for details.
