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
    explicit Controller(int log_level);
    int start();
};


#endif //TRAFFIC_COLLECTOR_CONTROLLER_H
