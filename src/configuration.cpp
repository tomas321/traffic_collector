//
// Created by tomas on 01/03/19.
//

#include <iostream>
#include "configuration.h"
#include "logging.h"

Configuration *Configuration::configuration = nullptr;

Configuration::Configuration() {
    YAML::Node config_data = YAML::LoadFile(config_file_path);

    this->config_filter(config_data, this->filter_config);
    this->config_database(config_data, this->db_config);
    this->config_sensor(config_data, this->sensor_config);

    Logging::log(logging::trivial::info, "loaded config from " + config_file_path);
}

Configuration* Configuration::initialize() {
    if (configuration == nullptr) {
        configuration = new Configuration();
    }
    return configuration;
}

filter_settings Configuration::get_filter_config() {
    Logging::log(logging::trivial::info, "retrieved filter configuration");
    return this->filter_config;
}

database_settings Configuration::get_db_config() {
    Logging::log(logging::trivial::info, "retrieved database configuration");
    return this->db_config;
}

sensor_settings Configuration::get_sensor_config() {
    Logging::log(logging::trivial::info, "retrieved interface configuration");
    return this->sensor_config;
}

void Configuration::config_filter(const YAML::Node &config, filter_settings &settings) {
    // TODO: should be optional so default values are to be used
    settings.src_ip = config["filter"]["src"].as<string>();
    settings.dst_ip = config["filter"]["dst"].as<string>();
}

void Configuration::config_database(const YAML::Node &config, database_settings &settings) {
    // TODO: should all have default values for no configuration specified
    settings.beats_host = config["database"]["beats"]["host"].as<string>();
    settings.beats_port = config["database"]["beats"]["port"].as<uint16_t>();

    settings.elastic_host = config["database"]["elastic"]["host"].as<string>();
    settings.elastic_port = config["database"]["elastic"]["port"].as<uint16_t>();

    settings.archive_path = config["database"]["archive"]["path"].as<string>();
    settings.archive_limit = config["database"]["archive"]["limit"].as<uint32_t >();
}

void Configuration::config_sensor(const YAML::Node &config, sensor_settings &settings) {
    // TODO: should all have default values for no configuration specified
    settings.interface = config["sensor"]["interface"].as<string>();
    settings.direction = str_to_enum(config["sensor"]["direction"].as<string>());
}

sniff_direction Configuration::str_to_enum(string source) {
    map<string, sniff_direction> mapper;
    mapper["promisc"] = sniff_direction::promisc;
    mapper["in"] = sniff_direction::in;
    mapper["out"] = sniff_direction::out;

    // TODO: include mapper check
    return mapper[source];
}

string Configuration::enum_to_str(sniff_direction dir) {
    map<sniff_direction, string> mapper;
    mapper[sniff_direction::promisc] = "promisc";
    mapper[sniff_direction::in] = "in";
    mapper[sniff_direction::out] = "out";

    // TODO: include mapper check
    return mapper[dir];
}
