#ifndef TRAFFIC_COLLECTOR_ARP_H
#define TRAFFIC_COLLECTOR_ARP_H

#include "layer.h"

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

    ARP_header *header;

    /**
     * Create ARP Layer of parsed header fields.
     *
     * @param bytes Raw bytes of ARP header.
     */
    explicit ARP(const uint8_t *bytes);
};


#endif //TRAFFIC_COLLECTOR_ARP_H
