//
// Created by tomas on 31/03/19.
//

#ifndef TRAFFIC_COLLECTOR_ICMPV6_H
#define TRAFFIC_COLLECTOR_ICMPV6_H


#include "layer.h"

class ICMPv6 : public Layer {
public:
    struct ICMPv6_header {
        uint8_t type;
        uint8_t code;
        uint16_t hdr_checksum;
        // and further fields for all code/type combinations
    };

    ICMPv6_header *header;

    /**
     * Create ICMPv6 Layer of parsed header fields.
     *
     * @param bytes Raw bytes of ICMPv6 header.
     */
    explicit ICMPv6(const uint8_t *bytes);
};


#endif //TRAFFIC_COLLECTOR_ICMPV6_H
