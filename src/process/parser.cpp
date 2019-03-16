//
// Created by tomas on 16/03/19.
//

#include "parser.h"

const map<pcpp::ProtocolType, const string> Parser::supported_layers = {
        {pcpp::Ethernet, "ethernet"},
        {pcpp::ARP, "arp"},
        {pcpp::IP, "ip"},
        {pcpp::IPv4, "ipv4"},
        {pcpp::IPv6, "ipv6"},
        {pcpp::ICMP, "icmp"},
        {pcpp::UDP, "udp"},
        {pcpp::TCP, "tcp"},
        {pcpp::SSL, "ssl"},
        {pcpp::DHCP, "dhcp"},
        {pcpp::HTTP, "http"},
        {pcpp::HTTPRequest, "http_request"},
        {pcpp::HTTPResponse, "http_response"},
        {pcpp::DNS, "dns"},
        {pcpp::SDP, "sdp"},
        {pcpp::PPPoE, "pppoe"},
        {pcpp::VLAN, "vlan"},
        {pcpp::PPP_PPTP, "ppp"},
        {pcpp::SIP, "sip"},
        {pcpp::SIPResponse, "sip_response"},
        {pcpp::SIPRequest, "sip_request"},
        {pcpp::GRE, "gre"},
        {pcpp::MPLS, "mpls"},
        {pcpp::IGMP, "igmp"},
        {pcpp::IGMPv1, "igmpv1"},
        {pcpp::IGMPv2, "igmpv2"},
        {pcpp::IGMPv3, "igmpv3"}
};

string Parser::timeval_to_string(const struct timeval &ts) {
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];

    nowtime = ts.tv_sec;
    nowtm = localtime(&nowtime);
    strftime(tmbuf, sizeof(tmbuf), "%Y-%m-%d %H:%M:%S", nowtm);
    snprintf(buf, sizeof(buf), "%s.%06ld", tmbuf, ts.tv_usec);

    return string(buf);
}

void Parser::print_packet(const string &timestamp, pcpp::RawPacket *raw_packet) {
    pcpp::Packet parsed_packet(raw_packet);

    cout << "[" << timestamp << "]" << endl;
    for (pcpp::Layer *layer = parsed_packet.getFirstLayer(); layer != NULL; layer = layer->getNextLayer()) {
        cout << "\t" << layer->toString() << endl;
    }
}

void Parser::process_packet(const uint32_t packet_len, const uint32_t caplen, const struct timeval &timestamp, const unsigned char *packet, const int datalink) {
    auto *raw_packet = new pcpp::RawPacket(packet, caplen, timestamp, true, static_cast<pcpp::LinkLayerType>(datalink));

    print_packet(timeval_to_string(timestamp), raw_packet);
}