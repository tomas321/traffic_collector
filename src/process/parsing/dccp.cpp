//
// Created by tomas on 01/04/19.
//

#include "parsing/dccp.h"
#include "parsing/operations.h"

DCCP::DCCP(const uint8_t *bytes) : Layer() {
    header = (DCCP_header *) malloc(sizeof(DCCP_header));

    header->src_port = Bytes::merge_bytes(bytes[0], bytes[1]);
    header->dst_port = Bytes::merge_bytes(bytes[2], bytes[3]);
    header->offset = bytes[4];
    header->ccval = (bytes[5] & 0xf0) >> 4;
    header->checksum_cov = bytes[5] & 0x0f;
    header->checksum = Bytes::merge_bytes(bytes[6], bytes[7]);
    header->type = (bytes[8] & 0x1e) >> 1; // mask = 0b00011110
    header->x = bytes[8] & 0x01;

    if (header->x == 0x01) {
        // 8 reserved bits (bytes[9]) + 48 bit seq number
        auto high = static_cast<uint64_t>(Bytes::merge_bytes(bytes[10], bytes[11]));
        auto low = static_cast<uint64_t>(Bytes::merge_bytes(bytes[12], bytes[13], bytes[14], bytes[15]));
        header->seq = ((high & 0x000000000000ffff) << 32) + low;
    } else {
        // 24 bit seq number
        auto high = static_cast<uint64_t>(bytes[9]);
        auto low = static_cast<uint64_t>(Bytes::merge_bytes(bytes[10], bytes[11]));
        header->seq = ((high & 0x00000000000000ff) << 16) + low;
    }

    layer_type = Layers::DCCP;
}