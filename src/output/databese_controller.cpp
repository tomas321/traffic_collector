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

DatabaseController::DatabaseController(database_settings settings) : DatabaseController(settings.beats_port,
                                                                                        settings.beats_host.c_str()) {}

DatabaseController::DatabaseController(uint16_t port, const char *host) {
    this->host = const_cast<char *>(host);
    this->port = port;
    this->resolved_host = resolve_hostname(host);

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
    char *resolv_host = inet_ntoa(*((struct in_addr **) host_entry->h_addr_list)[0]);
    Logging::log(debug, "resolved '" + string(hostname) + "' to '" + string(resolv_host) + "'");
    return resolv_host;
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
    if (inet_pton(AF_INET, resolved_host, &host_socket.sin_addr) <= 0) {
        throw SocketError("Bad IP address");
    }

    Logging::log(debug, "socket connect to host " + get_host());
    if (connect(socket_fd, (struct sockaddr *) &host_socket, sizeof(struct sockaddr_in)) == -1) {
        throw SocketError("Failed to connect: " + string(strerror(errno)));
    }

    Logging::log(info, "Successfully connected to database socket " + get_host());

    return 0;
}

int DatabaseController::reconnect(const char *data) {
    struct sockaddr_in host_socket;

    Logging::log(info, "Attempting to reconnect socket");
    this->resolved_host = resolve_hostname(host);

    for (int i = RECONNECT_TRYING; i > 0; --i) {
        if (socket_fd) close(socket_fd); // start anew each time -> close file descriptor

        Logging::log(debug, "socket reconnect to host " + get_host() + " attempt: " + to_string(i));
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        int flag = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE, (void *) &flag, sizeof(flag));
        if (socket_fd == -1) {
            Logging::log(debug, "socket is null");
            continue;
        }
        bzero(&host_socket, sizeof(struct sockaddr_in));
        host_socket.sin_family = AF_INET;
        host_socket.sin_port = htons(port);
        inet_pton(AF_INET, resolved_host, &host_socket.sin_addr); // no errors since it passed the first time

        if (connect(socket_fd, (struct sockaddr *) &host_socket, sizeof(struct sockaddr_in)) == -1) {
            Logging::log(debug, "socket connect failed: " + string(strerror(errno)));
            continue;
        }
        return write(socket_fd, data, strlen(data));
    }
    return -1;
}

DatabaseController::~DatabaseController() {
    close(socket_fd);
    Logging::log(warning, "SOCKET closed!");
}

int DatabaseController::send(const char *data) {
    ssize_t bytes;
    if ((bytes = write(socket_fd, data, strlen(data))) == -1) {
        Logging::log(warning, "Failed to write data: " + string(strerror(errno)));
        if ((bytes = reconnect(data)) == -1)
            throw SocketError("Failed to reconnect socket: " + string(strerror(errno)));
        Logging::log(info, "Successfully reconnected socket. Data sent.");
        return int(bytes); // wrote 0 bytes of data. Next write should work
    }

    return int(bytes);
}

std::string DatabaseController::get_host() {
    return string(resolved_host) + ":" + to_string(port);
}