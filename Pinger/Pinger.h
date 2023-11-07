#ifndef PINGER_H
#define PINGER_H

#include <arpa/inet.h>
#include <sys/socket.h>

class Pinger {
private:
    int pingerSocket;
    int pongerSocket;

    struct sockaddr_in pingerAddress;
    socklen_t size;

    bool keepOn;

    void createSocket();
    void initializePingerAddress();
    void bindingSocket();
    void listeningSocket();
    void acceptingClient();

    void startPinger();

    void closeSocket();
public:
    Pinger();
    ~Pinger();

    void ping();
};

#endif  // PINGER_H