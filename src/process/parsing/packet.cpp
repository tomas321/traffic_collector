#include <iostream>
#include <netdb.h>

#include "parsing/icmp.h"
#include "parsing/tcp.h"
#include "parsing/udp.h"
#include "parsing/rdp.h"
#include "parsing/arp.h"
#include "parsing/ipv4.h"
#include "parsing/ipv6.h"
#include "parsing/icmpv6.h"
#include "parsing/ethernet.h"
#include "parsing/operations.h"
#include "parsing/packet.h"

#include "logging.h"

Parsed_packet::Parsed_packet(const uint8_t *bytes) {
    int offset = 0, done = 0;
    string protoname;

    num_of_layers = 0;
    Layers::Type layer_type = Layers::Ethernet;
    layers = (Layer **) malloc(sizeof(Layer *));

    do {
        layers = (Layer **) realloc(layers, sizeof(Layer *) * (num_of_layers + 1));
        layers[num_of_layers] = (Layer *) malloc(sizeof(Layer));

        done = parse_header(&layers[num_of_layers], bytes, &layer_type, &offset);
        protoname += "," + resolve_protocol_number(layer_type);

        if (Layers::layer_string(layer_type) == Layers::layer_string(Layers::Unknown)) {
            Logging::log(warning, "parsing unknown layer type and finishing parsing. unknown: " + protoname);
            done = 1;
        }

        if (num_of_layers == 0) {
            layers[num_of_layers]->set_previous(nullptr);
        } else {
            layers[num_of_layers]->set_previous(layers[num_of_layers - 1]);
            layers[num_of_layers - 1]->set_next(layers[num_of_layers]);
        }
        layers[num_of_layers]->set_next(nullptr);
        ++num_of_layers;
    } while (! done);
}

int Parsed_packet::parse_header(Layer **layer, const uint8_t *bytes, Layers::Type *type, int *offset) {
    int increase;

//    Logging::log(debug, "parsing packet layer header for " + Layers::layer_string(*type));

    switch (*type) {
        case Layers::Ethernet:
            *layer = new Ethernet((bytes + *offset));
            increase = Layers::header_lengths(*type);
            *type = static_cast<Layers::Type>(reinterpret_cast<Ethernet *>(*layer)->header->ethertype);
            break;
        case Layers::IPv4:
            *layer = new IPv4((bytes + *offset));
            increase = static_cast<int>(reinterpret_cast<IPv4 *>(*layer)->header->ihl) * 4; // increase the offset by IHL * 4 bytes
            *type = static_cast<Layers::Type>(reinterpret_cast<IPv4 *>(*layer)->header->protocol);
            break;
        case Layers::IPv6:
            *layer = new IPv6((bytes + *offset));
            increase = Layers::header_lengths(*type);
            *type = static_cast<Layers::Type>(reinterpret_cast<IPv6 *>(*layer)->header->next_hdr);
            break;
        case Layers::ARP:
            *layer = new ARP((bytes + *offset));
//            increase = Layers::header_lengths(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::ICMP:
            *layer = new ICMP((bytes + *offset));
//            increase = Layers::header_lengths(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::ICMPv6:
            *layer = new ICMPv6((bytes + *offset));
//            increase = Layers::header_lengths(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::UDP:
            *layer = new UDP((bytes + *offset));
//            increase = Layers::header_lengths(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::TCP:
            *layer = new TCP((bytes + *offset));
//            increase = Layers::header_lengths(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::RDP:
            *layer = new RDP((bytes + *offset));
            *type = Layers::NONE;
        default:
            *layer = nullptr;
            return 1;
    }

    *offset += increase;
    return 0;
}

Layer **Parsed_packet::get_layers() {
    return layers;
}

string Parsed_packet::resolve_protocol_number(int proto_number) {
    string protoname;
    struct protoent *protocol;

    if ((protocol = getprotobynumber(proto_number))) {
        protoname = string(protocol->p_name);
        return protoname;
    }
    protoname = to_string(proto_number); // if unsuccessful save the protocol number
    return protoname;
}

int Parsed_packet::get_num_of_layers() {
    return num_of_layers;
}
