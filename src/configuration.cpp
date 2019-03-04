//
// Created by tomas on 01/03/19.
//

#include <iostream>
#include "configuration.h"
#include "logging.h"
#include "exceptions.h"

Configuration *Configuration::configuration = nullptr;

Configuration* Configuration::initialize() {
    if (configuration == nullptr) {
        configuration = new Configuration();
    }
    return configuration;
}

Configuration::Configuration() {
    try {
        load_configuration(config_file_path);
        Logging::log(info, "loaded config from " + config_file_path);
    } catch (const YAML::ParserException &ex) {
        throw ConfigurationError(ex.what()); // msq or what()
    }
}

void Configuration::load_configuration(const string &config_path) {
    YAML::Node config_data = YAML::LoadFile(config_file_path);

    config_filter(config_data);
    config_database(config_data);
    config_sensor(config_data);
}

void Configuration::config_filter(const YAML::Node &config) {
    // TODO: should be optional so default values are to be used
    main_config.filter_config.src_ip = config["filter"]["src"].as<string>();
    main_config.filter_config.dst_ip = config["filter"]["dst"].as<string>();
}

void Configuration::config_database(const YAML::Node &config) {
    // TODO: should all have default values for no configuration specified
    main_config.database_config.beats_host = config["database"]["beats"]["host"].as<string>();
    main_config.database_config.beats_port = config["database"]["beats"]["port"].as<uint16_t>();

    main_config.database_config.elastic_host = config["database"]["elastic"]["host"].as<string>();
    main_config.database_config.elastic_port = config["database"]["elastic"]["port"].as<uint16_t>();

    main_config.database_config.archive_path = config["database"]["archive"]["path"].as<string>();
    main_config.database_config.archive_limit = config["database"]["archive"]["limit"].as<uint32_t>();
}

void Configuration::config_sensor(const YAML::Node &config) {
    // TODO: should all have default values for no configuration specified
    main_config.sensor_config.interface = Configuration::load_or_default<string>(2, std::vector<string>{"sensor", "interface"}, config, "eno1"); // config["sensor"]["interface"].as<string>();
    auto tmp = Configuration::load_or_default<string>(2, std::vector<string>{"sensor", "direction"}, config, "promisc");
    main_config.sensor_config.direction = str_to_enum(tmp);
}

template <typename T>
T Configuration::load_or_default(int key_count, std::vector<std::string> keys, const YAML::Node &config, T default_value) {
    try {
        switch (key_count) {
            case 2:
                return config[keys[0]][keys[1]].as<T>();
            case 3:
                return config[keys[0]][keys[1]][keys[2]].as<T>();
        }
    } catch (YAML::InvalidNode &er) {
        return default_value;
    }
}

sniff_direction Configuration::str_to_enum(string source) {
    map<string, sniff_direction> mapper;
    mapper["promisc"] = sniff_direction::promisc;
    mapper["in"] = sniff_direction::in;
    mapper["out"] = sniff_direction::out;

    // TODO: include mapper check
    try {
        return mapper.at(source);
    } catch (const std::out_of_range &ex) {
        Logging::log(error, "Invalid sniffing direction " + source);
        throw ConfigurationError();
    }
}

string Configuration::enum_to_str(sniff_direction dir) {
    map<sniff_direction, string> mapper;
    mapper[sniff_direction::promisc] = "promisc";
    mapper[sniff_direction::in] = "in";
    mapper[sniff_direction::out] = "out";

    // no mapper check of loaded configuration, because values already prechecked
    return mapper[dir];
}

packed_settings Configuration::get_configuration() {
    return main_config;
}
