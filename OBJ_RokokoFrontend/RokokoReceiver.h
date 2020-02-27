// Copyright Peter Leontev 2020

#ifndef _ROKOKO_RECEIVER_H_
#define _ROKOKO_RECEIVER_H_

#include <string>

#include <FS/FS_EventGenerator.h>



class RokokoSocketReader;
class RokokoDataParser;


class RokokoReceiver : public FS_EventGenerator 
{

public:
    RokokoReceiver(const std::string& inIp, int inPort, int inRate);
    virtual ~RokokoReceiver();

    virtual const char* getClassName() const override;
    virtual int getPollTime() override;
    virtual int processEvents() override;

public:
    void start();
    void stop();
    void setUpdateRate(int inRate);
    void setIpAndPort(const std::string& inIp, int inPort);

private:
    bool bInstalled = false;

    RokokoSocketReader* socketReader;
    RokokoDataParser* dataParser;

    int rate;
};

#endif