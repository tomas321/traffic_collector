//
// Created by tomas on 20/03/19.
//

#ifndef TRAFFIC_COLLECTOR_PROTOCOL_TYPE_H
#define TRAFFIC_COLLECTOR_PROTOCOL_TYPE_H

#include <cstdint>
#include <cstdlib>
#include <cstdio>

namespace IPAddress {
    /**
     * Convert IP address from bytes to string representation.
     *
     * @param bytes Unsigned 32 bit raw ip address
     * @return String IP address
     */
    inline const char *to_string(uint32_t bytes) {
        char *ip = (char *) malloc(16);
        snprintf(ip, 16, "%u.%u.%u.%u", (bytes & 0xff000000) >> 24,
                 (bytes & 0x00ff0000) >> 16,
                 (bytes & 0x0000ff00) >> 8,
                 (bytes & 0x000000ff));
        return ip;
    }
}

namespace IPv6Address {
    /**
     * Convert IPv6 address from bytes to string representation.
     *
     * @param words Array of 16bit words of raw address.
     * @return String IPv6 address
     */
    inline const char *to_string(uint16_t words[8]) {
        char *ip = (char *) malloc(40);
        snprintf(ip, 40, "%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X", words[0],
                 words[1],
                 words[2],
                 words[3],
                 words[4],
                 words[5],
                 words[6],
                 words[7]);
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
    inline const char *to_string(const uint8_t *bytes) {
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

namespace Bytes {
    /**
     * Merge 2 bytes to single 16bit word in order.
     *
     * @param left First octet of the final word.
     * @param right Second octet of the final word.
     * @return 16bit word consisting both bytes.
     */
    inline const uint16_t merge_bytes(const uint8_t left, const uint8_t right) {
        return static_cast<uint16_t>(((left & 0x00ff) << 8) | (right & 0x00ff));
    }

    /**
     * Merge 4 bytes to single 32bit word in order.
     *
     * @param one First octet of the final word.
     * @param two Second octet of the final word.
     * @param three Third octet of the final word.
     * @param four Fourth octet of the final word.
     * @return 32bit word consisting all bytes.
     */
    inline const uint32_t merge_bytes(const uint8_t one, const uint8_t two, const uint8_t three, const uint8_t four) {
        return static_cast<uint32_t>(((one & 0x000000ff) << 24) | ((two & 0x000000ff) << 16) |
                                     ((three & 0x000000ff) << 8) | (four & 0x000000ff));
    }
};

#endif //TRAFFIC_COLLECTOR_PROTOCOL_TYPE_H
