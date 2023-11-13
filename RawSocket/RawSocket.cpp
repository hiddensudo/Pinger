#include "RawSocket.h"

#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

RawSocket::RawSocket() {
    this->rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (this->rawSocket == -1) {
        std::cout << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

RawSocket::~RawSocket() { close(this->rawSocket); }

int RawSocket::getSocket() { return this->rawSocket; }