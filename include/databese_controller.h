//
// Created by tomas on 01/03/19.
//

#ifndef TRAFFIC_COLLECTOR_DATABESE_CONTROLLER_H
#define TRAFFIC_COLLECTOR_DATABESE_CONTROLLER_H

#include <cstdint>

class DatabeseController {
private:
    int socket_fd;
    uint16_t port;
    char *host;

    /**
     * Resolve host to IP address.
     *
     * @param host Hostname or IP address.
     * @return IP address string.
     */
    char *resolve_hostname(char *host);

    /**
     * Initilaizes the socket fd and connects to given destination socket.
     *
     * @return 0 on success, else raises errors.
     */
    int initialize();

public:
    /**
     * Create database connection.
     *
     * @param port Destination port.
     * @param host Destination host.
     */
    DatabeseController(uint16_t port, char *host);

    /**
     * Destructor closes the socket connection
     */
    ~DatabeseController();

    /**
     * Send data through socket.
     *
     * @param data Json string data of single packet.
     * @return 0 on success.
     */
    int send(char *data);
};


#endif //TRAFFIC_COLLECTOR_DATABESE_CONTROLLER_H
