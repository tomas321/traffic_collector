//
// Created by tomas on 03/03/19.
//

#include <stdexcept>

#ifndef TRAFFIC_COLLECTOR_EXCEPTIONS_H
#define TRAFFIC_COLLECTOR_EXCEPTIONS_H

#endif //TRAFFIC_COLLECTOR_EXCEPTIONS_H

class ConfigurationError: public std::logic_error {
public:
    ConfigurationError(const std::string &msg): std::logic_error(msg) {}
    ConfigurationError(): std::logic_error("Invalid configuration value") {}
};