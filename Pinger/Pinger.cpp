#include "Pinger.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

static constexpr uint16_t port = 3000;

void Pinger::createSocket() {
    this->pongerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->pongerSocket < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void Pinger::initializePingerAddress() {
    this->pingerAddress.sin_port = htons(port);
    this->pingerAddress.sin_family = AF_INET;
    this->pingerAddress.sin_addr.s_addr = htons(INADDR_ANY);
}

void Pinger::bindingSocket() {
    int bindingStatus =
        bind(this->pongerSocket,
             reinterpret_cast<struct sockaddr*>(&this->pingerAddress),
             sizeof(this->pingerAddress));
    if (bindingStatus < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void Pinger::listeningSocket() {
    this->size = sizeof(pingerAddress);
    std::cout << "Listening ponger..." << std::endl;
    listen(pongerSocket, 1);
}

void Pinger::acceptingClient() {
    this->pingerSocket = accept(
        this->pongerSocket,
        reinterpret_cast<struct sockaddr*>(&this->pingerAddress), &this->size);

    if (pingerSocket < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }

    keepOn = true;
    std::cout << "Accepted" << std::endl;
}

void Pinger::ping() {
    while (this->pingerSocket > 0) {
        while (this->keepOn) {
        }
    }
}

void Pinger::closeSocket() { close(this->pingerSocket); }

void Pinger::startPinger() {
    createSocket();
    initializePingerAddress();
    bindingSocket();
    listeningSocket();
    acceptingClient();
}

Pinger::Pinger() : keepOn(false) { startPinger(); }

Pinger::~Pinger() { closeSocket(); }