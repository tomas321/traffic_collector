//
// Created by tomas on 01/03/19.
//

#ifndef TRAFFIC_COLLECTOR_CONFIGURATION_H
#define TRAFFIC_COLLECTOR_CONFIGURATION_H


#include <stdint-gcc.h>
#include <string>
#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#define CONFIG_FULL_PATH (boost::filesystem::current_path() / boost::filesystem::path("doc/sensor.yml")).generic_string()

using namespace std;
using namespace boost;

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
struct sensor_settings {
    string interface;
    sniff_direction direction;
};

class Configuration {
private:
    static Configuration *configuration;
    const string config_file_path = CONFIG_FULL_PATH;
    filter_settings filter_config;
    database_settings db_config;
    sensor_settings sensor_config;
    Configuration();

public:
    static Configuration *initialize();
    filter_settings get_filter_config();
    database_settings get_db_config();
    sensor_settings get_sensor_config();
    void config_filter(const YAML::Node&, filter_settings&);
    void config_database(const YAML::Node&, database_settings&);
    void config_sensor(const YAML::Node&, sensor_settings&);
    static sniff_direction str_to_enum(string);
    static string enum_to_str(sniff_direction);
};

#endif //TRAFFIC_COLLECTOR_CONFIGURATION_H
