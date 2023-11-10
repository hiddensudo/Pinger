#include "ICMP.h"

#include <errno.h>

#include <iostream>

void ICMP::createIPHeader(const char *sourceIP, const char *destIP) {
    this->ip = (struct ipheader *)buffer;
    this->ip->iph_ver = 4;                                // IPv4
    this->ip->iph_ihl = 5;                                // Internet Header Length
    this->ip->iph_ttl = 20;                               // Time To Live
    this->ip->iph_sourceip.s_addr = inet_addr(sourceIP);  // Sender Ip -> sourceIP
    this->ip->iph_destip.s_addr = inet_addr(destIP);      // Recipient Ip -> destIP
    this->ip->iph_protocol = IPPROTO_ICMP;                // ICMP
    this->ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct icmpheader));
}

void ICMP::createICMPHeader() {
    this->icmp = (struct icmpheader *)(buffer + sizeof(struct ipheader));
    this->icmp->icmp_type = 8;    // ICMP Echo Request
    this->icmp->icmp_chksum = 0;  // Start checksum = 0
    this->icmp->icmp_chksum =     // Calculate checksum by methon
        calculateChecksum((unsigned short *)icmp, sizeof(struct icmpheader));
}

void ICMP::creeateSocket() {
    this->icmpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmpSocket == -1) {
        std::cout << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void ICMP::setSocketOptins() {
    int enable = 1;
    if (setsockopt(this->icmpSocket, IPPROTO_IP, IP_HDRINCL, &enable,
                   sizeof(enable)) == -1) {
        std::cout << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void ICMP::createDestinationInfo() {
    this->destInfo.sin_family = AF_INET;
    this->destInfo.sin_addr = this->ip->iph_destip;
}

int ICMP::sendRawPacket() {
    return sendto(icmpSocket, ip, ntohs(ip->iph_len), 0,
                  (struct sockaddr *)&this->destInfo, sizeof(this->destInfo));
}

void ICMP::handleSendResult(int result) {
    if (result == -1)
        std::cout << "Error" << strerror(errno) << std::endl;
    else
        std::cout << "Successfully sent " << result << " bytes" << std::endl;
}

void ICMP::sendPacket() {
    createDestinationInfo();
    int sendResult = sendRawPacket();
    handleSendResult(sendResult);
}

void ICMP::receivePacket() {
    char recvBuffer[1024];
    struct sockaddr_in senderInfo;
    socklen_t senderInfoLen = sizeof(senderInfo);
    int recvResult = recvfrom(icmpSocket, recvBuffer, sizeof(recvBuffer), 0,
                              (struct sockaddr *)&senderInfo, &senderInfoLen);
    if (recvResult == -1) {
        std::cout << "Error: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Received " << recvResult << " bytes" << std::endl;
        struct ipheader *ip = (struct ipheader *)recvBuffer;
        struct icmpheader *icmp = (struct icmpheader *)(recvBuffer + sizeof(struct ipheader));
        std::cout << "Sender IP: " << inet_ntoa(ip->iph_sourceip) << std::endl;
        std::cout << "ICMP Type: " << (int)icmp->icmp_type << std::endl;
        std::cout << "ICMP ID: " << icmp->icmp_id << std::endl;
        std::cout << "ICMP Sequence: " << icmp->icmp_seq << std::endl;
        std::cout << "ICMP Timestamp: " << icmp->icmp_timestamp << std::endl;
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

ICMP::ICMP(const char *sourceIP, const char *destIP) {
    memset(buffer, 0, 1000);
    std::cout << "Dest ip: " << destIP << std::endl;
    std::cout << "Source ip: " <<  sourceIP << std::endl;
    createIPHeader(sourceIP, destIP);
    createICMPHeader();
    creeateSocket();
    setSocketOptins();
}

ICMP::~ICMP() { close(icmpSocket); }