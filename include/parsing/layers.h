//
// Created by tomas on 21/03/19.
//

#ifndef TRAFFIC_COLLECTOR_LAYER_H
#define TRAFFIC_COLLECTOR_LAYER_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "operations.h"

namespace Packet {
    enum Type {
        Unknown = 0xffff,
        Ethernet = 0x0001,
        ARP = 0x0806,
        IPv4 = 0x0800,
        IPv6 = 0x86dd,
        ICMP = 0x3A,
        UDP = 0x11,
        TCP = 0x06,
        DNS = 0x35
    };

    const std::map<Type, const std::string> supported_layers = {
            {Ethernet, "ethernet"},
            {ARP,      "arp"},
            {IPv4,     "ipv4"},
            {IPv6,     "ipv6"},
            {ICMP,     "icmp"},
            {UDP,      "udp"},
            {TCP,      "tcp"},
            {DNS,      "dns"},
    };
}

class Layer {
public:

};

class Ethernet : public Layer {
public:
    struct Ethernet_header {
        uint8_t dst_mac[6];
        uint8_t src_mac[6];
        uint16_t ethertype;
    };

    static struct Ethernet_header *parse(uint8_t *bytes) {
        auto *eth = (Ethernet_header *) malloc(sizeof(Ethernet_header));

        memcpy(eth->dst_mac, bytes, 6);
        memcpy(eth->src_mac, (bytes + 6), 6);
        eth->ethertype = (bytes[12] << 8) | bytes[13];

        return eth;
    }
};

class ARP : public Layer {
public:
    struct ARP_header {
        uint16_t hw_type;
        uint16_t ip_type;
        uint8_t hwaddr_len;
        uint8_t l3addr_len;
        uint16_t opcode;
        uint8_t sender_mac[6];
        uint32_t sender_ip;
        uint8_t target_mac[6];
        uint32_t target_ip;
    };
};

class IPv4 : public Layer {
public:
    struct IPv4_header {
        uint8_t version; // 4 bits
        uint8_t ihl; // 4 bits
        uint8_t tos; // 3 bit precedence + 5 single bit flags
        uint16_t total_length; // number of 32bit words (minimum is 5)
        uint16_t identification;
        uint8_t flags; // 3 bits defining fragmentation
        uint16_t offset; // 13 bits
        uint8_t ttl;
        uint8_t protocol;
        uint16_t hdr_checksum;
        uint32_t src_ip;
        uint32_t dst_ip;
    };

    static IPv4_header *parse(uint8_t *bytes) {
        auto *ipv4 = (IPv4_header *) malloc(sizeof(IPv4_header));

        ipv4->version = static_cast<uint8_t>( (bytes[0] & 0xf0) >> 4 );
        ipv4->ihl = static_cast<uint8_t>(bytes[0] & 0x0f);
        ipv4->tos =bytes[1];
        ipv4->total_length = static_cast<uint16_t>( ((bytes[2] & 0x00ff) << 8) | (bytes[3] & 0x00ff) );
        ipv4->identification = static_cast<uint16_t>( ((bytes[4] & 0x00ff) << 8) | (bytes[5] & 0x00ff) );
        ipv4->flags = static_cast<uint8_t>( (bytes[6] >> 5) & 0x07 );
        ipv4->offset = static_cast<uint16_t>( ((bytes[6] & 0x001f) << 5) | bytes[7] );
        ipv4->ttl = bytes[8];
        ipv4->protocol = bytes[9];
        ipv4->hdr_checksum = static_cast<uint16_t>( ((bytes[10] & 0x00ff) << 8) | (bytes[11] & 0x00ff) );
        ipv4->src_ip = static_cast<uint32_t>( ((bytes[12] & 0x000000ff) << 24) | ((bytes[13] & 0x000000ff) << 16) | ((bytes[14] & 0x000000ff) << 8) | (bytes[15] & 0x000000ff) );
        ipv4->dst_ip = static_cast<uint32_t>( ((bytes[16] & 0x000000ff) << 24) | ((bytes[17] & 0x000000ff) << 16) | ((bytes[18] & 0x000000ff) << 8) | (bytes[19] & 0x000000ff) );
    }
};

class IPv6 : public Layer {
public:
    struct IPv6_header {
        uint8_t version; // 4 bits
        uint8_t traffic_class;
        uint32_t flow_label; // 20 bits
        uint16_t payload_len;
        uint8_t next_hdr;
        uint8_t hop_limit;
        uint16_t src_ip[8];
        uint16_t dst_ip[8];
    };
};

class ICMP : public Layer {
public:
    struct ICMP_header {
        uint8_t type;
        uint8_t code;
        uint16_t hdr_checksum;
        // and further fields for all code/type combinations
    };
};

class UDP : public Layer {
public:
    struct UDP_header {
        uint16_t src_port;
        uint16_t dst_port;
        uint16_t length;
        uint16_t checksum;
        uint8_t *data;
    };
};

class TCP : public Layer {
public:
    struct TCP_header {
        uint16_t src_port;
        uint16_t dst_port;
        uint32_t seq;
        uint32_t ack;
        uint8_t offset; // 4 bits,
        // reserved 6 bits
        uint8_t control_bits; // 6 bits
        uint16_t window;
        uint16_t checksum;
        uint16_t urgent_ptr;
        // tcp options;
    };
};

#endif //TRAFFIC_COLLECTOR_LAYER_H
