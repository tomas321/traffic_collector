//
// Created by tomas on 09/03/19.
//

#ifndef TRAFFIC_COLLECTOR_VALIDATE_H
#define TRAFFIC_COLLECTOR_VALIDATE_H

#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#define SPECS_CONFIG_PATH (boost::filesystem::current_path() / boost::filesystem::path("doc/config_specs.yml")).generic_string()

using namespace std;
using namespace boost::filesystem;

class Validation {
private:
    static string sensor_config_full_path;
    static void specification_validatation(const YAML::Node &specification, const string &key = "");

public:
    static void check_key(const string &configuration_field_path, const bool &optional);
    static void iterate_whole_yaml(const YAML::Node &configuration,
                                   const YAML::Node &specification,
                                   const string &root_key = "");
    static void validate_config(const string &configuration_file_path);
};

#endif //TRAFFIC_COLLECTOR_VALIDATE_H