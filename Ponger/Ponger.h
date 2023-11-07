#ifndef PONGER_H
#define PONGER_H

#include <arpa/inet.h>

#include <string>

class Ponger {
private:
    int pongerSocket;

    const char* ip;
    uint16_t port;

    bool keepOn;

    struct sockaddr_in pingerAddress;

    void createSocket();
    void initializePongerAddress();
    void connectingToPinger();

    void closeSocket();

public:
    Ponger(const char* ip, uint16_t port);
    ~Ponger();

    void pong();
};

#endif //PONGER_H