#ifndef PINGER_H
#define PINGER_H

#include <arpa/inet.h>
#include <sys/socket.h>

class Pinger {
private:
    char *IPbuffer;
    char hostbuffer[256];
    struct hostent *host_entry;
    int hostname;

    void timeCalculation();
    
    void getUserIpAddres();
    void iterateInterfaces(struct ifaddrs *ifaddr);
    void checkInterface(struct ifaddrs *ifa, int family);

    void getReceiverIpAddress();

public:
    Pinger() = default;
    virtual ~Pinger() = default;

    void ping();
};

#endif  // PINGER_H