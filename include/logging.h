//
// Created by tomas on 27/02/19.
//

#ifndef TRAFFIC_COLLECTOR_LOGGING_H
#define TRAFFIC_COLLECTOR_LOGGING_H

#include <syslog.h>
#include <string>
#include <map>


enum log_level {
    debug,
    info,
    notice,
    warning,
    error,
    critical,
    alert,
    emergency
};

class Logging {
private:
    static const std::map<log_level, int> log_map;
    static Logging *logger;
    Logging(int lvl);

public:
    virtual ~Logging();

public:
    static void init(int severity_level);
    static void log(log_level level, const char *msg);
    static void log(log_level level, const std::string &msg);
};


#endif //TRAFFIC_COLLECTOR_LOGGING_H
