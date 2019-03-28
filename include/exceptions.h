//
// Created by tomas on 03/03/19.
//

#include <stdexcept>
#include "logging.h"

#ifndef TRAFFIC_COLLECTOR_EXCEPTIONS_H
#define TRAFFIC_COLLECTOR_EXCEPTIONS_H

#endif //TRAFFIC_COLLECTOR_EXCEPTIONS_H

class ConfigurationError : public std::logic_error {
private:
    log_level severity_level;
public:
    explicit ConfigurationError(const std::string &msg = "Invalid configuration value",
                                log_level severity_level = error)
            : std::logic_error(msg), severity_level(severity_level) {}

    log_level get_severity() {
        return severity_level;
    };
};

class CaptureError : public std::logic_error {
private:
    log_level severity_level;
public:
    explicit CaptureError(const std::string &msg = "Capturing raised an error",
                          log_level severity_level = error)
            : logic_error(msg), severity_level(severity_level) {}

    log_level get_severity() {
        return severity_level;
    };
};

class SocketError : public std::logic_error {
private:
    log_level severity_level;
public:
    explicit SocketError(const std::string &msg = "Manipulation with socket raised an error",
                         log_level severity_level = error)
            : logic_error(msg), severity_level(severity_level) {}

    log_level get_severity() {
        return severity_level;
    };
};
