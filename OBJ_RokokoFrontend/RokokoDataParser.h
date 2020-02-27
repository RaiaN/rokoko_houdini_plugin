// Copyright Peter Leontev 2020

#ifndef _ROKOKO_DATA_PARSER_H_
#define _ROKOKO_DATA_PARSER_H_

#include <string>

#include <UT/UT_Vector3.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Array.h>

#include "PropTrackerInfo.h"


class UT_JSONValue;


class RokokoDataParser
{

public:
    RokokoDataParser();
    virtual ~RokokoDataParser();

public:
    const UT_Array<PropTrackerInfo>& parse(const std::string& data);

protected:
    void parseData(const UT_JSONValue* jsonValue);
    void parsePropsOrTrackers(const UT_JSONValue* jsonValue);
    UT_Vector3 parsePosition(const UT_JSONValue* jsonValue);
    UT_Quaternion parseRotation(const UT_JSONValue* jsonValue);
    
private:
    UT_Array<PropTrackerInfo> propTrackers;
};

#endif