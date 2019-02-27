//
// Created by tomas on 27/02/19.
//

#include <iostream>
#include "controller.h"
#include "logging.h"

string Controller::get_name() {
    return this->name;
}

int Controller::get_year() {
    return this->year;
}

int main(int argc, char **argv) {
    Logging *log = Logging::get_instance();
    cout << log->base_path;
}
