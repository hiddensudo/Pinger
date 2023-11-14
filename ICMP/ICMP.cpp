#include "ICMP.h"

#include <errno.h>
#include <netdb.h>

#include <chrono>
#include <iostream>
#include <thread>

void ICMP::createICMPHeader() {
    icmp = (struct icmpheader *)(buffer + sizeof(struct ipheader));
    icmp->icmp_id = rand();
    icmp->icmp_code = 0;
    icmp->icmp_seq = icmpSequence++;
    icmp->icmp_type = 8;    // ICMP Echo Request
    icmp->icmp_chksum = 0;  // Start checksum = 0
    icmp->icmp_chksum =     // Calculate checksum by method
        calculateChecksum((unsigned short *)icmp, sizeof(struct icmpheader));
}

std::string ICMP::ipToHostname() {
    struct sockaddr_in sockAddr;
    char host[1024];
    char service[20];

    sockAddr.sin_family = AF_INET;
    inet_pton(AF_INET, this->destIp, &sockAddr.sin_addr);

    getnameinfo((struct sockaddr *)&sockAddr, sizeof(sockAddr), host,
                sizeof(host), service, sizeof(service), 0);

    return std::string(host);
}

void ICMP::createDestinationInfo() {
    this->destInfo.sin_family = AF_INET;
    this->destInfo.sin_addr.s_addr = inet_addr(destIp);
}

int ICMP::sendRawPacket() {
    return sendto(icmpSocket, icmp, sizeof(icmp), 0,
                  (struct sockaddr *)&this->destInfo, sizeof(this->destInfo));
}

void ICMP::handleSendResult(int result) {
    if (result < 0)
        std::cout << "Error: " << strerror(errno) << std::endl;
    else
        sentedPackagesCount++;
}

void ICMP::sendPacket() {
    createICMPHeader();
    createDestinationInfo();
    int sendResult = sendRawPacket();
    handleSendResult(sendResult);
}

void ICMP::receivePacket() {
    char recvBuffer[1024] = {0};  // Buffer to store received packet
    struct sockaddr_in senderInfo;
    socklen_t senderInfoLen = sizeof(senderInfo);

    rawSocket.setTimeout(1);

    while (true) {
        // Receive packet
        int recvLen = recvfrom(icmpSocket, recvBuffer, sizeof(recvBuffer), 0,
                               (struct sockaddr *)&senderInfo, &senderInfoLen);
        if (recvLen < 0) {
            std::cout << "Error receiving packet: " << strerror(errno)
                      << std::endl;
            lostPacketCount++;
            return;
        }

        // Extract IP header from received packet
        struct ipheader *ip = (struct ipheader *)recvBuffer;

        // Extract ICMP header from received packet
        struct icmpheader *icmpRec =
            (struct icmpheader *)(recvBuffer + (ip->iph_ihl * 4));

        // Check if received packet is ICMP echo reply
        if (icmpRec->icmp_type == 0 && icmpRec->icmp_seq == icmp->icmp_seq) {
            std::cout << sizeof(icmpRec) + sizeof(ip) << " bytes"
                      << " from: " << destHostname << " ("
                      << inet_ntoa(senderInfo.sin_addr) << ")"
                      << " icmp_seq=" << icmpRec->icmp_seq << " time=" ;
            recvPackagesCount++;
            break;
        }
    }
}

unsigned short ICMP::calculateChecksum(unsigned short *buffer, int length) {
    int nleft = length;
    int sum = 0;
    unsigned short *w = buffer;
    unsigned short answer = 0;

    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}

int ICMP::getSentedPackageCount() { return this->sentedPackagesCount; }

int ICMP::getRecvPackageCount() { return this->recvPackagesCount; }

int ICMP::getLostPackageCount() { return this->lostPacketCount; }

int ICMP::getPacketLoss() {
    return (this->lostPacketCount / this->sentedPackagesCount) * 100;
}

ICMP::ICMP(const char *destIp)
    : destIp(destIp),
      icmpSocket(rawSocket.getSocket()),
      icmpSequence(0),
      sentedPackagesCount(0),
      recvPackagesCount(0),
      lostPacketCount(0), destHostname(ipToHostname()) {
    memset(buffer, 0, sizeof(buffer));
}

ICMP::~ICMP() { close(icmpSocket); }