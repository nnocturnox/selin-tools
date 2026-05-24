# 🐧 Linux — System & Security Tools

A collection of Bash scripts and C++ utilities designed for Linux system hardening and network attack detection. These tools are built for educational purposes, home lab use, and system administrators who want to monitor and protect their Linux environments.

---

## 📁 Folder Structure

```
linux/
├── detection/         # Network attack detection tools (C++)
│   ├── detector.cpp       # SYN Flood attack detector
│   ├── pod_detector.cpp   # Ping of Death (PoD) detector
│   └── README.md
├── hardening/         # System hardening scripts (Bash)
│   ├── hardening.sh       # Automated Linux hardening script
│   └── README.md
└── README.md
```

---

## 🔍 Modules Overview

### `detection/`
Real-time network traffic monitoring tools built with **libpcap**. These programs listen on a specified network interface and alert when suspicious traffic patterns are detected.

| Tool | Description |
|---|---|
| `detector.cpp` | Detects SYN Flood attacks by counting TCP SYN packets per second |
| `pod_detector.cpp` | Detects oversized ICMP packets indicative of a Ping of Death attack |

### `hardening/`
An automated Bash script that applies a series of security configurations to a fresh Linux system.

| Script | Description |
|---|---|
| `hardening.sh` | Configures UFW firewall, kernel parameters, disables unused services, and hardens SSH |

---

## ⚙️ General Requirements

- **OS:** Ubuntu / Debian-based Linux distributions
- **Privileges:** `sudo` or root access required for all tools
- **Dependencies:**
  - `libpcap-dev` — for C++ detection tools
  - `ufw` — for the hardening script (auto-installed)
  - `g++` — to compile C++ source files

---

## ⚠️ Disclaimer

These tools are intended for **educational and defensive security purposes only**. Always obtain proper authorization before monitoring any network you do not own. The authors are not responsible for any misuse of these tools.

---

## 📄 License

MIT License — see the root [`LICENSE`](../../LICENSE) file for details.
