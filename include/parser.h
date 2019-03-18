//
// Created by tomas on 16/03/19.
//

#ifndef TRAFFIC_COLLECTOR_PARSER_H
#define TRAFFIC_COLLECTOR_PARSER_H

#include <stdint-gcc.h>
#include <iostream>
#include <Packet.h>
#include <RawPacket.h>
#include <map>

using namespace std;

class Parser {
private:
    /**
     * Convert packet timestamp to formatted string.
     *
     * @param ts Timeval structure.
     * @return Timestamp as string.
     */
    static string timeval_to_string(const struct timeval &ts);

    /**
     * Sample packet print function.
     *
     * @param timestamp String timestamp.
     * @param packet Raw packet object.
     */
    static void print_packet(const string &timestamp, pcpp::RawPacket *raw_packet);

public:
    static const map<pcpp::ProtocolType , const string> supported_layers;

    /**
     * Packet processing.
     *
     * @param meta Packet metadata including length and capture timestamp.
     * @param packet Packet bytes.
     */
    static void process_packet(uint32_t packet_len, uint32_t caplen, const struct timeval &timestamp, const unsigned char *packet, int datalink);
};


#endif //TRAFFIC_COLLECTOR_PARSER_H
