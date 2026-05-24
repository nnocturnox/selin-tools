# 🛡️ Hardening — Linux System Hardening Script

An automated Bash script that applies essential security configurations to a fresh Debian/Ubuntu Linux installation. Designed to quickly reduce the attack surface of a system by hardening the firewall, kernel parameters, SSH, and more.

---

## 📄 Script

### `hardening.sh`

Applies a comprehensive set of hardening measures in a single run. The script is modular — each section targets a different layer of the system.

---

## 🔧 What It Does

### 🔥 Firewall Configuration (UFW)
- Installs and enables **UFW** (Uncomplicated Firewall)
- Sets default policy: **deny all incoming**, allow all outgoing
- Opens only essential ports: **22 (SSH)**, **80 (HTTP)**, **443 (HTTPS)**

### 🧠 Kernel Hardening (`/etc/sysctl.conf`)
| Parameter | Value | Purpose |
|---|---|---|
| `net.ipv6.conf.all.disable_ipv6` | `1` | Disables IPv6 to reduce attack surface |
| `net.ipv4.tcp_syncookies` | `1` | Enables SYN cookie protection against SYN floods |
| `net.ipv4.icmp_echo_ignore_broadcasts` | `1` | Ignores ICMP broadcast pings |
| `net.ipv4.ip_forward` | `0` | Disables IP packet forwarding |
| `net.ipv4.conf.all.log_martians` | `1` | Logs packets with suspicious/spoofed source addresses |

### 🔌 Disabling Unnecessary Services
The following services are stopped and disabled:
- `bluetooth` — Bluetooth daemon
- `cups` — Printing system
- `avahi-daemon` — mDNS/DNS-SD service discovery

### 👤 User & SSH Hardening
- Removes the current user from the `wireshark` group (prevents unauthorized packet captures)
- Disables **root login via SSH** by modifying `/etc/ssh/sshd_config`

### 📦 System Resource Limits (`/etc/security/limits.conf`)
| Limit | Value | Description |
|---|---|---|
| `hard cpu` | `50` | Max CPU time per user |
| `hard nproc` | `100` | Max number of processes per user |
| `hard nofile` | `1024` | Max number of open file descriptors |

### 🔒 Emergency Lockdown Tool
Creates a script at `/usr/local/bin/lockdown.sh` that, when executed, instantly:
- Blocks all incoming connections via UFW
- Stops the SSH service

Use this in case of an active intrusion or emergency.

---

## 🚀 Installation & Usage

### 1. Clone or download the script

```bash
git clone https://github.com/nnocturnox/selin-tools.git
cd selin-tools/linux/hardening
```

### 2. Make the script executable

```bash
chmod +x hardening.sh
```

### 3. Run with root privileges

```bash
sudo ./hardening.sh
```

> ⚠️ **Important:** Run this script on a fresh system or a test environment first. Some configurations (e.g., disabling services, modifying SSH) may affect your ability to connect remotely.

---

## 📊 Sample Output

```
[*] Starting system hardening...
[*] Configuring firewall...
[*] Applying kernel hardening...
[*] Disabling unnecessary services...
[*] Hardening user permissions...
[*] Limiting system resources...
[*] Creating emergency lockdown tool...
[+] Hardening completed successfully!
[+] Use 'lockdown.sh' in case of emergency.
```

---

## 🆘 Emergency Lockdown

If you detect an active threat, run the lockdown tool immediately:

```bash
sudo lockdown.sh
```

This will isolate the machine by blocking all incoming traffic and shutting down SSH access. To restore normal operation:

```bash
sudo ufw default allow incoming   # or restore your previous rules
sudo systemctl start ssh
```

---

## ⚙️ Requirements

- **OS:** Ubuntu / Debian-based distributions
- **Privileges:** Must be run as `root` or with `sudo`
- **Dependencies:** All required packages (`ufw`) are installed automatically by the script

---

## ⚠️ Disclaimer

This script modifies core system configurations. Always test in a safe or virtual environment before applying to a production system. The authors are not responsible for any service disruptions caused by this script.

---

## 📄 License

MIT License — see the root [`LICENSE`](../../LICENSE) file for details.
