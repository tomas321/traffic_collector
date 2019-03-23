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

    static Ethernet_header *parse(uint8_t *bytes) {
        auto *eth = (Ethernet_header *) malloc(sizeof(Ethernet_header));

        memcpy(eth->dst_mac, bytes, 6);
        memcpy(eth->src_mac, (bytes + 6), 6);
        eth->ethertype = Bytes::merge_bytes(bytes[12], bytes[13]);

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

    static ARP_header *parse(uint8_t *bytes) {
        auto *arp = (ARP_header *) malloc(sizeof(ARP_header));

        arp->hw_type = Bytes::merge_bytes(bytes[0], bytes[1]);
        arp->ip_type = Bytes::merge_bytes(bytes[2], bytes[3]);
        arp->hwaddr_len = bytes[4];
        arp->l3addr_len = bytes[5];
        arp->opcode = Bytes::merge_bytes(bytes[6], bytes[7]);
        memcpy(arp->sender_mac, (bytes + 8), 6);
        arp->sender_ip = Bytes::merge_bytes(bytes[14], bytes[15], bytes[16], bytes[17]);
        memcpy(arp->target_mac, (bytes + 18), 6);
        arp->target_ip = Bytes::merge_bytes(bytes[24], bytes[25], bytes[26], bytes[27]);

        return arp;
    }
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
        ipv4->tos = bytes[1];
        ipv4->total_length = Bytes::merge_bytes(bytes[2], bytes[3]);
        ipv4->identification = Bytes::merge_bytes(bytes[4], bytes[5]);
        ipv4->flags = static_cast<uint8_t>( (bytes[6] >> 5) & 0x07 );
        ipv4->offset = static_cast<uint16_t>( ((bytes[6] & 0x001f) << 5) | bytes[7] );
        ipv4->ttl = bytes[8];
        ipv4->protocol = bytes[9];
        ipv4->hdr_checksum = Bytes::merge_bytes(bytes[10], bytes[11]);
        ipv4->src_ip = Bytes::merge_bytes(bytes[12], bytes[13], bytes[14], bytes[15]);
        ipv4->dst_ip = Bytes::merge_bytes(bytes[16], bytes[17], bytes[18], bytes[19]);

        return ipv4;
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

    static IPv6_header *parse(uint8_t *bytes) {
        auto ipv6 = (IPv6_header *) malloc(sizeof(IPv6_header));

        ipv6->version = static_cast<uint8_t>((bytes[0] & 0xf0) >> 4);
        ipv6->traffic_class = static_cast<uint8_t>( ((bytes[0] & 0x0f) << 4) | ((bytes[1] & 0xf0) >> 4) );
        ipv6->flow_label = Bytes::merge_bytes(0x00, static_cast<const uint8_t>(bytes[1] & 0x0f), bytes[2], bytes[3]);
        ipv6->payload_len = Bytes::merge_bytes(bytes[4], bytes[5]);
        ipv6->next_hdr = bytes[6];
        ipv6->hop_limit = bytes[7];
        int j = 0;
        for (int i = 8; i < 24; i += 2)
            ipv6->src_ip[j++] = Bytes::merge_bytes(bytes[i], bytes[i + 1]);
        j = 0;
        for (int i = 24; i < 40; i += 2)
            ipv6->dst_ip[j++] = Bytes::merge_bytes(bytes[i], bytes[i + 1]);

        return ipv6;
    }
};

class ICMP : public Layer {
public:
    struct ICMP_header {
        uint8_t type;
        uint8_t code;
        uint16_t hdr_checksum;
        // and further fields for all code/type combinations
    };

    static ICMP_header *parse(uint8_t *bytes) {
        auto *icmp = (ICMP_header *) malloc(sizeof(ICMP_header));

        icmp->type = bytes[0];
        icmp->code = bytes[1];
        icmp->hdr_checksum = Bytes::merge_bytes(bytes[2], bytes[3]);
        // TODO: add other fields

        return icmp;
    }
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

    static UDP_header *parse(uint8_t *bytes) {
        auto *udp = (UDP_header *) malloc(sizeof(UDP_header));

        udp->src_port = Bytes::merge_bytes(bytes[0], bytes[1]);
        udp->dst_port = Bytes::merge_bytes(bytes[2], bytes[3]);
        udp->length = Bytes::merge_bytes(bytes[4], bytes[5]);
        udp->checksum = Bytes::merge_bytes(bytes[6], bytes[7]);

        udp->data = (uint8_t *) malloc(static_cast<size_t>(udp->length - 8));
        memcpy(udp->data, (bytes + 8), static_cast<size_t>(udp->length - 8));

        return udp;
    }
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

    static TCP_header *parse(uint8_t *bytes) {
        auto *tcp = (TCP_header *) malloc(sizeof(TCP_header));

        tcp->src_port = Bytes::merge_bytes(bytes[0], bytes[1]);
        tcp->dst_port = Bytes::merge_bytes(bytes[2], bytes[3]);
        tcp->seq = Bytes::merge_bytes(bytes[4], bytes[5], bytes[6], bytes[7]);
        tcp->ack = Bytes::merge_bytes(bytes[8], bytes[9], bytes[10], bytes[11]);
        tcp->offset = static_cast<uint8_t>( (bytes[12] & 0xf0) >> 4 );
        tcp->control_bits = static_cast<uint8_t>(bytes[13] & 0x3f);
        tcp->window = Bytes::merge_bytes(bytes[14], bytes[15]);
        tcp->checksum = Bytes::merge_bytes(bytes[16], bytes[17]);
        tcp->urgent_ptr = Bytes::merge_bytes(bytes[18], bytes[19]);

        return tcp;
    }
};

#endif //TRAFFIC_COLLECTOR_LAYER_H
