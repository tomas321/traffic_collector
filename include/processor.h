//
// Created by tomas on 16/03/19.
//

#ifndef TRAFFIC_COLLECTOR_PROCESSOR_H
#define TRAFFIC_COLLECTOR_PROCESSOR_H

#include <stdint-gcc.h>
#include <iostream>
#include <map>

#include "parsing/layer.h"
#include "databese_controller.h"
#include "json.h"

using namespace std;

class Processor {
private:
    DatabaseController *db_control;

    /**
     * Convert packet timestamp to formatted string.
     *
     * @param ts Timeval structure.
     * @return Timestamp as string.
     */
    string timeval_to_string(const struct timeval &ts);

    /**
     * Sample packet print function.
     *
     * @param timestamp String timestamp.
     * @param packet Raw packet object.
     */
    void print_packet_layers(const string &timestamp, uint8_t *raw_packet);

    /**
     * Parse packet to json string.
     *
     * @param packet Address of parsed packet object.
     * @return Json string
     */
    string jsonize_packet(const uint8_t *raw_packet, uint32_t packet_len, string timestamp);

    /**
     * Packet layer to Json object.
     *
     * @param json Json object to be appended.
     * @param packet_layer Pakcet layer data.
     * @return 0 on success, else 1;
     */
    int layer_to_json(Json *json, Layer *packet_layer);

public:
    /**
     * Initialize Processor with active DB connector (TCP stream).
     *
     * @param db_control Database controller.
     */
    explicit Processor(DatabaseController *db_control);

    /**
     * Packet processing.
     *
     * @param meta Packet metadata including length and capture timestamp.
     * @param packet Packet bytes.
     */
    void process_packet(uint32_t packet_len, uint32_t caplen, const struct timeval &timestamp, const uint8_t *packet, int datalink);
};


#endif //TRAFFIC_COLLECTOR_PROCESSOR_H