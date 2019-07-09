#ifndef TRAFFIC_COLLECTOR_RDP_H
#define TRAFFIC_COLLECTOR_RDP_H


#include "layer.h"

class RDP : public Layer {
public:
    struct RDP_header {
        uint8_t flags; // 6 bits
        uint8_t version; // 2 bits
        uint8_t hdr_length; // 16bit words
        uint8_t src_port;
        uint8_t dst_port;
        uint16_t length; // in bytes excluding header
        uint32_t seq;
        uint32_t ack;
        uint32_t checksum;
        uint8_t *data;
    };

    RDP_header *header;

    /**
     * Create RDP Layer of parsed header fields.
     *
     * @param bytes Raw bytes of RDP header.
     */
    explicit RDP(const uint8_t *bytes);
};

#endif //TRAFFIC_COLLECTOR_RDP_H
