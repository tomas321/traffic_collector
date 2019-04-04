//
// Created by tomas on 10/03/19.
//

#include <iostream>

#include "databese_controller.h"
#include "collector.h"
#include "exceptions.h"
#include "logging.h"

int main(int argc, char **argv) {
    Logging::init(LOG_DEBUG);

    sensor_settings sensor_config{"eno1", sniff_direction::promisc};
    filter_settings filter_config{"", ""};
    Processor *parser;
    DatabaseController *db_control;
    Harmonization *harmonization;

    try{
        db_control = new DatabaseController(12000, "elk.bp.local");
        harmonization = new Harmonization();
        parser = new Processor(db_control, harmonization);
        Collector c(sensor_config, filter_config, parser);
        c.capture_network_packets(0);
        delete db_control;
    } catch (SocketError &e) {
        Logging::log(e.get_severity(), e.what());
        return 3;
    } catch (ParserError &e) {
        Logging::log(e.get_severity(), e.what());
        return 2;
    } catch (CaptureError &er) {
        Logging::log(er.get_severity(), er.what());
        return 1;
    }

    return 0;
}