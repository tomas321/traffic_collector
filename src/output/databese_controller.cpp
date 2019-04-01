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

DatabaseController::DatabaseController(uint16_t port, const char *host) : port(port) {
    this->host = resolve_hostname(host);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE, (void *) &flag, sizeof(flag)); // set keep-alive connection
    initialize();
}

char *DatabaseController::resolve_hostname(const char *hostname) {
    struct hostent *host_entry;

    host_entry = gethostbyname(hostname);
    if (host_entry == nullptr) {
        throw SocketError("Cannot resolve provided destination hostname");
    }
    char *resolved_host = inet_ntoa(*((struct in_addr **) host_entry->h_addr_list)[0]);
    Logging::log(debug, "resolved '" + string(hostname) + "' to '" + string(resolved_host) + "'");
    return resolved_host;
}

int DatabaseController::initialize() {
    struct sockaddr_in host_socket;
    
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

DatabaseController::~DatabaseController() {
    close(socket_fd);
    Logging::log(warning, "SOCKET closed!");
}

int DatabaseController::send(const char *data) {
//    Logging::log(debug, "attempting to send to " + get_host());
    ssize_t bytes;
    if ((bytes = write(socket_fd, data, strlen(data))) == -1) {
        throw SocketError("Failed to write data: " + string(strerror(errno)));
    }
    return int(bytes);
}

std::string DatabaseController::get_host() {
    return string(host) + ":" + to_string(port);
}