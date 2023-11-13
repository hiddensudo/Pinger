#ifndef RAWSOCKET_H
#define RAWSOCKET_H

class RawSocket
{
private:
    int rawSocket;
public:
    RawSocket();
    virtual ~RawSocket();

    int getSocket();
};

#endif //RAWSOCKET_H