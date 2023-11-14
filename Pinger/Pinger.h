#ifndef PINGER_H
#define PINGER_H

#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include <chrono>
#include <functional>
#include <string>
#include <vector>
#include "ICMP.h"

class Pinger {
private:
    std::string hostname;
    const char *ip = hostNameToIp();
    std::vector<int> timeVector;

    ICMP icmp;

    int measureTime(std::function<void()> sendAndRecv);

    const char *hostNameToIp();

    void getReceiverIpAddress();

    int getMaxPing();
    int getMinPing();
    int getAvgPing();

    void recvOrLoss(int dur);

    void ensurePingDataExists();
    void displayPingInfo();
    static void signalHandler(int signal);

public:
    Pinger();
    virtual ~Pinger() = default;

    void ping();
};

#endif  // PINGER_H