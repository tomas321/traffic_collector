//
// Created by tomas on 01/03/19.
//

#ifndef TRAFFIC_COLLECTOR_DATABESE_CONTROLLER_H
#define TRAFFIC_COLLECTOR_DATABESE_CONTROLLER_H

#include <cstdint>
#include "configuration.h"

#define RECONNECT_TRYING 3

class DatabaseController {
private:
    int socket_fd;
    uint16_t port;
    char *resolved_host;
    char *host;

    /**
     * Resolve host to IP address.
     *
     * @param hostname Hostname or IP address.
     * @return IP address string.
     */
    char *resolve_hostname(const char *hostname);

    /**
     * Initilaizes the socket fd and connects to given destination socket.
     *
     * @return 0 on success, else raises errors.
     */
    int initialize();

    /**
     * Attempt to reconnect to socket.
     *
     * @param data Data to be resent if reconnect succeeds.
     * @return 0 on success, else return 1.
     */
    int reconnect(const char *data);

public:
    /**
     * Create database connection.
     *
     * @param port Destination port.
     * @param host Destination host.
     */
    DatabaseController(uint16_t port, const char *host);

    /**
     * Create database connection from settings object.
     *
     * @param settings `database_settings` object.
     */
    DatabaseController(database_settings settings);

    /**
     * Destructor closes the socket connection
     */
    ~DatabaseController();

    /**
     * Send data through socket.
     *
     * @param data Json string data of single packet.
     * @return Number of bytes sent.
     */
    int send(const char *data);

    std::string get_host();
};


#endif //TRAFFIC_COLLECTOR_DATABESE_CONTROLLER_H
