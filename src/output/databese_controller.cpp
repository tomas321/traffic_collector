//
// Created by tomas on 01/03/19.
//

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <libnet.h>

#include "databese_controller.h"
#include "exceptions.h"

using namespace std;

DatabeseController::DatabeseController(uint16_t port, char *host) : port(port), host(resolve_hostname(host)) {
    initialize();
}

char *DatabeseController::resolve_hostname(char *host) {
    struct hostent *host_entry;

    host_entry = gethostbyname(host);
    if (host_entry == nullptr) {
        throw SocketError("Cannot resolve provided destination host");
    }
    return inet_ntoa(*((struct in_addr **) host_entry->h_addr_list)[0]);
}

int DatabeseController::initialize() {
    struct sockaddr_in host_socket;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        throw SocketError("Failed to open socket: " + string(strerror(errno)));
    }

    bzero(&host_socket, sizeof(struct sockaddr_in)); // write zeros to struct attributes
    host_socket.sin_family = AF_INET; // set socket to Internet address family
    host_socket.sin_port = htons(port); // convert port number to network byte order port

    // convert host IP address to network byte order
    if (inet_pton(AF_INET, host, &host_socket.sin_addr) <= 0) {
        throw SocketError("Bad IP address");
    }

    if (connect(socket_fd, (struct sockaddr *) &host_socket, sizeof(struct sockaddr_in)) == -1) {
        throw SocketError("Failed to connect: " + string(strerror(errno)));
    }

    return 0;
}

DatabeseController::~DatabeseController() {
    close(socket_fd);
}

int DatabeseController::send(const char *data) {
    if (write(socket_fd, data, strlen(data)) == -1) {
        throw SocketError("Failed to write data: " + string(strerror(errno)));
    }
}
