# 🌐 Cross-Platform — System & Security Tools

A collection of scripts and utilities compatible with both **Windows** and **Linux** environments.

---

## 📁 Folder Structure

```
cross-platform/
├── port_scanner/           # TCP port scanner (Python / Scapy)
│   ├── portscanner.py
│   └── README.md
└── README.md
```

---

## 🔍 Modules Overview

### `port_scanner/`
A Python-based TCP port scanner supporting SYN, Connect, and ACK scan types with optional service and banner detection.

| Script | Description |
|---|---|
| `port_scanner.py` | Multi-mode TCP port scanner with service and banner detection |

---

## ⚙️ General Requirements

- **OS:** Windows 10/11 or Linux (Debian/Ubuntu-based)
- **Dependencies:** Python 3, Scapy
- **Windows only:** [Npcap](https://npcap.com/#download) required for raw packet capture

---

## ⚠️ Disclaimer

These tools are intended for **educational and authorized security testing only**. Only use them on systems and networks you own or have explicit permission to test. The authors are not responsible for any misuse.

---

## 📄 License

MIT License — see the root [`LICENSE`](../LICENSE) file for details.
