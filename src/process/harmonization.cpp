#include "harmonization.h"
#include "logging.h"

using namespace std;

Harmonization::Harmonization(const std::string &geoip_filepath) : geoip_filepath(geoip_filepath) {
    gi = GeoIP_open(geoip_filepath.c_str(), GEOIP_MEMORY_CACHE);
    Logging::log(info, "Opened GeoIP database for IP resolution");
}

Harmonization::Harmonization() : Harmonization(GEOIP_FILEPATH) {}

string Harmonization::geoip(const std::string &ip) {
    return string(GeoIP_country_code_by_addr(gi, ip.c_str()));
}