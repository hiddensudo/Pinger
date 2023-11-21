#include "RawSocket.h"

#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

// receive std::chrono::duration/std::chrono::seconds
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
        exit(1); // If error happened it doesn't that we should exit, give it another try
    }
}

RawSocket::~RawSocket() { close(this->rawSocket); }

int RawSocket::getSocket() { return this->rawSocket; }