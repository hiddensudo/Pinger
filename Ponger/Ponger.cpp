#include "Ponger.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

void Ponger::createSocket() {
    this->pongerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->pongerSocket < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void Ponger::initializePongerAddress() {
    this->pingerAddress.sin_port = htons(this->port);
    this->pingerAddress.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &pingerAddress.sin_addr);
}

void Ponger::connectingToPinger() {
  if(connect(this->pongerSocket,reinterpret_cast<struct sockaddr*>(&this->pingerAddress), sizeof(this->pingerAddress)) == 0) {
        std::cout << "SERVER_MSG" << "Connection to server " << inet_ntoa(this->pingerAddress.sin_addr)
                << " with port number: " << port << std::endl;
    }
}

void Ponger::pong() {
    while(true ) {

    }
}

void Ponger::closeSocket() {
    close(this->pongerSocket);
}

Ponger::Ponger(const char* ip, uint16_t port) : ip(ip), port(port), keepOn(false) {
    createSocket();
    initializePongerAddress();
    connectingToPinger();
}

Ponger::~Ponger() {
    closeSocket();
}