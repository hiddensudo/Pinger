#ifndef PINGER_H
#define PINGER_H

#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ICMP.h"

class Pinger {
private:
    std::string hostname;
    const char* ip = hostNameToIp();

    int countPackageForSending;
    int specifyNumberOfPackage();

    std::vector<int> timeVector;

    ICMP icmp;

    int measureTime(std::function<void()> sendAndRecv);

    const char* hostNameToIp();

    void getReceiverIpAddress();

    int getMaxPing();
    int getMinPing();
    int getAvgPing();

    void recvOrLoss(int dur);

    void ensurePingDataExists();
    void displayPingInfo();
    static void signalHandler(int signal);

    Pinger();
    Pinger(const Pinger&) = delete;
    Pinger& operator=(const Pinger&) = delete;
    virtual ~Pinger() = default;
public:

    static Pinger& getInstance();

    void ping();
};

#endif  // PINGER_H