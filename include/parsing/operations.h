//
// Created by tomas on 20/03/19.
//

#ifndef TRAFFIC_COLLECTOR_PROTOCOL_TYPE_H
#define TRAFFIC_COLLECTOR_PROTOCOL_TYPE_H

#include <map>

namespace IPAddress {
    /**
     * Convert IP address from bytes to string representation.
     *
     * @param bytes Unsigned 32 bit raw ip address
     * @return String IP address
     */
    const char *to_string(uint32_t bytes) {
        char *ip = (char *) malloc(16);
        snprintf(ip, 16, "%u.%u.%u.%u", (bytes & 0xff000000) >> 24,
                 (bytes & 0x00ff0000) >> 16,
                 (bytes & 0x0000ff00) >> 8,
                 (bytes & 0x000000ff));
        return ip;
    }
}

namespace MACAddress {
    /**
     * Convert MAC address from bytes to string representation.
     *
     * @param bytes Raw MAC address bytes.
     * @return String MAC address.
     */
    const char *to_string(const uint8_t *bytes) {
        char *mac = (char *) malloc(18);
        snprintf(mac, 18, "%02X:%02X:%02X:%02X:%02X:%02X", *(bytes + 0),
                 *(bytes + 1),
                 *(bytes + 2),
                 *(bytes + 3),
                 *(bytes + 4),
                 *(bytes + 5));
        return mac;
    }
}

namespace Packet {

};

#endif //TRAFFIC_COLLECTOR_PROTOCOL_TYPE_H
