# 🪟 Windows — System & Security Tools

A collection of PowerShell scripts and utilities designed for Windows system maintenance and privacy. These tools are built for educational purposes, home lab use, and users who want to automate security-related tasks on their Windows environments.

---

## 📁 Folder Structure

```
windows/
├── cleanup/                # Artifact removal scripts (PowerShell)
│   ├── cleanup.ps1             # Removes browser history, cache, and recent files at logoff
│   └── README.md
└── README.md
```

---

## 🔍 Modules Overview

### `cleanup/`
An automated PowerShell script that wipes digital traces left by everyday Windows usage. Runs at user logoff via Task Scheduler.

| Script | Description |
|---|---|
| `cleanup.ps1` | Clears Chrome/Edge history & cache, Jump Lists, and Recent Files |

---

## ⚙️ General Requirements

- **OS:** Windows 10 or Windows 11
- **Privileges:** Administrator rights required for execution policy configuration
- **Dependencies:** PowerShell (built-in)

---

## ⚠️ Disclaimer

These scripts are intended for **educational and personal privacy purposes only**. Deletion performed by these tools is permanent. Always review the scripts before running them on your system. The authors are not responsible for any data loss caused by misuse.

---

## 📄 License

MIT License — see the root [`LICENSE`](../../LICENSE) file for details.
