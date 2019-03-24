//
// Created by tomas on 23/03/19.
//

#include "parsing/icmp.h"
#include "parsing/operations.h"

ICMP::ICMP(const uint8_t *bytes) : Layer() {
    header = (ICMP_header *) malloc(sizeof(ICMP_header));

    header->type = bytes[0];
    header->code = bytes[1];
    header->hdr_checksum = Bytes::merge_bytes(bytes[2], bytes[3]);
    // TODO: add other fields

    layer_type = Layers::ICMP;
}