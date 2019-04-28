//
// Created by tomas on 23/03/19.
//

#ifndef TRAFFIC_COLLECTOR_LAYERS2_H
#define TRAFFIC_COLLECTOR_LAYERS2_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>

#include <iostream>

using namespace std;

namespace Layers {
    /**
     * All supported Layer types.
     */
    enum Type {
        NONE = 0xffff,
        Unknown = 0x0000,
        Ethernet = 0x1111,
        ARP = 0x0806,
        IPv4 = 0x0800,
        IPv6 = 0x86dd,
        ICMP = 0x01,
        ICMPv6 = 0x3a,
        UDP = 0x11,
        TCP = 0x06,
        DCCP = 0x21,
        RDP = 0x1b,
        DNS = 0x35
    };

    /**
     * Mapping supported types to string representations.
     */
    inline const string layer_string(Type type) {
        const map<Type, const string> lmap = {
                {Ethernet, "ethernet"},
                {ARP,      "arp"},
                {IPv4,     "ipv4"},
                {IPv6,     "ipv6"},
                {ICMP,     "icmp"},
                {ICMPv6,   "icmpv6"},
                {UDP,      "udp"},
                {TCP,      "tcp"},
                {DCCP,     "dccp"},
                {RDP,      "rdp"},
                {DNS,      "dns"},
                {NONE,     "none"},
                {Unknown,  "unknown"}
        };

        try {
            return lmap.at(type);
        } catch (const std::out_of_range &e) {
            return lmap.at(Unknown);
        }
    }

    /**
     * Mapping headers to header lengths.
     */
    inline const int header_lengths(Type type) {
        const map<Type, const int> lmap = {
                {Ethernet, 14},
                {ARP,      28},
                {IPv4,     20},
                {IPv6,     24},
                {ICMP,     4}, // temporary
                {ICMPv6,   4}, // temporary
                {UDP,      8},
                {TCP,      20},
                {DCCP,     16}, // 16 or 12
                {RDP,      9}, // default length but is varying
                {DNS,      12}
        };

        try {
            return lmap.at(type);
        } catch (const std::out_of_range &e) {
            throw std::out_of_range("hdr length");
        }
    }
}

class Layer {
private:
    /**
     * Pointer to the next Layer in double linked list.
     */
    Layer *next;

    /**
     * Pointer to the previous layer in double linked list.
     */
    Layer *previous;

protected:
    /**
     * Current layer type.
     */
    Layers::Type layer_type;

public:
    Layer *get_next() const {
        return next;
    }

    Layer *get_previous() const {
        return previous;
    }

    Layers::Type get_layer_type() const {
        return layer_type;
    }

    void set_next(Layer *next) {
        this->next = next;
    }

    void set_previous(Layer *previous) {
        this->previous = previous;
    }
};

#endif //TRAFFIC_COLLECTOR_LAYERS2_H
