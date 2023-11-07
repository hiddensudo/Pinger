#include "Ponger.h"

#include <signal.h>

#include <iostream>

std::string enterIp(std::string ip) {
    std::cout << "Enter ip address" << std::endl;
    std::cin >> ip;
    return ip;
}

uint16_t enterPort(uint16_t port) {
    std::cout << "Enter port" << std::endl;
    std::cin >> port;
    return port;
}

int main() {
    std::string ip;
    uint16_t port;

    enterIp(ip);
    enterPort(port);


    Ponger ponger(ip.c_str(), port);
    ponger.pong();

    return 0;
}
