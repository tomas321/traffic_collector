//
// Created by tomas on 27/02/19.
//

#include <iostream>
#include "controller.h"
#include "logging.h"

int main(int argc, char **argv) {
    Logging::init(logging::trivial::debug);

    Logging::log(logging::trivial::debug, "test message");
}
