#include "Pinger.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

char *Pinger::hostNameToIp() {
    std::cout << "Enter host name for ping: " << std::endl;
    std::cin >> hostname;

    struct hostent *host = gethostbyname(hostname.c_str());

    if (host == NULL) {
        std::cerr << "Error: " << hstrerror(h_errno) << std::endl;
        exit(1);
    }

    return inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
}

unsigned int Pinger::specifyNumberOfPackage() {
    unsigned int num;
    std::cout << "Enter number of package to sent" << std::endl;

    while (true) {
        if (std::cin >> num) {
            break;
        } else {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return num;
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

void Pinger::skipLostPacket(int dur) {
    if (dur <= 1000) {
        this->timeVector.push_back(dur);
        std::cout << dur << " ms" << std::endl;
    }
}

void Pinger::signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\b\b \b\b"; // remove ^C
        Pinger::getInstance().displayPingInfo();
        exit(signal);
    }
}

bool Pinger::pingAnotherLink() {
    std::string yn;
    std::cout << "Do you want to ping another link? y/n" << std::endl;

    while (true) {
        std::cin >> yn;

        if (yn != "y" && yn != "n")
            std::cout << "Invalid input. Please enter y/n" << std::endl;

        if (yn == "y") {
            ip = hostNameToIp();
            numberPackageForSending = specifyNumberOfPackage();
            icmp.rebuildIcmp(ip);
            break;
        } 

        if(yn == "n") break;
    }

    return yn == "y" ? true : false;
}

void Pinger::ping() {
    while (true) {
        std::cout << "Ping " << hostname << " "
                  << "(" << ip << ")" << std::endl;

        signal(SIGINT, Pinger::signalHandler);

        for (int i = 0; i < numberPackageForSending; i++) {
            auto dur = measureTime([&]() {
                this->icmp.sendPacket();
                this->icmp.receivePacket();
            });

            skipLostPacket(dur);
            sleep(1);
        }
        displayPingInfo();

        if (!pingAnotherLink()) return;
    }
}

Pinger::Pinger()
    : timeVector(),
      ip(hostNameToIp()),
      icmp(ip),
      numberPackageForSending(specifyNumberOfPackage()) {}

Pinger &Pinger::getInstance() {
    static Pinger pinger;
    return pinger;
}