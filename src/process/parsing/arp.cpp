//
// Created by tomas on 23/03/19.
//

#include "parsing/operations.h"
#include "parsing/arp.h"

ARP::ARP(const uint8_t *bytes) : Layer() {
    header = (ARP_header *) malloc(sizeof(ARP_header));

    header->hw_type = Bytes::merge_bytes(bytes[0], bytes[1]);
    header->ip_type = Bytes::merge_bytes(bytes[2], bytes[3]);
    header->hwaddr_len = bytes[4];
    header->l3addr_len = bytes[5];
    header->opcode = Bytes::merge_bytes(bytes[6], bytes[7]);
    memcpy(header->sender_mac, (bytes + 8), 6);
    header->sender_ip = Bytes::merge_bytes(bytes[14], bytes[15], bytes[16], bytes[17]);
    memcpy(header->target_mac, (bytes + 18), 6);
    header->target_ip = Bytes::merge_bytes(bytes[24], bytes[25], bytes[26], bytes[27]);

    layer_type = Layers::ARP;
}