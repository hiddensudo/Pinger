#include "RawSocket.h"

#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

void RawSocket::setTimeout(int seconds) {
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    if (setsockopt(rawSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                   sizeof(timeout)) < 0) {
        std::cout << "Error: " << strerror(errno) << std::endl;
    }
}

RawSocket::RawSocket() {
    this->rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (this->rawSocket == -1) {
        std::cout << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

RawSocket::~RawSocket() { close(this->rawSocket); }

int RawSocket::getSocket() { return this->rawSocket; }