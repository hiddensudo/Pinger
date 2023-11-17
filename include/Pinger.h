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
    char* ip;

    unsigned int numberPackageForSending;

    std::vector<int> timeVector;

    ICMP icmp;

    unsigned int specifyNumberOfPackage();

    int measureTime(std::function<void()> sendAndRecv);

    char* hostNameToIp();

    void getReceiverIpAddress();

    int getMaxPing();
    int getMinPing();
    int getAvgPing();

    void skipLostPacket(int dur);

    void ensurePingDataExists();
    void displayPingInfo();
    static void signalHandler(int signal);

    bool pingAnotherLink();

    Pinger();
    Pinger(const Pinger&) = delete;
    Pinger& operator=(const Pinger&) = delete;
    virtual ~Pinger() = default;
public:
    static Pinger& getInstance();

    void ping();
};

#endif  // PINGER_H