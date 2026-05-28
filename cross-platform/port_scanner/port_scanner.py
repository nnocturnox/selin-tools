#!/usr/bin/env python3
from scapy.all import IP, TCP, sr1, send, conf
import argparse
import socket

conf.iface = "eth0"

# Mapping of common ports to service names for identification
COMMON_SERVICES = {
    21: "FTP", 22: "SSH", 23: "Telnet", 25: "SMTP", 53: "DNS",
    80: "HTTP", 110: "POP3", 143: "IMAP", 443: "HTTPS", 445: "SMB",
    3306: "MySQL", 3389: "RDP", 8080: "HTTP-Alt"
}

def detect_banner(target_ip, port):
    """
    Attempts to grab the service banner by establishing a socket connection.
    """
    try:
        s = socket.socket()
        s.settimeout(2)
        s.connect((target_ip, port))
        
        # Some services reveal their name/version immediately 
        banner = s.recv(1024).decode(errors="ignore").strip()
        if not banner:
            # Trigger a response if the service is silent initially
            s.send(b"HEAD / HTTP/1.0\r\n\r\n")
            banner = s.recv(1024).decode(errors="ignore").strip()
        s.close()
        return banner if banner else "No banner detected"
    except:
        return None

def syn_scan(target_ip, port):
    """
    Performs a TCP SYN scan (Stealth Scan)
    """
    packet = IP(dst=target_ip) / TCP(dport=port, flags="S")
    response = sr1(packet, timeout=1, verbose=0)

    if response is None:
        return "FILTERED" 
    elif response.haslayer(TCP):
        if response[TCP].flags == 0x12: # SYN-ACK detected (Port is OPEN) 
            send(IP(dst=target_ip) / TCP(dport=port, flags="R"), verbose=0) # Send RST to close
            return "OPEN"
        elif response[TCP].flags == 0x14: # RST detected (Port is CLOSED) 
            return "CLOSED"
    return "FILTERED"

def connect_scan(target_ip, port):
    """
    Performs a full TCP Connect scan
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(1)
    result = s.connect_ex((target_ip, port))
    s.close()
    return "OPEN" if result == 0 else "CLOSED"

def ack_scan(target_ip, port):
    """
    Performs a TCP ACK scan to analyze firewall filtering
    """
    packet = IP(dst=target_ip) / TCP(dport=port, flags="A")
    response = sr1(packet, timeout=1, verbose=0)

    if response is None:
        return "FILTERED"
    elif response.haslayer(TCP):
        if response[TCP].flags == 0x04: # RST detected
            return "UNFILTERED" 
    return "FILTERED"

def main():
    parser = argparse.ArgumentParser(description="TCP Port Scanner")
    parser.add_argument("--target", required=True, help="Target IP address")
    parser.add_argument("--scan", choices=["syn", "connect", "ack"], required=True, help="Scan type")
    parser.add_argument("--ports", help="Port range (e.g., 1-100). Scans all ports if empty.")
    parser.add_argument("--service-detection", action="store_true", help="Enable service identification.")
    
    args = parser.parse_args()

    # Define port range
    if args.ports:
        if "-" in args.ports:
            start, end = map(int, args.ports.split("-"))
            ports = range(start, end + 1) 
        else:
            ports = [int(args.ports)]
    else:
        ports = range(1, 65536) 

    print(f"[*] Target: {args.target}")
    print(f"[*] Scan type: {args.scan}")
    print(f"[*] Interface: {conf.iface}")
    print("[*] Scan started...\n")

    for port in ports:
        if args.scan == "syn":
            result = syn_scan(args.target, port)
        elif args.scan == "connect":
            result = connect_scan(args.target, port)
        else:
            result = ack_scan(args.target, port)

        if result in ["OPEN", "UNFILTERED"]:
            service = COMMON_SERVICES.get(port, "unknown")
            output = f"[+] Port {port}/tcp {result} | Service: {service}"
            
            if args.service_detection and result == "OPEN":
                banner = detect_banner(args.target, port)
                if banner:
                    output += f" | Banner: {banner}"
            print(output)

if __name__ == "__main__":
    main()
