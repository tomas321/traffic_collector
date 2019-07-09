#ifndef TRAFFIC_COLLECTOR_CONFIGURATION_H
#define TRAFFIC_COLLECTOR_CONFIGURATION_H


#include <string>
#include <yaml-cpp/yaml.h>

#include "config_files.h"

using namespace std;


/**
 * These are the possible capture directions
 * in - receiving (RX)
 * out - transmitting (TX)
 * promisc - promiscuous
 */
enum sniff_direction {
    in,
    out,
    promisc
};
/**
 * Databse settings structure holds the hostname and port of Filebeat daemon.
 */
struct database_settings {
    string beats_host = DEFAULT_BEATS_HOST;
    uint16_t beats_port = DEFAULT_BEATS_PORT;
};
/**
 * Sensor settings structure holds the capture device and the capture direction.
 */
struct sensor_settings {
    string interface;
    sniff_direction direction = promisc;
};
/**
 * Packed settings hold all setting structures.s
 */
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

    /**
     * Map capture direction as string to enumerated value.
     *
     * @param source String capture direction representation.
     * @return Capture direction enumerated value of type `sniff_direction`.
     */
    static sniff_direction str_to_enum(string source);  // should be used only on configuration insertion

    /**
     * Main function validating, opening config file and composses the `packed_settings` structure.
     *
     * @param config_path Path to configuration file.
     */
    void load_configuration(const string &config_path);

    /**
     * Fills in the database configuration part.
     *
     * @param config `YAML::Node` object to parse.
     */
    void config_database(const YAML::Node &config);

    /**
     * Fills in the sensor configuration part.
     *
     * @param config `YAML::Node` object to parse.
     */
    void config_sensor(const YAML::Node &config);

public:
    /**
     * Create instance and forbid multiple instances.
     *
     * @return Configuration object.
     */
    static Configuration *initialize() {
        if (configuration == nullptr) {
            configuration = new Configuration();
        }
        return configuration;
    }

    /**
     * Get configuration structure.
     *
     * @return `packed_settings` configuration structure.
     */
    packed_settings get_configuration();

    /**
     * Map `sniff_direction` enumerated value to a string equivalent.
     *
     * @param dir Capture direction.
     * @return Capture direction string equivalent.
     */
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
