//
// Created by tomas on 16/03/19.
//

#include "processor.h"
#include "parsing/packet.h"
#include "parsing/ipv4.h"
#include "parsing/ipv6.h"
#include "parsing/ethernet.h"
#include "parsing/arp.h"
#include "parsing/udp.h"
#include "parsing/tcp.h"
#include "parsing/icmp.h"
#include "parsing/operations.h"
#include "logging.h"
#include "exceptions.h"


Processor::Processor(DatabaseController *db_control) : db_control(db_control) {}

string Processor::timeval_to_string(const struct timeval &ts) {
    struct tm *time_struct;
    char str_time[64], str_timestamp[64], time_zone[8];

    time_struct = localtime(&ts.tv_sec);
    strftime(str_time, sizeof(str_time), "%Y-%m-%dT%H:%M:%S", time_struct);
    strftime(time_zone, sizeof(time_zone), "%z", time_struct);
    snprintf(str_timestamp, sizeof(str_timestamp), "%s.%06ld%s", str_time, ts.tv_usec, time_zone);

    return string(str_timestamp);
}

void Processor::print_packet_layers(const string &timestamp, uint8_t *raw_packet) {
    cout << "[" << timestamp << "]" << endl;
}

void Processor::process_packet(const uint32_t packet_len, const uint32_t caplen, const struct timeval &timestamp, const uint8_t *packet, const int datalink) {
    int bytes;

    string json_packet_str = jsonize_packet(packet, packet_len, timeval_to_string(timestamp));

//    if (db_control) bytes = db_control->send(json_packet_str.c_str());
//    else throw SocketError("DB controller is null");
    auto db_c = new DatabaseController(12000, "elk.bp.local");

    bytes = db_c->send(json_packet_str.c_str());
    delete db_c;

    Logging::log(debug, "sent " + to_string(bytes) + " bytes to " + db_control->get_host());
}

string Processor::jsonize_packet(const uint8_t *raw_packet, uint32_t packet_len, string timestamp) {
    Json json;
    // TODO: handle errors.
    Parsed_packet packet(raw_packet);
    Layer **layers = packet.get_layers();

//    Logging::log(debug, "Parsed packet and retrieved layers");

    // Loop layers using the next pointer of the double linked list.
    for (Layer *current = *layers; current != nullptr; current = current->get_next())
        layer_to_json(&json, current);

    // Add timestamp as a final time.source key.
    json.start_object("time");
    json.add<string>("source", std::move(timestamp));
    json.end_object();

    return json.stringify();
}

