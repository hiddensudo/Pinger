#ifndef ICMP_H
#define ICMP_H

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "RawSocket.h"

struct ipheader {
    unsigned char iph_ihl : 4, iph_ver : 4;
    unsigned char iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_id;
    unsigned char iph_flag : 3, iph_offset : 13;
    unsigned char iph_ttl;
    unsigned char iph_protocol;
    unsigned short int iph_chksum;
    struct in_addr iph_sourceip;
    struct in_addr iph_destip;
};

struct icmpheader {
    unsigned char icmp_type;         // ICMP message type
    unsigned char icmp_code;         // Error code
    unsigned short int icmp_chksum;  // Checksum of ICMP
    unsigned short int icmp_id;      // Used for identifying request
    unsigned short int icmp_seq;     // Sequence number
};

class ICMP {
private:
    char sendBuffer[sizeof(icmpheader) + sizeof(ipheader)];

    RawSocket rawSocket;
    int icmpSocket;

    int sentedPackagesCount;
    int recvPackagesCount;
    int lostPacketCount;
    int packetLoss;

    unsigned short int icmpSequence;

    struct ipheader* ip;
    struct icmpheader* icmp;
    struct sockaddr_in destInfo;
    char* destIp;
    std::string destHostname;

    unsigned short calculateChecksum(unsigned short* buffer, int length);

    void createIPHeader(char* destIp);
    void createICMPHeader();

    void createDestinationInfo();
    int sendRawPacket();

    void handleSendResult(int result);
    std::string ipToHostname();

    void handleRecvError();
    void printRecvPacketInfo(struct icmpheader *icmpRec, struct sockaddr_in senderInfo);

public:
    ICMP(char* destIp);
    virtual ~ICMP();
    void sendPacket();
    void receivePacket();

    int getSentedPackageCount();
    int getRecvPackageCount();
    int getLostPackageCount();
    int getPacketLoss();
    void setDestIp(char* destIp);

    void rebuildIcmp(char *destIp);
};

#endif  // ICMP_H