//
// Created by tomas on 06/03/19.
//

#include "validate.h"
#include "exceptions.h"

string Validation::sensor_config_full_path;
// TODO: document the functions and remove prints and change logging messages
void Validation::check_key(const string &field_path, const bool &optional) {
    string sensor_path = sensor_config_full_path;
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
    } else if (!node.IsDefined() && !optional) {
        string msg = "field " + field_path + " is missing\n";
        throw ConfigurationError(msg);
    }
}

void Validation::iterate_whole_yaml(const YAML::Node &config, const YAML::Node &specs, const string &key) {
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

void Validation::specification_validatation(const YAML::Node &specs, const string &key) {
    for (YAML::const_iterator it = specs.begin(); it != specs.end(); ++it) {
        string current_key;
        if (key.empty()) current_key = it->first.Scalar();
        else current_key = key + "." + it->first.Scalar();
        if (it->second.IsMap()) {
            specification_validatation(it->second, current_key);
        } else if (it->second.IsScalar()) {
//            cout << current_key << " has " << it->second.Scalar() << endl;
            check_key(current_key, it->second.as<bool>());
        } else if (it->second.IsSequence()) {
            for (int i = 0; i < it->second.size(); ++i);
//                cout << "has " << i << ": " << it->second[i] << endl;
        }
    }
}

void Validation::validate_config(const string &config_path) {
    YAML::Node specs = YAML::LoadFile(SPECS_CONFIG_PATH);

    sensor_config_full_path = config_path;

    specification_validatation(specs);
}
