//
// Created by tomas on 01/04/19.
//

#ifndef TRAFFIC_COLLECTOR_DCCP_H
#define TRAFFIC_COLLECTOR_DCCP_H


#include "layer.h"

class DCCP : public Layer {
public:
    // TODO: add fields to Elastic template
    struct DCCP_header {
        uint16_t src_port;
        uint16_t dst_port;
        uint8_t offset;
        uint8_t ccval; // 4 bits
        uint8_t checksum_cov; // 4 bits
        uint16_t checksum;
        // 3 ignored (reserved) bits
        uint8_t type; // 4 bits
        uint8_t x; // 1 bit which decides the size of seq number
        uint64_t seq; // 8 reserved bits + 48 bit seq number IF x = 1 ELSE 24 bit seq number
    };

    DCCP_header *header;

    /**
     * Create DCCP Layer of parsed header fields.
     *
     * @param bytes Raw bytes of DCCP header.
     */
    explicit DCCP(const uint8_t *bytes);
};


#endif //TRAFFIC_COLLECTOR_DCCP_H
