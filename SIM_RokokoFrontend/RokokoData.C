// Copyright Peter Leontev 2020

#include "RokokoData.h"

#include <string.h>

#include <UT/UT_WorkBuffer.h>
#include <UT/UT_NetSocket.h>
#include <UT/UT_StringHolder.h>

#include "RokokoClientSocket.h"


RokokoData::RokokoData()
{
    createClientSocket();

    
}

RokokoData::~RokokoData()
{
    destroyClientSocket();

}

bool RokokoData::readData()
{
    if (!myClientSocket)
    {
        return false;
    }

    if (!openClientSocket())
    {
        return false;
    }

    UT_WorkBuffer buffer;
    const int readResult = myClientSocket->read(buffer);

    if (readResult == UT_NetSocket::UT_CONNECT_SUCCESS)
    {
        UT_AutoJSONParser parser(buffer.buffer(), buffer.length());

        UT_JSONValue value;
        if (value.parseValue(parser))
        {
            parseData(&value);
        }
    }

    return true;
}

const UT_Array<PropTrackerInfo>& RokokoData::getPropTrackers() const
{
    return propTrackers;
}


void RokokoData::parseData(const UT_JSONValue* jsonValue)
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

    propTrackers.clear();

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

void RokokoData::parsePropsOrTrackers(const UT_JSONValue* jsonValue)
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


UT_Vector3 RokokoData::parsePosition(const UT_JSONValue* jsonValue)
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

UT_Quaternion RokokoData::parseRotation(const UT_JSONValue* jsonValue)
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

void RokokoData::createClientSocket()
{
    // TODO: use Node param
    myClientSocket = new RokokoClientSocket("127.0.0.1", 11111);
}

bool RokokoData::openClientSocket()
{
    if (myClientSocket->isConnected())
    {
        return true;
    }

    return myClientSocket->connect() == UT_NetSocket::UT_CONNECT_SUCCESS;
}

void RokokoData::destroyClientSocket()
{
    if (myClientSocket)
    {
        myClientSocket->close();
        delete myClientSocket;

        myClientSocket = nullptr;
    }
}
