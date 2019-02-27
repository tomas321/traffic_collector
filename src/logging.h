//
// Created by tomas on 27/02/19.
//

#ifndef TRAFFIC_COLLECTOR_LOGGING_H
#define TRAFFIC_COLLECTOR_LOGGING_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#define BASE_PATH "/etc/traffic_collector"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace std;

class Logging: public {
private:
    static Logging *logger;
    Logging();

public:
    static Logging *get_instance();
    string base_path;
    void trace(string msg);
    void debug(string msg);
    void info(string msg);
    void warning(string msg);
    void error(string msg);
    void fatal(string msg);
};


#endif //TRAFFIC_COLLECTOR_LOGGING_H
