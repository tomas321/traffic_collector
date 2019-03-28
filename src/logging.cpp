//
// Created by tomas on 27/02/19.
//

#include "logging.h"

using namespace std;

Logging *Logging::logger = nullptr;

const std::map<log_level, int> Logging::log_map = {
        {debug, LOG_DEBUG},
        {info, LOG_INFO},
        {notice, LOG_NOTICE},
        {warning, LOG_WARNING},
        {error, LOG_ERR},
        {critical, LOG_CRIT},
        {alert, LOG_ALERT},
        {emergency, LOG_EMERG}
};

void Logging::init(int severity_level) {
    if (logger == nullptr) logger = new Logging(severity_level);
}

void Logging::log(log_level level, const char *msg) {
    syslog(log_map.at(level), "%s", msg);
    printf("logged: [%d] %s\n", level, msg);
}

void Logging::log(log_level level, const std::string& msg) {
    log(level, msg.c_str());
}

Logging::Logging(int level) {
    openlog("traffic_collector", (LOG_CONS | LOG_NDELAY | LOG_PID), LOG_LOCAL1);
    setlogmask(LOG_UPTO(level));
}

Logging::~Logging() {
    closelog();
}
