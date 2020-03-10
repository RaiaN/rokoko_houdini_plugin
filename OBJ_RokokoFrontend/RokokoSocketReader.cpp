// Copyright Peter Leontev 2020

#include "RokokoSocketReader.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>

#include <UT/UT_NetSocket.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_NetSocket.h>


#pragma comment( lib, "ws2_32.lib")



class WSASession
{
public:
    WSASession()
    {
        bIsValid = (WSAStartup(MAKEWORD(2, 2), &data) == 0);
    }
    ~WSASession()
    {
        WSACleanup();
    }

    bool IsValid() const
    {
        return bIsValid;
    }

private:
    WSAData data;

    bool bIsValid;
};

class UDPSocket
{
public:
    UDPSocket(unsigned short port)
    {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        bIsValid = (sock != INVALID_SOCKET);
        if (!bIsValid)
        {
            return;
        }

        Bind(port);
    }

    ~UDPSocket()
    {
        closesocket(sock);
    }

    void Bind(unsigned short port)
    {
        sockaddr_in add;
        add.sin_family = AF_INET;
        add.sin_addr.s_addr = htonl(INADDR_ANY);
        add.sin_port = htons(port);

        const int ret = bind(sock, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));

        bIsValid = (ret >= 0);
    }

    void RecvFrom(char* buffer, int len, int flags = 0)
    {
        fd_set read_s;
        timeval time_out; 

        FD_ZERO(&read_s); 
        FD_SET(sock, &read_s);
        time_out.tv_sec = 0;
        time_out.tv_usec = 0; 

        const int socketsNum = select(0, &read_s, NULL, NULL, &time_out);
        if (socketsNum != SOCKET_ERROR && FD_ISSET(sock, &read_s))
        {
            sockaddr_in from;
            int size = sizeof(from);
            const int bytesReceived = recvfrom(sock, buffer, len, flags, reinterpret_cast<SOCKADDR*>(&from), &size);
            if (bytesReceived >= 0)
            {
                buffer[bytesReceived] = 0;
            }
        }
        else
        {
            buffer[0] = 0;
        }
    }

    bool IsValid() const
    {
        return bIsValid;
    }

private:
    SOCKET sock;

    bool bIsValid;
};


RokokoSocketReader::RokokoSocketReader(const std::string& inIp, int inPort) : ip(inIp), port(inPort)
{
    createClientSocket(); 
}

RokokoSocketReader::~RokokoSocketReader()
{
    destroyClientSocket();
}

bool RokokoSocketReader::read(std::string& outBuffer)
{
    if (!udpSocket)
    {
        return false;
    }

    char buffer[1024 * 100];
    udpSocket->RecvFrom(buffer, sizeof(buffer));

    outBuffer = buffer;
    return true;
}

void RokokoSocketReader::setIpAndPort(const std::string& inIp, int inPort)
{
    ip = inIp;
    port = inPort;
    
    reset();
}

void RokokoSocketReader::reset()
{
    destroyClientSocket();
    createClientSocket();
}

void RokokoSocketReader::createClientSocket()
{
    udpSocket = new UDPSocket(port);
    session = new WSASession();
}


void RokokoSocketReader::destroyClientSocket()
{
    if (udpSocket)
    {
        delete udpSocket;

        udpSocket = nullptr;
    }

    if (session)
    {
        delete session;

        session = nullptr;
    }
}
