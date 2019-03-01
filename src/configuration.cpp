//
// Created by tomas on 01/03/19.
//

#include <iostream>
#include "configuration.h"
#include "logging.h"

Configuration *Configuration::configuration = nullptr;

Configuration::Configuration() {
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

interface_settings Configuration::get_iface_config() {
    Logging::log(logging::trivial::info, "retrieved interface configuration");
    return this->iface_config;
}
