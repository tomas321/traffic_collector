//
// Created by tomas on 06/03/19.
//

#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include "exceptions.h"
#include "configuration.h"

using namespace std;
using namespace boost::filesystem;

bool check_key(const string &field_path, const bool optional) {
    string sensor_path = CONFIG_FULL_PATH;
    YAML::Node node = YAML::LoadFile(sensor_path);

    string delimiter = ".";
    size_t pos = 0;
    string token;
    string field_path_copy = field_path;

    while ((pos = field_path_copy.find(delimiter)) != string::npos) {
        node = node[field_path_copy.substr(0, pos)];
        field_path_copy.erase(0, pos + delimiter.length());
    }
    node = node[field_path_copy];

    if (node.IsNull() && optional) {
        throw ConfigurationError("bad config after while");
    } else if (! node.IsDefined() && ! optional) {
        string msg = "field " + field_path + " is missing\n";
        throw ConfigurationError(msg);
    }
}

void iterate_whole_yaml(const YAML::Node &config, const YAML::Node &specs, const string &key) {
    for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
        cout << key << " with " << it->first.Scalar() << " ";
        if (it->second.IsMap()) {
            cout << "has MAP" << endl;
            iterate_whole_yaml(it->second, specs[it->first.Scalar()], it->first.Scalar());
            cout << "finalized " << it->first.Scalar() << " of type " << it->second.Type() << endl;
        } else if (it->second.IsScalar()) {
            cout << "has " << it->second.Scalar() << endl;
        } else if (it->second.IsSequence()) {
            for (int i = 0; i < it->second.size(); ++i)
                cout << "has " << i << ": " << it->second[i] << endl;
        }
    }
}

void validate_config(const YAML::Node &specs, const YAML::Node config, const string &key = "") {
    for (YAML::const_iterator it = specs.begin(); it != specs.end(); ++it) {
        string current_key;
        if (key.empty()) current_key = it->first.Scalar();
        else current_key = key + "." + it->first.Scalar();
        if (it->second.IsMap()) {
            validate_config(it->second, config, current_key);
        } else if (it->second.IsScalar()) {
//            cout << current_key << " has " << it->second.Scalar() << endl;
            check_key(current_key, it->second.as<bool>());
        } else if (it->second.IsSequence()) {
            for (int i = 0; i < it->second.size(); ++i)
                cout << "has " << i << ": " << it->second[i] << endl;
        }
    }
}

int main(void) {
    string specs_path = (boost::filesystem::current_path() / boost::filesystem::path("doc/config_specs.yml")).generic_string();
    string sensor_path = CONFIG_FULL_PATH;

    YAML::Node specs = YAML::LoadFile(specs_path);
    YAML::Node config = YAML::LoadFile(sensor_path);

    try {
        validate_config(specs, config);
    } catch (const ConfigurationError &er) {
        cout << er.what() << endl;
        return 1;
    }

    return 0;
}