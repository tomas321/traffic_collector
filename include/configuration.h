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
struct packed_settings{
    filter_settings filter_config;
    database_settings database_config;
    sensor_settings sensor_config;
};

class Configuration {
private:
    static Configuration *configuration;
    const string config_file_path = CONFIG_FULL_PATH;

    packed_settings main_config;
    Configuration();
    ~Configuration();

    template <typename T>
    static T load_or_default(int key_count, std::vector<string> keys, const YAML::Node &config, T default_value);
    static sniff_direction str_to_enum(string);  // should be used only on configuration insertion

    void load_configuration(const string&);
    void config_filter(const YAML::Node&);
    void config_database(const YAML::Node&);
    void config_sensor(const YAML::Node&);

public:
    static Configuration *initialize();
    packed_settings get_configuration(); // maybe include separate getters of each configuration
    static string enum_to_str(sniff_direction); // this is used by any other class/object to convert enum values to str
};

#endif //TRAFFIC_COLLECTOR_CONFIGURATION_H
