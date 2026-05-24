
echo "[*] Starting system hardening..."

#firewall-network hardening
echo "[*] Configuring firewall..."

apt update -y
apt install ufw -y

ufw default deny incoming
ufw default allow outgoing

#allow essential ports
ufw allow 22    # SSH
ufw allow 80    # HTTP
ufw allow 443   # HTTPS

ufw --force enable

#kernel hardening
echo "[*] Applying kernel hardening..."

cat <<EOF >> /etc/sysctl.conf

#disable IPv6
net.ipv6.conf.all.disable_ipv6 = 1

#SYN flood protection
net.ipv4.tcp_syncookies = 1

#ignore ICMP broadcast
net.ipv4.icmp_echo_ignore_broadcasts = 1

#disable IP forwarding
net.ipv4.ip_forward = 0

#log suspicious packets
net.ipv4.conf.all.log_martians = 1

EOF

sysctl -p

#services hardening
echo "[*] Disabling unnecessary services..."

systemctl disable bluetooth 2>/dev/null
systemctl stop bluetooth 2>/dev/null

systemctl disable cups 2>/dev/null
systemctl stop cups 2>/dev/null

systemctl disable avahi-daemon 2>/dev/null
systemctl stop avahi-daemon 2>/dev/null

#user hardening
echo "[*] Hardening user permissions..."

#remove user from wireshark group (if exists)
gpasswd -d $SUDO_USER wireshark 2>/dev/null

#disable root SSH login
if [ -f /etc/ssh/sshd_config ]; then
    sed -i 's/^#PermitRootLogin.*/PermitRootLogin no/' /etc/ssh/sshd_config
    sed -i 's/^PermitRootLogin.*/PermitRootLogin no/' /etc/ssh/sshd_config
    systemctl restart ssh 2>/dev/null
fi

#system resource limits
echo "[*] Limiting system resources..."

cat <<EOF >> /etc/security/limits.conf

#resource limits
* hard cpu 50
* hard nproc 100
* hard nofile 1024

EOF

#ultimate lockdown feature
echo "[*] Creating emergency lockdown tool..."

cat <<EOF > /usr/local/bin/lockdown.sh

echo "[!] Activating lockdown mode..."
ufw default deny incoming
systemctl stop ssh
echo "[!] System is now isolated."
EOF

chmod +x /usr/local/bin/lockdown.sh

echo "[+] Hardening completed successfully!"
echo "[+] Use 'lockdown.sh' in case of emergency."
