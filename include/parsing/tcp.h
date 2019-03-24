//
// Created by tomas on 23/03/19.
//

#ifndef TRAFFIC_COLLECTOR_TCP_H
#define TRAFFIC_COLLECTOR_TCP_H

#include "layer.h"

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

    TCP_header *header;

    /**
     * Create TCP Layer of parsed header fields.
     *
     * @param bytes Raw bytes of TCP header.
     */
    explicit TCP(const uint8_t *bytes);
};



#endif //TRAFFIC_COLLECTOR_TCP_H
