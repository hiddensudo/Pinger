#ifndef PINGER_H
#define PINGER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <vector>
#include <string>
#include <functional>

class Pinger {
private:
    std::string hostname;

    double measureTime(std::function<void()> sendAndRecv);

    const char *hostNameToIp();
    
    void getReceiverIpAddress();

public:
    Pinger() = default;
    virtual ~Pinger() = default;

    void ping();
};

#endif  // PINGER_H