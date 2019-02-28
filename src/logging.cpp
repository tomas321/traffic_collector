//
// Created by tomas on 27/02/19.
//

#include "logging.h"

Logging *Logging::logger = nullptr;

Logging::Logging() {
    this->base_path = BASE_PATH;

    boost::log::add_file_log(
            boost::log::keywords::file_name = this->base_path + "sample_%Y.log",
            boost::log::keywords::rotation_size = 10 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(12, 0, 0),
            boost::log::keywords::format = "[%TimeStamp%]: %Message%"
    );

    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
    );
    boost::log::add_common_attributes();
}
