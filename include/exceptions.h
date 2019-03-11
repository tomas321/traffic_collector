//
// Created by tomas on 03/03/19.
//

#include <stdexcept>

#ifndef TRAFFIC_COLLECTOR_EXCEPTIONS_H
#define TRAFFIC_COLLECTOR_EXCEPTIONS_H

#endif //TRAFFIC_COLLECTOR_EXCEPTIONS_H

class ConfigurationError: public std::logic_error {
private:
    int severity_level;
public:
    ConfigurationError(const std::string &msg, int severity_level): std::logic_error(msg),  severity_level(severity_level) {}
    ConfigurationError(const std::string &msg): std::logic_error(msg) {}
    ConfigurationError(): std::logic_error("Invalid configuration value") {}
};

class CaptureError: public std::logic_error {
private:
    int severity_level;
public:
    CaptureError(const string &msg, int severity_level): logic_error(msg), severity_level(severity_level) {}
    CaptureError(const std::string &msg): std::logic_error(msg) {}
    CaptureError(): std::logic_error("Capturing raised an error") {}
};