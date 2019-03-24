//
// Created by tomas on 23/03/19.
//

#ifndef TRAFFIC_COLLECTOR_LAYERS2_H
#define TRAFFIC_COLLECTOR_LAYERS2_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>

using namespace std;

namespace Layers {
    /**
     * All supported Layer types.
     */
    enum Type {
        NONE = 0xffff,
        Unknown = 0x0000,
        Ethernet = 0x0001,
        ARP = 0x0806,
        IPv4 = 0x0800,
        IPv6 = 0x86dd,
        ICMP = 0x3A,
        UDP = 0x11,
        TCP = 0x06,
        DNS = 0x35
    };

    /**
     * Mapping supported types to string representations.
     */
    const map<Type, const string> layer_string = {
            {Ethernet, "ethernet"},
            {ARP,      "arp"},
            {IPv4,     "ipv4"},
            {IPv6,     "ipv6"},
            {ICMP,     "icmp"},
            {UDP,      "udp"},
            {TCP,      "tcp"},
            {DNS,      "dns"},
            {NONE,     "none"},
            {Unknown,  "unknown"}
    };

    /**
     * Mapping headers to header lengths.
     */
    const map<Type, const int> header_lengths = {
            {Ethernet, 14},
            {ARP,      28},
            {IPv4,     20},
            {IPv6,     24},
            {ICMP,     4}, // temporary
            {UDP,      8},
            {TCP,      20},
            {DNS,      12}
    };
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
        Layer::next = next;
    }

    void set_previous(Layer *previous) {
        Layer::previous = previous;
    }
};

#endif //TRAFFIC_COLLECTOR_LAYERS2_H
