#include "ICMP.h"

#include <errno.h>

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
        std::cout << "Successfully sent " << result << " bytes" << std::endl;
}

void ICMP::sendPacket() {
    createDestinationInfo();
    std::cout << "Sending ICMP packet:" << std::endl;
    std::cout << "Destination IP: " << inet_ntoa(this->destInfo.sin_addr)
              << std::endl;
    std::cout << "ICMP Type: " << (int)icmp->icmp_type << std::endl;
    std::cout << "ICMP Code: " << (int)icmp->icmp_code << std::endl;
    std::cout << "ICMP Checksum: " << icmp->icmp_chksum << std::endl;
    std::cout << "ICMP ID: " << icmp->icmp_id << std::endl;
    std::cout << "ICMP Seq: " << icmp->icmp_seq << std::endl;
    int sendResult = sendRawPacket();
    handleSendResult(sendResult);
}

void ICMP::receivePacket() {
    char recvBuffer[1024] = {0};  // Buffer to store received packet
    struct sockaddr_in senderInfo;
    socklen_t senderInfoLen = sizeof(senderInfo);

    while (true) {
        // Receive packet
        int recvLen = recvfrom(icmpSocket, recvBuffer, sizeof(recvBuffer), 0,
                               (struct sockaddr *)&senderInfo, &senderInfoLen);
        if (recvLen < 0) {
            std::cout << "Error receiving packet: " << strerror(errno) << std::endl;
            return;
        }

        // Extract IP header from received packet
        struct ipheader *ip = (struct ipheader *)recvBuffer;

        // Extract ICMP header from received packet
        struct icmpheader *icmpRec =
            (struct icmpheader *)(recvBuffer + (ip->iph_ihl * 4));

        // Check if received packet is ICMP echo reply
        if (icmpRec->icmp_type == 0) {
            //std::cout << "Ping: "
            //          << inet_ntoa(senderInfo.sin_addr) << std::endl;
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

ICMP::ICMP(const char *destIp)
    : destIp(destIp), icmpSocket(rawSocket.getSocket()), icmpSequence(0) {
    memset(buffer, 0, sizeof(buffer));
    createICMPHeader();
}

ICMP::~ICMP() { close(icmpSocket); }