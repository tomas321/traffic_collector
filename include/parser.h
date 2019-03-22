//
// Created by tomas on 16/03/19.
//

#ifndef TRAFFIC_COLLECTOR_PARSER_H
#define TRAFFIC_COLLECTOR_PARSER_H

#include <stdint-gcc.h>
#include <iostream>
#include <map>

#include "parsing/layers.h"
#include "parsing/operations.h"

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
    static void print_packet_layers(const string &timestamp, uint8_t *raw_packet);

    /**
     * Parse packet to json string.
     *
     * @param packet Address of parsed packet object.
     * @return Json string
     */
    static string jsonize_packet(uint8_t *raw_packet);

    /**
     * Stringify packet layer to string.
     *
     * @param type Packet protocol type.
     * @param packet_layer Pakcet layer data.
     * @return Json string.
     */
    static string parse_protocol(Packet::Type type, uint8_t *data);

public:

    /**
     * Packet processing.
     *
     * @param meta Packet metadata including length and capture timestamp.
     * @param packet Packet bytes.
     */
    static void process_packet(uint32_t packet_len, uint32_t caplen, const struct timeval &timestamp, uint8_t *packet, int datalink);
};


#endif //TRAFFIC_COLLECTOR_PARSER_H
