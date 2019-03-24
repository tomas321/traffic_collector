//
// Created by tomas on 23/03/19.
//

#include "parsing/tcp.h"
#include "parsing/operations.h"

TCP::TCP(const uint8_t *bytes) : Layer() {
    header = (TCP_header *) malloc(sizeof(TCP_header));

    header->src_port = Bytes::merge_bytes(bytes[0], bytes[1]);
    header->dst_port = Bytes::merge_bytes(bytes[2], bytes[3]);
    header->seq = Bytes::merge_bytes(bytes[4], bytes[5], bytes[6], bytes[7]);
    header->ack = Bytes::merge_bytes(bytes[8], bytes[9], bytes[10], bytes[11]);
    header->offset = static_cast<uint8_t>( (bytes[12] & 0xf0) >> 4 );
    header->control_bits = static_cast<uint8_t>(bytes[13] & 0x3f);
    header->window = Bytes::merge_bytes(bytes[14], bytes[15]);
    header->checksum = Bytes::merge_bytes(bytes[16], bytes[17]);
    header->urgent_ptr = Bytes::merge_bytes(bytes[18], bytes[19]);

    layer_type = Layers::TCP;
}
