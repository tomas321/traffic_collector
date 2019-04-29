#ifndef TRAFFIC_COLLECTOR_CONTROLLER_H
#define TRAFFIC_COLLECTOR_CONTROLLER_H

#include "traffcol.h"

using namespace std;

class Controller {
private:
    Configuration *configuration;
    Processor *parser;
    DatabaseController *db_control;
    Harmonization *harmonization;

public:
    /**
     * Initialize Main controller.
     *
     * @param log_level Minimum log level.
     */
    explicit Controller(int log_level);

    /**
     * Create and initialize object variables, run capture process and catch program exceptions.
     *
     * @return Exit code.
     */
    int start();
};


#endif //TRAFFIC_COLLECTOR_CONTROLLER_H
