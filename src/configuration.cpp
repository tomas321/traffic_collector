//
// Created by tomas on 01/03/19.
//

#include <iostream>
#include "configuration.h"
#include "logging.h"
#include "exceptions.h"
#include "validate.h"

Configuration *Configuration::configuration = nullptr;

Configuration::Configuration() {
    try {
        load_configuration(config_file_path);
        Logging::log(info, "loaded config from " + config_file_path);
    } catch (const YAML::ParserException &ex) {
        throw ConfigurationError(ex.what()); // msq or what()
    }
}

void Configuration::load_configuration(const string &config_path) {
    Validation::validate_config(CONFIG_FULL_PATH);

    YAML::Node config_data = YAML::LoadFile(config_file_path);

    config_database(config_data);
    config_sensor(config_data);
}

void Configuration::config_database(const YAML::Node &config) {
    if (config["database"]) {
        if (config["database"]["beats"]) {
            main_config.database_config.beats_host = config["database"]["beats"]["host"].as<string>(DEFAULT_BEATS_HOST);
            main_config.database_config.beats_port = config["database"]["beats"]["port"].as<uint16_t>(DEFAULT_BEATS_PORT);
        }
    }
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

packed_settings Configuration::get_configuration() {
    return main_config;
}
