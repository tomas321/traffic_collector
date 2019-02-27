//
// Created by tomas on 27/02/19.
//

#ifndef TRAFFIC_COLLECTOR_CONTROLLER_H
#define TRAFFIC_COLLECTOR_CONTROLLER_H

#include <string>

using namespace std;

class Controller {
private:
    string name;
    int year;

public:
    string get_name();
    int get_year();
};


#endif //TRAFFIC_COLLECTOR_CONTROLLER_H
