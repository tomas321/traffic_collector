//
// Created by tomas on 23/03/19.
//

#include "parsing/operations.h"
#include "parsing/ethernet.h"

Ethernet::Ethernet(const uint8_t *bytes) : Layer() {
    header = (Ethernet_header *) malloc(sizeof(Ethernet_header));

    memcpy(header->dst_mac, bytes, 6);
    memcpy(header->src_mac, (bytes + 6), 6);
    header->ethertype = Bytes::merge_bytes(bytes[12], bytes[13]);

    layer_type = Layers::Ethernet;
}