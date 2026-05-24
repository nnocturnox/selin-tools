#include <pcap.h>
#include <iostream>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

const int SIZE_THRESHOLD = 2000;

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    /* Skip the Ethernet header (14 bytes) to access the IP header */
    struct iphdr *ip = (struct iphdr *)(packet + 14);

    /* Specifically check for the ICMP protocol (protocol 1) */
    if (ip->protocol == IPPROTO_ICMP) {
        /* * Extract the fragment offset from the 13-bit offset field.
         * The offset is represented in 8-byte blocks, so multiply by 8 to get bytes.
         */
        int offset = (ntohs(ip->frag_off) & IP_OFFMASK) * 8;
        /* Calculate the data length carried in this specific fragment */
        int frag_data_len = ntohs(ip->tot_len) - (ip->ihl * 4);
        /* * Estimate total reassembled size.
         * The end position of the fragment indicates the cumulative packet size so far.
         * The formula used is $TotalSize = Offset + FragmentLength$.
         */
        int estimated_total_size = offset + frag_data_len;

        if (estimated_total_size > SIZE_THRESHOLD) {
            std::cout << "\n[!!!] ALERT: Oversized ICMP Packet Detected!" << std::endl;
            std::cout << "[*] Source IP: " << inet_ntoa(*(struct in_addr *)&ip->saddr) << std::endl;
            std::cout << "[*] Estimated Total Reassembled Size: " << estimated_total_size << " bytes" << std::endl;
            
            if (estimated_total_size >= 65535) {
                std::cout << "[CRITICAL] Potential Ping of Death Attack!" << std::endl;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <interface>" << std::endl;
        return 1;
    }

    char *dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    /* * Filter expression to capture:
     * 1. Standard ICMP packets.
     * 2. Any IP fragments (where the 'MF' flag is set or the offset is non-zero).
     */
    char filter_exp[] = "icmp or (ip[6:2] & 0x3fff != 0)"; 

    /* Open the network interface for live capture */
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        std::cerr << "Device error: " << errbuf << std::endl;
        return 2;
    }

    /* Compile and apply the Berkeley Packet Filter (BPF) */
    struct bpf_program fp;
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Filter error: " << pcap_geterr(handle) << std::endl;
        return 2;
    }
    pcap_setfilter(handle, &fp);

    std::cout << "[*] Monitoring " << dev << " for oversized ICMP packets (Threshold: " << SIZE_THRESHOLD << " bytes)..." << std::endl;

    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_close(handle);
    return 0;
}
