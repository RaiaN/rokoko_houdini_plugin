// Copyright Peter Leontev 2020

#include "RokokoSocketReader.h"

#include <UT/UT_NetSocket.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_NetSocket.h>

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
    if (!socket)
    {
        return false;
    }

    if (!openClientSocket())
    {
        return false;
    }

    UT_WorkBuffer buffer;
    const int readResult = socket->read(buffer);
    if (readResult != UT_NetSocket::UT_CONNECT_SUCCESS)
    {
        return false;
    }

    outBuffer.assign(buffer.toStdString());
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
    socket = UT_NetSocket::newSocketFromAddr(ip.c_str(), port);
}

bool RokokoSocketReader::openClientSocket()
{
    if (socket && socket->isConnected())
    {
        return true;
    }

    return socket->connect() == UT_NetSocket::UT_CONNECT_SUCCESS;
}

void RokokoSocketReader::destroyClientSocket()
{
    if (socket)
    {
        socket->close();
        delete socket;

        socket = nullptr;
    }
}
