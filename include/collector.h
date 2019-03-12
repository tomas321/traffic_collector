//
// Created by tomas on 10/03/19.
//

#ifndef TRAFFIC_COLLECTOR_COLLECTOR_H
#define TRAFFIC_COLLECTOR_COLLECTOR_H

#include "configuration.h"
#include <pcap/pcap.h>
#include <arpa/inet.h>

#define GIGABIT_SEC_SIZE 125000000 // data in second on 1Gb/s
#define SNAPLEN 1500
#define FILTER_TO_INCOMING_IP(ip) "dst host " + ip
#define FILTER_TO_OUTGOING_IP(ip) "src host " + ip
#define FILTER_TO_INCOMING_MAC(mac) "ether dst " + mac
#define FILTER_TO_OUTGOING_MAC(mac) "ether src " + mac
#define FILTER_IP(ip, dir) (dir == in) ? "dst host " + ip : "src host " + ip
#define FILTER_MAC(mac, dir) (dir == in) ? "ether dst " + mac : "ether src " + mac

class Collector {
private:
    sensor_settings sensor_config;
    filter_settings filter_config;
    pcap_t *capture_handle;
    string device_ip_addr;
    string device_mac_addr;

    void setup_handle();
    int set_capture_handle(const string &device);
    int set_preferences();
    int set_direction_capture();
    void set_direction_by_filter(const char *filter_str);
    int activate_handle();

public:
    Collector(const sensor_settings &sensor_config, const filter_settings &filter_config);
};


#endif //TRAFFIC_COLLECTOR_COLLECTOR_H
