#ifndef TRAFFIC_COLLECTOR_LOGGING_H
#define TRAFFIC_COLLECTOR_LOGGING_H

#include <syslog.h>
#include <string>
#include <map>


/**
 * Log level enumeration to keep backward compatibility.
 */
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
    /**
     * Mapping of previous (boost) log level names with syslog log levels.
     */
    static const std::map<log_level, int> log_map;
    static Logging *logger;

    /**
     * Open syslog session (descriptor) and set minimum log level.
     * @param lvl Minimum log level.
     */
    explicit Logging(int lvl);

public:
    /**
     * Closes the log level descriptor.
     */
    virtual ~Logging();

public:
    /**
     * Initialize the logging descriptor with minimum loging level.
     * @param severity_level Minimum log level.
     */
    static void init(int severity_level);

    /**
     * Log message of given severity level.
     *
     * @param level Message severity level (log_level).
     * @param msg Log message.
     */
    static void log(log_level level, const char *msg);

    /**
     * Log message of given severity level.
     *
     * @param level Message severity level (log_level).
     * @param msg Log message.
     */
    static void log(log_level level, const std::string &msg);
};


#endif //TRAFFIC_COLLECTOR_LOGGING_H
