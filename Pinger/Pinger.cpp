#include "Pinger.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>

#include "ICMP.h"

void Pinger::getUserIpAddres() {
    struct ifaddrs *ifaddr;

    // Get the network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    // Iterate over the network interfaces
    iterateInterfaces(ifaddr);

    // Free the memory allocated for the network interfaces
    freeifaddrs(ifaddr);
}

void Pinger::iterateInterfaces(struct ifaddrs *ifaddr) {
    struct ifaddrs *ifa;

    // Loop over each network interface
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        int family = ifa->ifa_addr->sa_family;

        // Check if the interface is IPv4 or IPv6
        if (family == AF_INET || family == AF_INET6) {
            // Check the interface
            checkInterface(ifa, family);
        }
    }
}

// This method checks a network interface
void Pinger::checkInterface(struct ifaddrs *ifa, int family) {
    char host[NI_MAXHOST];

    // Get the name of the host
    int s = getnameinfo(ifa->ifa_addr,
                        (family == AF_INET) ? sizeof(struct sockaddr_in)
                                            : sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    if (s != 0) {
        printf("getnameinfo() failed: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    // Check if the interface is not the loopback and it's IPv4
    if (strcmp(ifa->ifa_name, "lo") != 0 && family == AF_INET) {
        // Save the IP address
        this->IPbuffer = strdup(host);  
    }
}

const char* Pinger::hostNameToIp() {
    std::cout << "Enter host name for ping: " << std::endl;
    std::string hostname;
    std::cin >> hostname;

    struct hostent* host = gethostbyname(hostname.c_str());

    if (host == NULL) {
        std::cerr << "Error: " << strerror(h_errno) << std::endl;
        exit(1);
    }

    return inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
}

void Pinger::ping() {
    getUserIpAddres();
    const char* ip = hostNameToIp();
    ICMP icmp(IPbuffer, ip);
    icmp.sendPacket();
    icmp.receivePacket();
}

void Pinger::timeCalculation() {}