//
// Created by tomas on 16/03/19.
//

#include "parser.h"
#include "json.h"

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

void Parser::process_packet(const uint32_t packet_len, const uint32_t caplen, const struct timeval &timestamp, uint8_t *packet, const int datalink) {
    // TODO: rethink jsonizing API and all available data
    jsonize_packet(packet);
}

string Parser::jsonize_packet(uint8_t *raw_packet) {
    string json_packet = "json_packet";

    // TODO: loop all data and one by one parse_protocol

    return json_packet;
}

string Parser::parse_protocol(Packet::Type type, uint8_t *packet_layer) {
    Json json;

    switch (type) {
        case Packet::Type::Ethernet:
            json.start_object("ethernet");
            // TODO: add all ethernet header data
            json.end_object();
            return json.stringify();
        case Packet::Type::IPv4:
            json.start_object("ip");
            // TODO: add all ip header data
            json.end_object();
            return json.stringify();
    }
}
