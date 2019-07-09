#ifndef TRAFFIC_COLLECTOR_IPV4_H
#define TRAFFIC_COLLECTOR_IPV4_H

#include "layer.h"

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

    IPv4_header *header;

    /**
     * Create IPv4 Layer of parsed header fields.
     *
     * @param bytes Raw bytes of IPv4 header.
     */
    explicit IPv4(const uint8_t *bytes);
};


#endif //TRAFFIC_COLLECTOR_IPV4_H
