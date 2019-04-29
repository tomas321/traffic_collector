#include "controller.h"

int main(int argc, char **argv) {
    Controller controller(LOG_INFO);

    int exit_code = controller.start();
    if (exit_code) Logging::log(error, "Exiting with exit code " + to_string(exit_code));
    else Logging::log(info, "Exiting successfully");
}