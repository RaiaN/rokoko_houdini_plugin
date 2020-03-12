// Copyright Peter Leontev 2020

#ifndef _ROKOKO_DATA_PARSER_H_
#define _ROKOKO_DATA_PARSER_H_

#include <string>

#include <UT/UT_Vector3.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Array.h>

#include "PropTrackerInfo.h"
#include "ActorInfo.h"


class UT_JSONValue;
class UT_JSONValueMap;


class RokokoDataParser
{

public:
    RokokoDataParser();
    virtual ~RokokoDataParser();

public:
    void parse(const std::string& data);

    const UT_Array<PropTrackerInfo>& getPropTrackers();
    const UT_Array<ActorInfo>& getActors();

protected:
    void parseData(const UT_JSONValue* jsonValue);

    PropTrackerInfo parsePropTracker(const UT_JSONValueMap* propTrackerAsMap) const;
    void parsePropsOrTrackers(const UT_JSONValue* jsonValue);

    ActorInfo parseActor(const UT_JSONValueMap* actorAsMap) const;
    void parseActors(const UT_JSONValue* jsonValue);

    UT_Vector3 parsePosition(const UT_JSONValue* jsonValue) const;
    UT_Quaternion parseRotation(const UT_JSONValue* jsonValue) const;
    
private:
    UT_Array<PropTrackerInfo> propTrackers;
    UT_Array<ActorInfo> actors;
};

#endif