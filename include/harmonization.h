#ifndef TRAFFIC_COLLECTOR_HARMONIZATION_H
#define TRAFFIC_COLLECTOR_HARMONIZATION_H


#include <string>
#include <GeoIP.h>

#define GEOIP_FILEPATH "resources/geoip/GeoIP.dat"

class Harmonization {
private:
    std::string geoip_filepath;
    GeoIP *gi;

public:
    /**
     * Initialize geoip database for searching.
     *
     * @param geoip_filepath Path to GeoIP.dat file
     */
    explicit Harmonization(const std::string &geoip_filepath);

    /**
     * Initialize geoip database for searching with default path to GeoIP.dat file
     */
    Harmonization();

    /**
     * Get geoip info for IP address.
     *
     * @param ip IP address to be queried.
     * @return Geoip data structure.
     */
    std::string geoip(const std::string &ip);
};


#endif //TRAFFIC_COLLECTOR_HARMONIZATION_H
