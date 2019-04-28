#include "validate.h"
#include "yaml-cpp/yaml.h"
#include "exceptions.h"
#include "logging.h"

int main(int argc, char **argv) {
    Logging::init(LOG_INFO);

    string specs_path = SPECS_CONFIG_PATH;
    string sensor_path = CONFIG_FULL_PATH;

    try {
        YAML::Node specs = YAML::LoadFile(specs_path);
        YAML::Node config = YAML::LoadFile(sensor_path);
        Validation::validate_config(sensor_path);
    } catch (ConfigurationError &er) {
        fprintf(stderr, "Error: 'validate': %s\n", er.what());
        return 1;
    } catch (std::exception &er) {
        fprintf(stderr, "Error: 'validate': %s\n", er.what());
        return 2;
    }

    fprintf(stderr, "OK! Config is Valid.\n");
    return 0;
}