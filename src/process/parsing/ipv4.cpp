//
// Created by tomas on 23/03/19.
//

#include "parsing/operations.h"
#include "parsing/ipv4.h"
#include "parsing/layer.h"

IPv4::IPv4(const uint8_t *bytes) : Layer() {
    header = (IPv4_header *) malloc(sizeof(IPv4_header));

    header->version = static_cast<uint8_t>( (bytes[0] & 0xf0) >> 4 );
    header->ihl = static_cast<uint8_t>(bytes[0] & 0x0f);
    header->tos = bytes[1];
    header->total_length = Bytes::merge_bytes(bytes[2], bytes[3]);
    header->identification = Bytes::merge_bytes(bytes[4], bytes[5]);
    header->flags = static_cast<uint8_t>( (bytes[6] >> 5) & 0x07 );
    header->offset = static_cast<uint16_t>( ((bytes[6] & 0x001f) << 5) | bytes[7] );
    header->ttl = bytes[8];
    header->protocol = bytes[9];
    header->hdr_checksum = Bytes::merge_bytes(bytes[10], bytes[11]);
    header->src_ip = Bytes::merge_bytes(bytes[12], bytes[13], bytes[14], bytes[15]);
    header->dst_ip = Bytes::merge_bytes(bytes[16], bytes[17], bytes[18], bytes[19]);

    layer_type = Layers::IPv4;
}