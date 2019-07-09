#ifndef TRAFFIC_COLLECTOR_ICMP_H
#define TRAFFIC_COLLECTOR_ICMP_H

#include "layer.h"

class ICMP : public Layer {
public:
    struct ICMP_header {
        uint8_t type;
        uint8_t code;
        uint16_t hdr_checksum;
        // and further fields for all code/type combinations
    };

    ICMP_header *header;

    /**
     * Create ICMP Layer of parsed header fields.
     *
     * @param bytes Raw bytes of ICMP header.
     */
    explicit ICMP(const uint8_t *bytes);
};


#endif //TRAFFIC_COLLECTOR_ICMP_H
