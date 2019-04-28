//
// Created by tomas on 01/03/19.
//

#ifndef TRAFFIC_COLLECTOR_CONFIGURATION_H
#define TRAFFIC_COLLECTOR_CONFIGURATION_H


#include <string>
#include <yaml-cpp/yaml.h>

#define CONFIG_FULL_PATH "resources/sensor.yml"
#define DEFAULT_BEATS_HOST "127.0.0.1"
#define DEFAULT_BEATS_PORT 12000

using namespace std;


enum sniff_direction {
    in,
    out,
    promisc
};

struct database_settings {
    string beats_host = DEFAULT_BEATS_HOST;
    uint16_t beats_port = DEFAULT_BEATS_PORT;
};
struct sensor_settings {
    string interface;
    sniff_direction direction = promisc;
};
struct packed_settings{
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
    void config_database(const YAML::Node&);
    void config_sensor(const YAML::Node&);

public:
    static Configuration *initialize() {
        if (configuration == nullptr) {
            configuration = new Configuration();
        }
        return configuration;
    }
    packed_settings get_configuration(); // maybe include separate getters of each configuration
    // this is used by any other class/object to convert enum values to str
    static string enum_to_str(sniff_direction dir) {
        map<sniff_direction, string> mapper;
        mapper[sniff_direction::promisc] = "promisc";
        mapper[sniff_direction::in] = "in";
        mapper[sniff_direction::out] = "out";

        // no mapper check of loaded configuration, because values already prechecked
        return mapper[dir];
    }
};

#endif //TRAFFIC_COLLECTOR_CONFIGURATION_H
