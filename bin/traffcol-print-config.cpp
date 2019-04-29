#include <iostream>
#include "logging.h"
#include "configuration.h"
#include "exceptions.h"

int main(int argc, char **argv) {
    Logging::init(LOG_INFO); // minimum logging level is INFO

    Configuration *configuration;

    try {
        configuration = Configuration::initialize();
    } catch (const ConfigurationError &er) {
        fprintf(stderr, "Error: 'configuration error': %s", er.what());
        return 1;
    }

    packed_settings config = configuration->get_configuration();

    cout << "sensor:" << endl
         << "  interface: " << config.sensor_config.interface << endl
         << "  direction: " << Configuration::enum_to_str(config.sensor_config.direction) << endl;
    cout << "beats: " << config.database_config.beats_host << ":" << config.database_config.beats_port << endl;

    return 0;
}