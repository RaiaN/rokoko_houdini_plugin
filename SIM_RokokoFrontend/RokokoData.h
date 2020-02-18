// Copyright Peter Leontev 2020

#ifndef _SIM_ROKOKO_DATA_H
#define _SIM_ROKOKO_DATA_H

#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONValue.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Array.h>

#include "PropTrackerInfo.h"


class RokokoClientSocket;


class RokokoData
{

public:
    explicit RokokoData();
    virtual ~RokokoData();

public:
    bool readData();

    const UT_Array<PropTrackerInfo>& getPropTrackers() const;

protected:
    void parseData(const UT_JSONValue* jsonValue);
    void parsePropsOrTrackers(const UT_JSONValue* jsonValue);
    UT_Vector3 parsePosition(const UT_JSONValue* jsonValue);
    UT_Quaternion parseRotation(const UT_JSONValue* jsonValue);

private:
    void createClientSocket();
    bool openClientSocket();
    void destroyClientSocket();
    
private:
    RokokoClientSocket* myClientSocket;

    UT_Array<PropTrackerInfo> propTrackers;
};

#endif