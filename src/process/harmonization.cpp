//
// Created by tomas on 02/04/19.
//

#include "harmonization.h"

using namespace std;

Harmonization::Harmonization(const std::string &geoip_filepath) : geoip_filepath(geoip_filepath) {
    gi = GeoIP_open(geoip_filepath.c_str(), GEOIP_MEMORY_CACHE);
}

Harmonization::Harmonization() {
    geoip_filepath = GEOIP_FILEPATH;
    gi = GeoIP_open(geoip_filepath.c_str(), GEOIP_MEMORY_CACHE);
}

string Harmonization::geoip(const std::string &ip) {
    return string(GeoIP_country_code_by_addr(gi, ip.c_str()));
}