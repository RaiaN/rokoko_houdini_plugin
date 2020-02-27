// Copyright Peter Leontev 2020

#include "RokokoDataParser.h"

#include <UT/UT_StringHolder.h>
#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONValue.h>
#include <UT/UT_JSONValueMap.h>
#include <UT/UT_JSONValueArray.h>


RokokoDataParser::RokokoDataParser()
{
    
}

RokokoDataParser::~RokokoDataParser()
{

}

const UT_Array<PropTrackerInfo>& RokokoDataParser::parse(const std::string& data)
{
    propTrackers.clear();
    
    UT_AutoJSONParser parser(data.c_str(), data.length());

    UT_JSONValue value;
    if (value.parseValue(parser))
    {
        parseData(&value);
    }

    return propTrackers;
}

void RokokoDataParser::parseData(const UT_JSONValue* jsonValue)
{
    if (!jsonValue)
    {
        return;
    }

    static const UT_StringRef PROPS_KEY("props");
    static const UT_StringRef TRACKERS_KEY("trackers");
    static const UT_StringRef FACES_KEY("faces");


    UT_JSONValueMap* jsonMap = jsonValue->getMap();
    if (!jsonMap)
    {
        // TODO: SET ERROR
        return;
    }

    UT_JSONValue* props = jsonMap->get(PROPS_KEY);
    if (props)
    {
        parsePropsOrTrackers(props);
    }

    UT_JSONValue* trackers = jsonMap->get(TRACKERS_KEY);
    if (trackers)
    {
        parsePropsOrTrackers(trackers);
    }

    // TODO: faces
}

void RokokoDataParser::parsePropsOrTrackers(const UT_JSONValue* jsonValue)
{
    if (!jsonValue)
    {
        return;
    }

    static const UT_StringRef NAME_KEY("name");
    static const UT_StringRef POSITION_KEY("position");
    static const UT_StringRef ROTATION_KEY("rotation");


    UT_JSONValueArray* objects = jsonValue->getArray();
    if (!objects)
    {
        return;
    }

    for (int objInd = 0; objInd < objects->size(); ++objInd)
    {
        UT_JSONValue* obj = objects->get(objInd);
        if (obj)
        {
            UT_JSONValueMap* objAsMap = obj->getMap();
            if (objAsMap)
            {
                PropTrackerInfo objInfo;

                UT_JSONValue* nameValue = objAsMap->get(NAME_KEY);
                if (nameValue)
                {
                    objInfo.name = nameValue->getS();
                }

                objInfo.position = parsePosition(objAsMap->get(POSITION_KEY));
                objInfo.rotation = parseRotation(objAsMap->get(ROTATION_KEY));

                propTrackers.append(objInfo);
            }
        }
    }
}


UT_Vector3 RokokoDataParser::parsePosition(const UT_JSONValue* jsonValue)
{
    UT_Vector3 position(0.0, 0.0, 0.0);

    UT_JSONValueMap* positionValueMap = jsonValue->getMap();
    if (positionValueMap)
    {
        const UT_JSONValue* xValue = positionValueMap->get("x");
        const UT_JSONValue* yValue = positionValueMap->get("y");
        const UT_JSONValue* zValue = positionValueMap->get("z");

        if (xValue)
        {
            position.x() = xValue->getF();
        }

        if (yValue)
        {
            position.y() = yValue->getF();
        }

        if (zValue)
        {
            position.z() = zValue->getF();
        }
    }

    return position;
}

UT_Quaternion RokokoDataParser::parseRotation(const UT_JSONValue* jsonValue)
{
    UT_Quaternion rotation;

    UT_JSONValueMap* rotationValueMap = jsonValue->getMap();
    if (rotationValueMap)
    {
        const UT_JSONValue* xValue = rotationValueMap->get("x");
        const UT_JSONValue* yValue = rotationValueMap->get("y");
        const UT_JSONValue* zValue = rotationValueMap->get("z");
        const UT_JSONValue* wValue = rotationValueMap->get("w");

        if (xValue)
        {
            rotation.x() = xValue->getF();
        }

        if (yValue)
        {
            rotation.y() = yValue->getF();
        }

        if (zValue)
        {
            rotation.z() = zValue->getF();
        }

        if (wValue)
        {
            rotation.w() = wValue->getF();
        }
    }

    return rotation;
}

