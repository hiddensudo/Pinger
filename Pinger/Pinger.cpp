#include "Pinger.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

#include "ICMP.h"

const char *Pinger::hostNameToIp() {
    std::cout << "Enter host name for ping: " << std::endl;
    std::cin >> hostname;

    struct hostent *host = gethostbyname(hostname.c_str());

    if (host == NULL) {
        std::cerr << "Error: " << strerror(h_errno) << std::endl;
        exit(1);
    }

    return inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
}

std::chrono::duration<double> Pinger::measureTime(
    std::function<void()> sendAndRecv) {
    auto start = std::chrono::high_resolution_clock::now();

    sendAndRecv();

    auto end = std::chrono::high_resolution_clock::now();

    return end - start;
}

void Pinger::ping() {
    const char *ip = hostNameToIp();

    ICMP icmp(ip);

    std::cout << "Ping " << hostname << " "
              << "(" << ip << ")" << std::endl;

    auto dur = measureTime([&icmp]() {
        icmp.sendPacket();
        icmp.receivePacket();
    });

    std ::cout << hostname << " " << dur.count() << " seconds" << std::endl;
}
