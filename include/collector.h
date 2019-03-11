//
// Created by tomas on 10/03/19.
//

#ifndef TRAFFIC_COLLECTOR_COLLECTOR_H
#define TRAFFIC_COLLECTOR_COLLECTOR_H

#include "configuration.h"
#include <pcap/pcap.h>

#define GIGABIT_SEC_SIZE 125000000 // data in second on 1Gb/s
#define SNAPLEN 1500
#define FILTER_TO_INCOMING(ip) "dst host " + ip
#define FILTER_TO_OUTGOING(ip) "src host " + ip

class Collector {
private:
    sensor_settings sensor_config;
    filter_settings filter_config;
    pcap_t *capture_handle;

    void setup_handle();
    int set_capture_handle(const string &device);
    int set_preferences();
    int set_direction_capture();

public:
    Collector(const sensor_settings &sensor_config, const filter_settings &filter_config);
};


#endif //TRAFFIC_COLLECTOR_COLLECTOR_H
