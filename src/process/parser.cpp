//
// Created by tomas on 16/03/19.
//

#include "parser.h"
#include "json.h"
#include "parsing/packet.h"
#include "parsing/ipv4.h"
#include "parsing/operations.h"

string Parser::timeval_to_string(const struct timeval &ts) {
    struct tm *time_struct;
    char str_time[64], str_timestamp[64];

    time_struct = localtime(&ts.tv_sec);
    strftime(str_time, sizeof(str_time), "%Y-%m-%dT%H:%M:%S", time_struct);
    snprintf(str_timestamp, sizeof(str_timestamp), "%s.%06ld", str_time, ts.tv_usec);

    return string(str_timestamp);
}

void Parser::print_packet_layers(const string &timestamp, uint8_t *raw_packet) {
    cout << "[" << timestamp << "]" << endl;
    // TODO: loop all layers ?? and print
}

void Parser::process_packet(const uint32_t packet_len, const uint32_t caplen, const struct timeval &timestamp, const uint8_t *packet, const int datalink) {
    // TODO: rethink jsonizing API and all available data
    jsonize_packet(packet, packet_len, timeval_to_string(timestamp));
}

string Parser::jsonize_packet(const uint8_t *raw_packet, uint32_t packet_len, string timestamp) {
    string json_packet = "json_packet";

    Parsed_packet packet(raw_packet);
    Layer **layers = packet.get_layers();

    for (Layer *current = *layers; current != nullptr; current = current->get_next()) {
//        cout << Layers::layer_string.at(current->layer_type) << endl;
//        if (current->layer_type == Layers::IPv4) {
//            cout << IPAddress::to_string(reinterpret_cast<IPv4 *>(current)->header->src_ip) << endl;
//        }
        // TODO: properly merge json strings from each layer
        string s = layer_to_json(current);
        cout << s << endl;
    }

    return json_packet;
}

string Parser::layer_to_json(Layer *packet_layer) {
    Json json;

    switch (packet_layer->get_layer_type()) {
        case Layers::Ethernet:
            json.start_object("ethernet");
            // TODO: add all ethernet header data
            json.end_object();
            return json.stringify();
        case Layers::IPv4:
            json.start_object("ip");
            // TODO: add all ip header data
            json.end_object();
            return json.stringify();
        // TODO: include all layers for json parsing
        default:
            json.start_object("notyet");
            //
            json.end_object();
            return json.stringify();
    }
}
