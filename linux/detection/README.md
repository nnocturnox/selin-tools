# 🔎 Detection — Network Attack Detectors

Real-time network intrusion detection tools written in **C++** using the **libpcap** library. These programs passively monitor a network interface and raise alerts when attack patterns are identified.

---

## 📄 Tools

### `detector.cpp` — SYN Flood Detector

Monitors incoming TCP traffic and detects **SYN Flood** attacks by counting pure SYN packets (i.e., not SYN-ACK) within a one-second window. If the count exceeds the defined threshold, an alert is printed to the terminal.

**How it works:**
- Uses a BPF filter to capture only TCP SYN packets (`tcp-syn` set, `tcp-ack` not set)
- Resets the packet counter every second
- Fires an alert if `syn_count > THRESHOLD` (default: **50 packets/sec**)

---

### `pod_detector.cpp` — Ping of Death Detector

Monitors ICMP traffic and IP fragments to detect **Ping of Death (PoD)** attacks. It estimates the total reassembled packet size from individual fragments and raises an alert if it exceeds a safe threshold.

**How it works:**
- Captures all ICMP packets and any fragmented IP packets
- Reconstructs the estimated total size using: `TotalSize = FragmentOffset + FragmentLength`
- Alerts if estimated size exceeds **2000 bytes**
- Marks packets at or above **65535 bytes** as a critical PoD indicator

---

### `local.rules` — Snort IDS Rule

A custom **Snort** rule using rate-based detection to identify **horizontal TCP SYN scanning** behavior. Instead of evaluating individual packets, it tracks how many SYN packets a single source sends within a time window.

```snort
alert tcp any any -> any any (
    msg:"Suspicious TCP SYN Traffic Detected";
    flags:S;
    detection_filter:track by_src, count 20, seconds 1;
    sid:2000002;
    rev:1;
)
```

**How it works:**
- Matches packets with only the TCP SYN flag set (`flags:S`)
- Tracks packet rate per source IP (`track by_src`)
- Triggers an alert if a source sends **20+ SYN packets within 1 second**

---

## 🛠️ Installation & Compilation

### 1. Install Dependencies

```bash
sudo apt update
sudo apt install g++ libpcap-dev -y
```

### 2. Compile

```bash
# SYN Flood Detector
g++ detector.cpp -o detector -lpcap

# Ping of Death Detector
g++ pod_detector.cpp -o pod_detector -lpcap
```

### 3. Install Snort & Load the Rule

```bash
sudo apt install snort -y
sudo cp local.rules /etc/snort/rules/local.rules
```

Make sure this line exists in `/etc/snort/snort.conf`:
```
include $RULE_PATH/local.rules
```

---

## 🚀 Usage

Both tools require a network interface name as an argument and must be run with root privileges.

```bash
# SYN Flood Detector
sudo ./detector <interface>

# Ping of Death Detector
sudo ./pod_detector <interface>
```

**Example:**
```bash
sudo ./detector eth0
sudo ./pod_detector eth0
```

To find your active interface:
```bash
ip link show
# or
ifconfig
```

**Snort in IDS mode:**
```bash
sudo snort -A console -q -c /etc/snort/snort.conf -i eth0
```

---

## 📊 Sample Output

**SYN Flood Detector:**
```
[*] Monitoring interface eth0 for SYN flooding...
..................................................
[!!!] ALERT: SYN Flood Attack Detected!
[*] Packets in last second: 87
```

**Ping of Death Detector:**
```
[*] Monitoring eth0 for oversized ICMP packets (Threshold: 2000 bytes)...

[!!!] ALERT: Oversized ICMP Packet Detected!
[*] Source IP: 192.168.1.105
[*] Estimated Total Reassembled Size: 65535 bytes
[CRITICAL] Potential Ping of Death Attack!
```

**Snort (local.rules):**
```
[**] [1:2000002:1] Suspicious TCP SYN Traffic Detected [**]
05/24-14:32:01.123456 172.16.2.96:54321 -> 93.184.216.34:80
TCP TTL:64 ******S*
```

---

## ⚙️ Configuration

You can adjust detection sensitivity by modifying the following constants in the source files:

| File | Constant | Default | Description |
|---|---|---|---|
| `detector.cpp` | `THRESHOLD` | `50` | Max SYN packets per second before alert |
| `pod_detector.cpp` | `SIZE_THRESHOLD` | `2000` | Max ICMP packet size in bytes before alert |
| `local.rules` | `count` | `20` | Max SYN packets per second before Snort alert |

After changing a value, recompile the binary.

---

## ⚠️ Disclaimer

These tools are for **educational and defensive use only**. Only monitor networks and systems you own or have explicit authorization to test. Passive packet capture may be subject to local laws and regulations.

---

## 📄 License

MIT License — see the root [`LICENSE`](../../LICENSE) file for details.
