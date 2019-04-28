#ifndef TRAFFIC_COLLECTOR_PACKET_H
#define TRAFFIC_COLLECTOR_PACKET_H

#include <cstdint>

#include "layer.h"

class Parsed_packet {
private:
    /**
     * Double linked list of layers.
     */
    Layer **layers;

    /**
     * Number of layers of the parsed packet.
     */
    int num_of_layers;

    /**
     * Parse raw packet header bytes to given layer type.
     *
     * @param layer Layer to be filled.
     * @param bytes Raw packet bytes.
     * @param type Layer type to be parsed. and is set to the next Layer type.
     * @param offset Offset from the first byte of `bytes`.
     * @return 0 if parsing will continue for another layer, else return 1.
     */
    int parse_header(Layer **layer, const uint8_t *bytes, Layers::Type *type, int *offset);

public:
    /**
     * Create parsed Packet object with layers' double linked list.
     *
     * @param bytes Raw packet bytes from pcap capture.
     */
    explicit Parsed_packet(const uint8_t *bytes);

    Layer **get_layers();

    int get_num_of_layers();
};

#endif //TRAFFIC_COLLECTOR_PACKET_H
