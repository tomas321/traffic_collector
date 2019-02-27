//
// Created by tomas on 27/02/19.
//

#include "logging.h"

Logging *Logging::logger = nullptr;

Logging::Logging() {
    this->base_path = BASE_PATH;

    logging::add_file_log(
            keywords::file_name = "sample_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format = "[%TimeStamp%]: %Message%"
    );

    logging::core::get()->set_filter
            (
                    logging::trivial::severity >= logging::trivial::info
            );
}

Logging *Logging::get_instance() {
    if (logger == nullptr) {
        logger = new Logging();
    }
    return logger;
}

void Logging::trace(string msg) {
    BOOST_LOG_TRIVIAL(trace) << msg;
}

void Logging::debug(string msg) {
    BOOST_LOG_TRIVIAL(debug) << msg;
}

void Logging::info(string msg) {
    BOOST_LOG_TRIVIAL(info) << msg;
}

void Logging::warning(string msg) {
    BOOST_LOG_TRIVIAL(warning) << msg;
}

void Logging::error(string msg) {
    BOOST_LOG_TRIVIAL(error) << msg;
}

void Logging::fatal(string msg) {
    BOOST_LOG_TRIVIAL(fatal) << msg;
}