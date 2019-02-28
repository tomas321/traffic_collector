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

#define BASE_PATH "/tmp/" //etc/traffic_collector/"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace std;

class Logging {
private:
    static Logging *logger;
    string base_path;
    Logging();

public:
    static void init(logging::trivial::severity_level level) {
        if (logger == nullptr) {
            logger = new Logging();
            boost::log::core::get()->set_filter(
                    boost::log::trivial::severity >= level
            );
        }
    }
    static void log(logging::trivial::severity_level level, const string &msg) {
        src::severity_logger< logging::trivial::severity_level > severity_logger;
        BOOST_LOG_SEV(severity_logger, level) << msg;
    }
};


#endif //TRAFFIC_COLLECTOR_LOGGING_H
