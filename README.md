# Pinger

Pinger is a console application for pinging remote hosts. It was created using the C++ programming language and Linux raw sockets.

## Sequence diagram

![seqdiagram](https://github.com/hiddensudo/Pinger/assets/111019096/5d90767a-0d84-4535-95a2-c5ab00fc8cd5)

## Requirments

```
Gnu/Linux OS (Tested on Ubuntu 23.04)
CMake 3.16+
C++17
```

## Build

To build a project, you need to run the following commands. 
First, you need to clone the repository from the GitHub:
``` bash
$ cd ~/Downloads
$ git clone git@github.com:hiddensudo/Pinger.git
$ cd Pinger
```
After that, use CMake to build the project:
``` bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## How to use

Since the project uses Raw sockets to send ICMP packets to receive them from a remote host, the application can only be run as a ```super user```, so to run it you need to write ```sudo ./pinger```

### How to ping a remote host
As soon as you launch the application, you will be asked which address you want to ping, and you will need to enter the address.
If you enter an address that does not exist, you will see a message about it and the program will end.
```
Enter host name for ping:
askdgjaskg
Error: Host name lookup failure
```
If the address is entered correctly, then you will need to enter the number of packets you want to send.
```
Enter host name for ping:
8.8.8.8
Enter number of package to sent
5
```
After sending each packet, you will receive a message with information about: the amount of data received, the addresses from which a response was received, the time between sending and receiving the packet, and the packet number. 
```
Ping 8.8.8.8 (8.8.8.8)
16 bytes from: dns.google (8.8.8.8) icmp_seq=0 time=26 ms
16 bytes from: dns.google (8.8.8.8) icmp_seq=1 time=27 ms
16 bytes from: dns.google (8.8.8.8) icmp_seq=2 time=28 ms
16 bytes from: dns.google (8.8.8.8) icmp_seq=3 time=28 ms
16 bytes from: dns.google (8.8.8.8) icmp_seq=4 time=28 ms
```
If you interrupt the application with ctrl+c, or if a specified number of packets are sent and received, you will receive detailed ping information.
```
Ping information min/max/avg:
min_ping=26ms, max_ping=28ms, avg_ping=27ms
Information about the packages:
sented_packet_count=5, recv packet count=5, packet loss=0%
```
If pinging was completed correctly, you will be able to continue pinging addresses.
```
Do you want to ping another ling? y/n
y
Enter host name for ping:
microsoft.com
Enter number of package to sent
3
Ping microsoft.com (20.70.246.20)
16 bytes from: 20.70.246.20 (20.70.246.20) icmp_seq=0 time=277 ms
16 bytes from: 20.70.246.20 (20.70.246.20) icmp_seq=1 time=278 ms
16 bytes from: 20.70.246.20 (20.70.246.20) icmp_seq=2 time=277 ms
Ping information min/max/avg:
min_ping=26ms, max_ping=278ms, avg_ping=121ms
Information about the packages:
sented_packet_count=3, recv packet count=3, packet loss=0%
Do you want to ping another link? y/n
n
```
The app has a timeout of 1 second. If the socket does not receive a response within one second, we will receive a notification in the console, and the packet itself will be considered lost.
If none of the packets return, then you will receive a message that an error has occurred and the program will terminate.
```
Enter host name for ping:
dl.nure.ua
Enter number of package to sent
3
Ping dl.nure.ua (3.71.158.53)
Error receiving packet: Resource temporarily unavailable
Error receiving packet: Resource temporarily unavailable
Error receiving packet: Resource temporarily unavailable
Something went wrong! No ping information available. packet_loss=100%
```
