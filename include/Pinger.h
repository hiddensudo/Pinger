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
    char* ip; // Better to save it in string

    unsigned int numberPackageForSending; // unnecessary

    std::vector<int> timeVector; // name could be better

    ICMP icmp;

    unsigned int specifyNumberOfPackage();

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
    Pinger& operator=(const Pinger&) = delete; // You need to delete move contructors as well
    virtual ~Pinger() = default; // Why is it virtual?
public:
    static Pinger& getInstance();

    void ping();
};

#endif  // PINGER_H