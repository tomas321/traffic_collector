#include "controller.h"

Controller::Controller(int log_level) {
    Logging::init(log_level);
}

int Controller::start() {
    Logging::log(info, "Starting traffcol");
    try {
        configuration = Configuration::initialize();
    } catch (ConfigurationError &er) {
        Logging::log(er.get_severity(), "Configuration error: " + std::string(er.what()));
        return 4;
    }

    try{
        db_control = new DatabaseController(configuration->get_configuration().database_config);
        harmonization = new Harmonization();
        parser = new Processor(db_control, harmonization);
        Collector c(configuration->get_configuration().sensor_config, parser);
        c.capture_network_packets(PACKET_COUNT);
        delete db_control;
        delete  harmonization;
        delete parser;
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
