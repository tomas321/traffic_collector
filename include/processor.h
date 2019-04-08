//
// Created by tomas on 16/03/19.
//

#ifndef TRAFFIC_COLLECTOR_PROCESSOR_H
#define TRAFFIC_COLLECTOR_PROCESSOR_H

#include <iostream>
#include <map>

#include "parsing/layer.h"
#include "databese_controller.h"
#include "json.h"
#include "harmonization.h"

using namespace std;

class Processor {
private:
    DatabaseController *db_control;
    Harmonization *harmonozation;

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
    string jsonize_packet(const uint8_t *raw_packet, const uint32_t packet_len, const uint32_t caplen, string timestamp,
                          const uint8_t *handle_mac_addr);

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
     * @param harmonization Data harmonization object.
     */
    explicit Processor(DatabaseController *db_control, Harmonization *harmonization);

    /**
     * Packet processing.
     *
     * @param packet_len Total packet length (part of pcap metadata).
     * @param caplen Snapshot length (part of pcap metadata).
     * @param timestamp Time the packet was capture (part of pcap metadata).
     * @param packet Packet bytes.
     * @param handle_mac_addr MAC address of capture device.
     */
    void process_packet(const uint32_t packet_len, const uint32_t caplen, const struct timeval &timestamp,
                        const uint8_t *packet, const uint8_t *handle_mac_addr);
};


#endif //TRAFFIC_COLLECTOR_PROCESSOR_H
