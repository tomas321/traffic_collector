//
// Created by tomas on 23/03/19.
//

#ifndef TRAFFIC_COLLECTOR_UDP_H
#define TRAFFIC_COLLECTOR_UDP_H

#include "layer.h"

class UDP : public Layer {
public:
    struct UDP_header {
        uint16_t src_port;
        uint16_t dst_port;
        uint16_t length;
        uint16_t checksum;
        uint8_t *data;
    };

    UDP_header *header;

    /**
     * Create UDP Layer of parsed header fields.
     *
     * @param bytes Raw bytes of UDP header.
     */
    explicit UDP(const uint8_t *bytes);
};


#endif //TRAFFIC_COLLECTOR_UDP_H
