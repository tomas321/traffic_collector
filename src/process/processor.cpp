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
#include "parsing/rdp.h"
#include "parsing/dccp.h"
#include "parsing/icmp.h"
#include "parsing/icmpv6.h"
#include "parsing/operations.h"
#include "logging.h"
#include "exceptions.h"

#include <netdb.h>


Processor::Processor(DatabaseController *db_control, Harmonization *harmonization) : db_control(db_control),
                                                                                     harmonozation(harmonization) {}

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

void Processor::process_packet(const uint32_t packet_len, const uint32_t caplen, const struct timeval &timestamp,
                               const uint8_t *packet, const uint8_t *handle_mac_addr) {
    int bytes;

    string json_packet_str = jsonize_packet(packet, packet_len, caplen, timeval_to_string(timestamp),
                                            handle_mac_addr);

    if (db_control) bytes = db_control->send(json_packet_str.c_str());
    else throw SocketError("DB controller is null", critical);

    Logging::log(debug, "sent " + to_string(bytes) + " bytes to " + db_control->get_host());
}

string Processor::jsonize_packet(const uint8_t *raw_packet, const uint32_t packet_len, const uint32_t caplen,
                                 string timestamp,
                                 const uint8_t *handle_mac_addr) {
    Json json;
    // TODO: handle errors.
    Parsed_packet packet(raw_packet);
    Layer **layers = packet.get_layers();
    uint8_t *src_mac = nullptr, *dst_mac = nullptr;

    // Loop layers using the next pointer of the double linked list.
    for (Layer *current = *layers; current != nullptr; current = current->get_next()) {
        if (current->get_layer_type() == Layers::Ethernet) {
            src_mac = reinterpret_cast<Ethernet *>(current)->header->src_mac;
            dst_mac = reinterpret_cast<Ethernet *>(current)->header->dst_mac;
        }
        layer_to_json(&json, current);
    }

    // Add timestamp as a time.source key and pcap capture meta data
    json.start_object("time");
    json.add<string>("source", std::move(timestamp));
    json.end_object();
    json.start_object("pcap");
    json.start_object("length");
    json.add<uint32_t>("capture", packet_len);
    json.add<uint32_t>("snapshot", caplen);
    json.end_object();
    if (src_mac && dst_mac) {
        if (string(MACAddress::to_string(src_mac)) == string(MACAddress::to_string(handle_mac_addr)))
            json.add<string>("direction", "tx");
        else if (string(MACAddress::to_string(dst_mac)) == string(MACAddress::to_string(handle_mac_addr)))
            json.add<string>("direction", "rx");
    }
    json.end_object();

    return json.stringify() + "\n"; // new-line separates json strings
}

// TODO: redo as templated function (change as last) !!!
int Processor::layer_to_json(Json *json, Layer *packet_layer) {
    Ethernet *eth;
    IPv4 *ipv4;
    TCP *tcp;
    UDP *udp;
    ARP *arp;
    IPv6 *ipv6;
    ICMP *icmp;
    ICMPv6 *icmp6;
    RDP *rdp;
    DCCP *dccp;

    string geoip_src, geoip_dst, protoname;

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
            protoname = string(getprotobynumber(ipv4->header->protocol)->p_name);
            if (! protoname.empty()) cout << "protocol in IP: " << protoname << endl;
            else protoname = Layers::layer_string(static_cast<Layers::Type>(ipv4->header->protocol));
            json->add<string>("protocol", protoname);
            json->add<uint16_t>("checksum", ipv4->header->hdr_checksum);
            json->add<string>("source", IPAddress::to_string(ipv4->header->src_ip));
            json->add<string>("destination", IPAddress::to_string(ipv4->header->dst_ip));
            json->end_object();

            try {
                if (IPAddress::is_public(ipv4->header->src_ip))
                    geoip_src = harmonozation->geoip(IPAddress::to_string(ipv4->header->src_ip));
            } catch (const std::logic_error &e) {
                Logging::log(warning,
                             "no geoip data for ip: " + string(IPAddress::to_string(ipv4->header->src_ip)));
            }
            try {
                if (IPAddress::is_public(ipv4->header->dst_ip))
                    geoip_dst = harmonozation->geoip(IPAddress::to_string(ipv4->header->dst_ip));
            } catch (const std::logic_error &e) {
                Logging::log(warning,
                             "no geoip data for ip: " + string(IPAddress::to_string(ipv4->header->dst_ip)));
            }

            if ((IPAddress::is_public(ipv4->header->src_ip) && !geoip_src.empty()) ||
                (IPAddress::is_public(ipv4->header->dst_ip) && !geoip_dst.empty())) {
                json->start_object("geoip");
                if (IPAddress::is_public(ipv4->header->src_ip) && !geoip_src.empty())
                    json->add<string>("source", geoip_src);
                if (IPAddress::is_public(ipv4->header->dst_ip) && !geoip_dst.empty())
                    json->add<string>("destination", geoip_dst);
                json->end_object();
            }

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
        case Layers::ICMPv6:
            // TODO: add type and code mapping to names
            // ICMPv6 is mapped to the same ES field mappings (imcp)
            icmp6 = reinterpret_cast<ICMPv6 *>(packet_layer);
            json->start_object("icmp");
            json->add<uint8_t>("type", icmp6->header->type);
            json->add<uint8_t>("code", icmp6->header->code);
            json->add<uint16_t>("checksum", icmp6->header->hdr_checksum);
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
            protoname = string(getprotobynumber(ipv6->header->next_hdr)->p_name);
            if (! protoname.empty()) cout << "protocol in IPv6: " << protoname << endl;
            else protoname = Layers::layer_string(static_cast<Layers::Type>(ipv6->header->next_hdr));
            json->add<string>("next_header", protoname);
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
        case Layers::RDP:
            rdp = reinterpret_cast<RDP *>(packet_layer);
            json->start_object("rdp");
            json->add<uint8_t>("flags", rdp->header->flags);
            json->add<uint8_t>("version", rdp->header->version);
            json->add<uint8_t>("header_length", rdp->header->hdr_length);
            json->add<uint8_t>("source_port", rdp->header->src_port);
            json->add<uint8_t>("destination_port", rdp->header->dst_port);
            json->add<uint16_t>("length", rdp->header->length);
            json->add<uint32_t>("seq", rdp->header->seq);
            json->add<uint32_t>("ack", rdp->header->ack);
            json->add<uint32_t>("checksum", rdp->header->checksum);
            json->end_object();
            break;
        case Layers::DCCP:
            dccp = reinterpret_cast<DCCP *>(packet_layer);
            json->start_object("dccp");
            json->add<uint16_t>("source_port", dccp->header->src_port);
            json->add<uint16_t>("destination_port", dccp->header->dst_port);
            json->add<uint8_t>("offset", dccp->header->offset);
            json->add<uint8_t>("ccval", dccp->header->ccval);
            json->add<uint8_t>("checksum_coverage", dccp->header->checksum_cov);
            json->add<uint16_t>("checksum", dccp->header->checksum);
            json->add<uint8_t>("type", dccp->header->type);
            json->add<bool>("extended_seq", bool(dccp->header->x));
            json->add<uint64_t>("seq", dccp->header->seq);
            json->end_object();
            break;
        default:
            return 1;
    }
    return 0;
}
