#ifndef TRAFFIC_COLLECTOR_IPV6_H
#define TRAFFIC_COLLECTOR_IPV6_H

#include "layer.h"

class IPv6 : public Layer {
public:
    struct IPv6_header {
        uint8_t version; // 4 bits
        uint8_t traffic_class;
        uint32_t flow_label; // 20 bits
        uint16_t payload_len;
        uint8_t next_hdr;
        uint8_t hop_limit;
        uint16_t src_ip[8];
        uint16_t dst_ip[8];
    };

    IPv6_header *header;

    /**
     * Create IPv6 Layer of parsed header fields.
     *
     * @param bytes Raw bytes of IPv6 header.
     */
    explicit IPv6(const uint8_t *bytes);
};


#endif //TRAFFIC_COLLECTOR_IPV6_H
