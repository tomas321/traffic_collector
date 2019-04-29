#include "parsing/icmpv6.h"
#include "parsing/operations.h"

ICMPv6::ICMPv6(const uint8_t *bytes) : Layer() {
    header = (ICMPv6_header *) malloc(sizeof(ICMPv6_header));

    header->type = bytes[0];
    header->code = bytes[1];
    header->hdr_checksum = Bytes::merge_bytes(bytes[2], bytes[3]);
    // TODO: add other fields

    layer_type = Layers::ICMPv6;
}