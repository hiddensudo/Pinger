#include "Pinger.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <vector>

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

double Pinger::measureTime(std::function<void()> sendAndRecv) {
    auto start = std::chrono::high_resolution_clock::now();

    sendAndRecv();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return duration.count();
}

void Pinger::ping() {
    int iter = 6;
    const char *ip = hostNameToIp();

    ICMP icmp(ip);

    std::vector<double> timeVector;

    std::cout << "Ping " << hostname << " "
              << "(" << ip << ")" << std::endl;

    while (true) {
        auto dur = measureTime([&icmp]() {
            icmp.sendPacket();
            icmp.receivePacket();
        });
        timeVector.push_back(dur);
        std::cout << dur << " ms" << std::endl;
        iter--;
        sleep(1);
    }
}
