#ifndef TRAFFIC_COLLECTOR_ETHERNET_H
#define TRAFFIC_COLLECTOR_ETHERNET_H

#include "layer.h"

class Ethernet : public Layer {
public:
    struct Ethernet_header {
        uint8_t dst_mac[6];
        uint8_t src_mac[6];
        uint16_t ethertype;
    };

    Ethernet_header *header;

    /**
     * Create Ethernet Layer of parsed header fields.
     *
     * @param bytes Raw bytes of Ethernet header.
     */
    explicit Ethernet(const uint8_t *bytes);
};

#endif //TRAFFIC_COLLECTOR_ETHERNET_H
