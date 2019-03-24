//
// Created by tomas on 23/03/19.
//

#include "parsing/ipv6.h"
#include "parsing/operations.h"

IPv6::IPv6(const uint8_t *bytes) : Layer() {
    header = (IPv6_header *) malloc(sizeof(IPv6_header));

    header->version = static_cast<uint8_t>((bytes[0] & 0xf0) >> 4);
    header->traffic_class = static_cast<uint8_t>( ((bytes[0] & 0x0f) << 4) | ((bytes[1] & 0xf0) >> 4) );
    header->flow_label = Bytes::merge_bytes(0x00, static_cast<const uint8_t>(bytes[1] & 0x0f), bytes[2], bytes[3]);
    header->payload_len = Bytes::merge_bytes(bytes[4], bytes[5]);
    header->next_hdr = bytes[6];
    header->hop_limit = bytes[7];
    int j = 0;
    for (int i = 8; i < 24; i += 2)
        header->src_ip[j++] = Bytes::merge_bytes(bytes[i], bytes[i + 1]);
    j = 0;
    for (int i = 24; i < 40; i += 2)
        header->dst_ip[j++] = Bytes::merge_bytes(bytes[i], bytes[i + 1]);

    layer_type = Layers::IPv6;
}