//
// Created by tomas on 31/03/19.
//

#include "parsing/rdp.h"
#include "parsing/operations.h"

RDP::RDP(const uint8_t *bytes) : Layer() {
    header = (RDP_header *) malloc(sizeof(RDP_header));

    header->flags = static_cast<uint8_t>( (bytes[0] & 0xfc) >> 2);
    header->version = static_cast<uint8_t>(bytes[0] & 0x03);
    header->hdr_length = bytes[1];
    header->src_port = bytes[2];
    header->dst_port = bytes[3];
    header->length = Bytes::merge_bytes(bytes[4], bytes[5]);
    header->seq = Bytes::merge_bytes(bytes[6], bytes[7], bytes[8], bytes[9]);
    header->ack = Bytes::merge_bytes(bytes[10], bytes[11], bytes[12], bytes[13]);
    header->checksum = Bytes::merge_bytes(bytes[14], bytes[15], bytes[16], bytes[17]);

    layer_type = Layers::RDP;
}
