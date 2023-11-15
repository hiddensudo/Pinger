#include "Pinger.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

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

int Pinger::specifyNumberOfPackage() {
    std::cout << "Enter cout of package to sent" << std::endl;
    std::cin >> countPackageForSending;
    return countPackageForSending;
}

int Pinger::measureTime(std::function<void()> sendAndRecv) {
    auto start = std::chrono::high_resolution_clock::now();

    sendAndRecv();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return duration.count();
}

void Pinger::ensurePingDataExists() {
    if (timeVector.empty()) {
        std::cout << "Something went wrong! No ping information available.";

        if (icmp.getSentedPackageCount() == 0)
            std::cout << " No package was sent!";
        if (icmp.getPacketLoss() == 100)
            std::cout << " packet_loss=100%" << std::endl;

        exit(1);
    }
}

int Pinger::getMinPing() {
    return *std::min_element(timeVector.begin(), timeVector.end());
}

int Pinger::getMaxPing() {
    return *std::max_element(timeVector.begin(), timeVector.end());
}

int Pinger::getAvgPing() {
    int sum = 0;
    for (auto &time : timeVector) {
        sum += time;
    }
    return sum / timeVector.size();
}

void Pinger::displayPingInfo() {
    ensurePingDataExists();
    std::cout << "Ping information min/max/avg: "
              << "\nmin_ping=" << getMinPing()
              << "ms, max_ping=" << getMaxPing()
              << "ms, avg_ping=" << getAvgPing() << "ms" << std::endl;
    std::cout << "Information about the packages:"
              << "\nsented_packet_count=" << icmp.getSentedPackageCount()
              << ", recv packet count=" << icmp.getRecvPackageCount()
              << ", packet loss=" << icmp.getPacketLoss() << "%" << std::endl;
}

void Pinger::recvOrLoss(int dur) {
    if (dur < 1000) {
        this->timeVector.push_back(dur);
        std::cout << dur << " ms" << std::endl;
    }
}

void Pinger::signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\b\b \b\b";
        Pinger::getInstance().displayPingInfo();
        exit(signal);
    }
}

void Pinger::ping() {
    std::cout << "Ping " << hostname << " "
              << "(" << ip << ")" << std::endl;

    signal(SIGINT, Pinger::signalHandler);
    while (countPackageForSending > 0) {
        auto dur = measureTime([&]() {
            this->icmp.sendPacket();
            this->icmp.receivePacket();
        });

        recvOrLoss(dur);
        countPackageForSending--;
        sleep(1);
    }
    displayPingInfo();
}

Pinger::Pinger() : timeVector(), ip(hostNameToIp()), icmp(ip), countPackageForSending(specifyNumberOfPackage()) {}

Pinger& Pinger::getInstance() {
    static Pinger pinger;
    return pinger;
}