int Processor::layer_to_json(Json *json, Layer *packet_layer) {
    Ethernet *eth;
    IPv4 *ipv4;
    TCP *tcp;
    UDP *udp;
    ARP *arp;
    IPv6 *ipv6;
    ICMP *icmp;

//    Logging::log(debug, "creating json string for " + Layers::layer_string(packet_layer->get_layer_type()));

    switch (packet_layer->get_layer_type()) {
        case Layers::Ethernet:
            eth = reinterpret_cast<Ethernet *>(packet_layer);
            json->start_object("ethernet");
            json->add<string>("source", MACAddress::to_string(eth->header->src_mac));
            json->add<string>("destination", MACAddress::to_string(eth->header->dst_mac));
            json->add<string>("ethertype", Layers::layer_string(static_cast<Layers::Type>(eth->header->ethertype)));
            json->end_object();
            break;
        case Layers::IPv4:
            ipv4 = reinterpret_cast<IPv4 *>(packet_layer);
            json->start_object("ip");
            json->add<uint8_t>("version", ipv4->header->version);
            json->add<uint8_t>("ihl", ipv4->header->ihl);
            json->add<uint8_t>("tos", ipv4->header->tos);
            json->add<uint16_t>("total_length", ipv4->header->total_length);
            json->add<uint16_t>("identification", ipv4->header->identification);
            json->add<uint8_t>("flags", ipv4->header->flags);
            json->add<uint16_t>("offset", ipv4->header->offset);
            json->add<uint8_t>("ttl", ipv4->header->ttl);
            json->add<string>("protocol", Layers::layer_string(static_cast<Layers::Type>(ipv4->header->protocol)));
            json->add<uint16_t>("checksum", ipv4->header->hdr_checksum);
            json->add<string>("source", IPAddress::to_string(ipv4->header->src_ip));
            json->add<string>("destination", IPAddress::to_string(ipv4->header->dst_ip));
            json->end_object();
            break;
        case Layers::ICMP:
            // TODO: add type and code mapping to names

            icmp = reinterpret_cast<ICMP *>(packet_layer);
            json->start_object("icmp");
            json->add<uint8_t>("type", icmp->header->type);
            json->add<uint8_t>("code", icmp->header->code);
            json->add<uint16_t>("checksum", icmp->header->hdr_checksum);
            json->end_object();
            break;
        case Layers::ARP:
            arp = reinterpret_cast<ARP *>(packet_layer);
            json->start_object("arp");
            json->start_object("hardware");
            json->add<uint16_t>("type", arp->header->hw_type);
            json->add<uint8_t>("length", arp->header->hwaddr_len);
            json->end_object();
            json->start_object("protocol");
            json->add<uint16_t>("type", arp->header->ip_type);
            json->add<uint8_t>("length", arp->header->l3addr_len);
            json->end_object();
            json->add<uint16_t>("opcode", arp->header->opcode);
            json->start_object("sender");
            json->add<string>("mac_address", MACAddress::to_string(arp->header->sender_mac));
            json->add<string>("protocol_addr", IPAddress::to_string(arp->header->sender_ip));
            json->end_object();
            json->start_object("target");
            json->add<string>("mac_address", MACAddress::to_string(arp->header->target_mac));
            json->add<string>("protocol_addr", IPAddress::to_string(arp->header->target_ip));
            json->end_object();
            json->end_object();
            break;
        case Layers::IPv6:
            ipv6 = reinterpret_cast<IPv6 *>(packet_layer);
            json->start_object("ipv6");
            json->add<uint8_t>("version", ipv6->header->version);
            json->add<uint8_t>("traffic_class", ipv6->header->traffic_class);
            json->add<uint32_t>("flow_label", ipv6->header->flow_label);
            json->add<uint16_t>("payload_length", ipv6->header->payload_len);
            // TODO: include string representation
            json->add<uint8_t>("next_header", ipv6->header->next_hdr);
            json->add<uint8_t>("ttl", ipv6->header->hop_limit); // or hop_limit
            json->add<string>("source", IPv6Address::to_string(ipv6->header->src_ip));
            json->add<string>("destination", IPv6Address::to_string(ipv6->header->dst_ip));
            json->end_object();
            break;
        case Layers::TCP:
            tcp = reinterpret_cast<TCP *>(packet_layer);
            json->start_object("tcp");
            json->add<uint16_t>("source_port", tcp->header->src_port);
            json->add<uint16_t>("destination_port", tcp->header->dst_port);
            json->add<uint32_t>("seq", tcp->header->seq);
            json->add<uint32_t>("ack", tcp->header->ack);
            json->add<uint8_t>("offset", tcp->header->offset);
            json->add<uint8_t>("control_bits", tcp->header->control_bits);
            json->add<uint16_t>("window", tcp->header->window);
            json->add<uint16_t>("checksum", tcp->header->checksum);
            json->add<uint16_t>("urgent_pointer", tcp->header->urgent_ptr);
            json->end_object();
            break;
        case Layers::UDP:
            udp = reinterpret_cast<UDP *>(packet_layer);
            json->start_object("udp");
            json->add<uint16_t>("source_port", udp->header->src_port);
            json->add<uint16_t>("destination_port", udp->header->dst_port);
            json->add<uint16_t>("length", udp->header->length);
            json->add<uint16_t>("checksum", udp->header->checksum);
            json->end_object();
            break;
        default:
            return 1;
    }
    return 0;
}
