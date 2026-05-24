#include <pcap.h>
#include <iostream>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <ctime>

int syn_count = 0;
time_t last_check_time = 0;
const int THRESHOLD = 50; 

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    time_t current_time = header->ts.tv_sec;

    // Reset counter every second
    if (current_time > last_check_time) {
        if (syn_count > THRESHOLD) {
            std::cout << "\n[!!!] ALERT: SYN Flood Attack Detected!" << std::endl;
            std::cout << "[*] Packets in last second: " << syn_count << std::endl;
        }
        syn_count = 0;
        last_check_time = current_time;
    }

    // Capture logic: The filter already ensures these are TCP SYN packets
    syn_count++;
    std::cout << "." << std::flush; // Visual indicator for incoming packets
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <interface>" << std::endl;
        return 1;
    }

    char *dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program fp;
    
    // Filter: Capture only SYN packets that are NOT part of a SYN-ACK (ack == 0)
    char filter_exp[] = "tcp[tcpflags] & (tcp-syn) != 0 and tcp[tcpflags] & (tcp-ack) == 0";

    // Open the interface for live capture
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        std::cerr << "Could not open device " << dev << ": " << errbuf << std::endl;
        return 2;
    }

    // Compile and apply the filter
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Could not parse filter " << filter_exp << std::endl;
        return 2;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "Could not install filter" << std::endl;
        return 2;
    }

    std::cout << "[*] Monitoring interface " << dev << " for SYN flooding..." << std::endl;
    last_check_time = time(NULL);

    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_close(handle);
    return 0;
}
