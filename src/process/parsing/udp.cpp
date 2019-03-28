//
// Created by tomas on 23/03/19.
//

#include "parsing/udp.h"
#include "parsing/operations.h"

UDP::UDP(const uint8_t *bytes) : Layer() {
    header = (UDP_header *) malloc(sizeof(UDP_header));

    header->src_port = Bytes::merge_bytes(bytes[0], bytes[1]);
    header->dst_port = Bytes::merge_bytes(bytes[2], bytes[3]);
    header->length = Bytes::merge_bytes(bytes[4], bytes[5]);
    header->checksum = Bytes::merge_bytes(bytes[6], bytes[7]);

//    header->data = (uint8_t *) malloc(static_cast<size_t>(header->length - 8));
//    memcpy(header->data, (bytes + 8), static_cast<size_t>(header->length - 8));

    layer_type = Layers::UDP;
}