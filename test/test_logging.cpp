//
// Created by tomas on 28/03/19.
//

#include "catch.hpp"
#include "logging.h"

TEST_CASE("Test program logging to local syslog") {
    Logging::init(LOG_NOTICE);

    SECTION("Send dummy debug msg to syslog") {
        Logging::log(debug, "this is a test debug message");
    }
    SECTION("Send dummy info msg to syslog") {
        Logging::log(info, "this is a test info message");
    }
    SECTION("Send dummy notice msg to syslog") {
        Logging::log(notice, "this is a test notice message");
    }
    SECTION("Send dummy warning msg to syslog") {
        Logging::log(warning, "this is a test warning message");
    }
    SECTION("Send dummy error msg to syslog") {
        Logging::log(error, "this is a test error message");
    }
    SECTION("Send dummy critical msg to syslog") {
        Logging::log(critical, "this is a test critical message");
    }
    SECTION("Send dummy critical msg to syslog") {
        Logging::log(alert, "this is a test alert message");
    }
    SECTION("Send dummy critical msg to syslog") {
        Logging::log(emergency, "this is a test emrgency message");
    }
}