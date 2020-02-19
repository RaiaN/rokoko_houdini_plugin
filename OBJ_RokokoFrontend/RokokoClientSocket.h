// Copyright Peter Leontev 2020

#ifndef _ROKOKO_CLIENT_SOCKET_H
#define _ROKOKO_CLIENT_SOCKET_H


#include <UT/UT_NetSocket.h>


class RokokoClientSocket : public UT_NetSocket
{
public:
    RokokoClientSocket(const char *address, int port, bool blocking = false, int localport = -1)
        : UT_NetSocket(address, port, blocking, localport)
    {
        myIsServer = false;
    }
};

#endif