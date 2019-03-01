//
// Created by tomas on 01/03/19.
//

#ifndef TRAFFIC_COLLECTOR_CONFIGURATION_H
#define TRAFFIC_COLLECTOR_CONFIGURATION_H


#include <stdint-gcc.h>
#include <string>

using namespace std;

enum sniff_direction {
    in,
    out,
    promisc
};
struct filter_settings {
    string src_ip;
    string dst_ip;
};
struct database_settings {
    string beats_host;
    uint16_t beats_port;
    string elastic_host;
    uint16_t elastic_port;
    string archive_path;
    uint32_t archive_limit;
};
struct interface_settings {
    string interface;
    sniff_direction direction;
};

class Configuration {
private:
    static Configuration *configuration;
    const string config_file_path = "/etc/sensor/conf.d/";
    filter_settings filter_config;
    database_settings db_config;
    interface_settings iface_config;
    Configuration();

public:
    static Configuration *initialize();
    filter_settings get_filter_config();
    database_settings get_db_config();
    interface_settings get_iface_config();
};

#endif //TRAFFIC_COLLECTOR_CONFIGURATION_H
