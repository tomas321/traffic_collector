//
// Created by tomas on 23/03/19.
//

#include <iostream>

#include "parsing/icmp.h"
#include "parsing/tcp.h"
#include "parsing/udp.h"
#include "parsing/arp.h"
#include "parsing/ipv4.h"
#include "parsing/ipv6.h"
#include "parsing/ethernet.h"
#include "parsing/operations.h"
#include "parsing/packet.h"

Parsed_packet::Parsed_packet(const uint8_t *bytes) {
    int offset = 0, done = 0;

    num_of_layers = 0;
    Layers::Type layer_type = Layers::Ethernet;
    layers = (Layer **) malloc(sizeof(Layer *));

    do {
        layers = (Layer **) realloc(layers, sizeof(Layer *) * (num_of_layers + 1));
        layers[num_of_layers] = (Layer *) malloc(sizeof(Layer));

        done = parse_header(&layers[num_of_layers], bytes, &layer_type, &offset);

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
    switch (*type) {
        case Layers::Ethernet:
            *layer = new Ethernet((bytes + *offset));
            increase = Layers::header_lengths.at(*type);
            *type = static_cast<Layers::Type>(reinterpret_cast<Ethernet *>(*layer)->header->ethertype);
            break;
        case Layers::IPv4:
            *layer = new IPv4((bytes + *offset));
            increase = Layers::header_lengths.at(*type);
            *type = static_cast<Layers::Type>(reinterpret_cast<IPv4 *>(*layer)->header->protocol);
            break;
        case Layers::IPv6:
            *layer = new IPv6((bytes + *offset));
            increase = Layers::header_lengths.at(*type);
            *type = static_cast<Layers::Type>(reinterpret_cast<IPv6 *>(*layer)->header->next_hdr);
            break;
        case Layers::ARP:
            *layer = new ARP((bytes + *offset));
//            increase = Layers::header_lengths.at(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::ICMP:
            *layer = new ICMP((bytes + *offset));
//            increase = Layers::header_lengths.at(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::UDP:
            *layer = new UDP((bytes + *offset));
//            increase = Layers::header_lengths.at(*type);
            *type = Layers::NONE;
            return 1;
        case Layers::TCP:
            *layer = new TCP((bytes + *offset));
//            increase = Layers::header_lengths.at(*type);
            *type = Layers::NONE;
            return 1;
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

int Parsed_packet::get_num_of_layers() {
    return num_of_layers;
}
