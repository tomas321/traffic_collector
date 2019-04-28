#ifndef TRAFFIC_COLLECTOR_VALIDATE_H
#define TRAFFIC_COLLECTOR_VALIDATE_H

#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>

#include "config_files.h"

using namespace std;

class Validation {
private:
    static string sensor_config_full_path;

    /**
     * Recursively deep reads the specification configuration file.
     *
     * @param specification Specification configuration YAML Node.
     * @param key Current key when deep reading. Starting on empty string.
     */
    static void specification_validatation(const YAML::Node &specification, const string &key = "");

public:
    /**
     * Check presence of Key in actual configuration file.
     *
     * @param configuration_field_path Dot separated field key.
     * @param optional Whether the key is optional.
     */
    static void check_key(const string &configuration_field_path, const bool &optional);

    /**
     * Iterates the specification and configuration file simultaneously.
     *
     * @param configuration Configuration file YAML Node.
     * @param specification Specification configuration file YAML Node.
     * @param root_key Root key when deep reading.
     */
    static void iterate_whole_yaml(const YAML::Node &configuration,
                                   const YAML::Node &specification,
                                   const string &root_key = "");

    /**
     * Runs `specification_validation` for specific configuration file.
     *
     * @param configuration_file_path Path to configuration file.
     */
    static void validate_config(const string &configuration_file_path);
};

#endif //TRAFFIC_COLLECTOR_VALIDATE_H