// Copyright Peter Leontev 2020

#ifndef _ROKOKO_SOCKET_READER_H_
#define _ROKOKO_SOCKET_READER_H_

#include <string>


class UDPSocket;
class WSASession;


class RokokoSocketReader
{

public:
    RokokoSocketReader(const std::string& inIp, int inPort);
    virtual ~RokokoSocketReader();

public:
    bool read(std::string& outBuffer);
    void setIpAndPort(const std::string& inIp, int inPort);
    void reset();

private:
    void createClientSocket();
    void destroyClientSocket();

private:
    UDPSocket* udpSocket;
    WSASession* session;

    std::string ip;
    int port;
};

#endif