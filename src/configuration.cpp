//
// Created by tomas on 01/03/19.
//

#include <iostream>
#include "configuration.h"
#include "logging.h"
#include "exceptions.h"

Configuration *Configuration::configuration = nullptr;

Configuration *Configuration::initialize() {
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
    // default is empty string
    if (config["filter"]) {
        main_config.filter_config.src_ip = config["filter"]["src"].as<string>("");
        main_config.filter_config.dst_ip = config["filter"]["dst"].as<string>("");
    }
}

void Configuration::config_database(const YAML::Node &config) {
    if (config["database"]) {
        if (config["database"]["beats"]) {
            main_config.database_config.beats_host = config["database"]["beats"]["host"].as<string>(DEFAULT_BEATS_HOST);
            main_config.database_config.beats_port = config["database"]["beats"]["port"].as<uint16_t>(DEFAULT_BEATS_PORT);
        }
        if (config["database"]["elastic"]) {
            main_config.database_config.elastic_host = config["database"]["elastic"]["host"].as<string>(DEFAULT_ELASTIC_HOST);
            main_config.database_config.elastic_port = config["database"]["elastic"]["port"].as<uint16_t>(DEFAULT_ELASTIC_PORT);
        }
        if (config["database"]["archive"]) {
            main_config.database_config.archive_path = config["database"]["archive"]["path"].as<string>(DEFAULT_ARCHIVE_PATH);
            main_config.database_config.archive_limit = config["database"]["archive"]["limit"].as<uint32_t>(DEFAULT_ARCHIVE_LIMIT);
        }
    }
}

void Configuration::error_fallback(const vector<string> &keys) {
    string concat_keys;
    for (const auto &key : keys) concat_keys += key;
    Logging::log(debug, concat_keys + ": is left with its default value");
}

void Configuration::config_sensor(const YAML::Node &config) {
    string tmp = config["sensor"]["direction"].as<string>("promisc");
    main_config.sensor_config.direction = str_to_enum(tmp);

    // sensor interface is a mandatory setting.
    // TODO: should be checked by the validator
    try {
        main_config.sensor_config.interface = config["sensor"]["interface"].as<string>();
    } catch (const YAML::InvalidNode &er) {
        throw ConfigurationError("sensor.interface is a mandatory setting. missing value.");
    }
}

sniff_direction Configuration::str_to_enum(string source) {
    /* TODO: make mapper a public thing */
    map<string, sniff_direction> mapper;
    mapper["promisc"] = sniff_direction::promisc;
    mapper["in"] = sniff_direction::in;
    mapper["out"] = sniff_direction::out;

    // TODO: it should be checked by the configuration checker and not in parsing
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
