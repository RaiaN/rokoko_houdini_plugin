// Copyright Peter Leontev 2020

#ifndef _ROKOKO_SOCKET_READER_H_
#define _ROKOKO_SOCKET_READER_H_

#include <string>


class UT_NetSocket;


class RokokoSocketReader
{

public:
    RokokoSocketReader(const std::string& inIp, int inPort);
    virtual ~RokokoSocketReader();

public:
    bool read(std::string& outBuffer);
    void setIpAndPort(const std::string& inIp, int inPort);

private:
    void createClientSocket();
    bool openClientSocket();
    void destroyClientSocket();

private:
    UT_NetSocket* socket;

    std::string ip;
    int port;
};

#endif