#ifndef RAWSOCKET_H
#define RAWSOCKET_H

class RawSocket
{
private:
    int rawSocket;
public:
    RawSocket();
    virtual ~RawSocket();
    void setTimeout(int seconds);
    int getSocket();
};

#endif //RAWSOCKET_H