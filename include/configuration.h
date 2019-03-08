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
#define DEFAULT_BEATS_HOST "127.0.0.1"
#define DEFAULT_BEATS_PORT 12000
#define DEFAULT_ELASTIC_HOST "127.0.0.1"
#define DEFAULT_ELASTIC_PORT 9200
#define DEFAULT_ARCHIVE_PATH "/etc/sensor"
#define DEFAULT_ARCHIVE_LIMIT 50000 // in MB

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
    string beats_host = DEFAULT_BEATS_HOST;
    uint16_t beats_port = DEFAULT_BEATS_PORT;
    string elastic_host = DEFAULT_ELASTIC_HOST;
    uint16_t elastic_port = DEFAULT_ELASTIC_PORT;
    string archive_path = DEFAULT_ARCHIVE_PATH;
    uint32_t archive_limit = DEFAULT_ARCHIVE_LIMIT;
};
struct sensor_settings {
    string interface;
    sniff_direction direction = promisc;
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
