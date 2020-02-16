// Copyright Peter Leontev 2020

#include "SIM_RokokoData.h"

#include <string.h>

#include <SIM/SIM_Engine.h>
#include <SIM/SIM_Options.h>
#include <SIM/SIM_Object.h>
#include <SIM/SIM_ObjectArray.h>
#include <SIM/SIM_DopDescription.h>
#include <SIM/SIM_Random.h>
#include <SIM/SIM_RandomTwister.h>
#include <SIM/SIM_Position.h>
#include <SIM/SIM_PRMShared.h>
#include <SIM/SIM_Guide.h>
#include <SIM/SIM_GuideShared.h>
#include <GU/GU_Detail.h>
#include <GU/GU_PrimPart.h>
#include <GU/GU_RayIntersect.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <PRM/PRM_Include.h>
#include <UT/UT_StringStream.h>
#include <UT/UT_WorkBuffer.h>

#include "UT_RokokoSocket.h"


SIM_RokokoData::SIM_RokokoData(const SIM_DataFactory* factory) : BaseClass(factory)
{
    createClientSocket();
}

SIM_RokokoData::~SIM_RokokoData()
{
    destroyClientSocket();
}

void SIM_RokokoData::parseData(const UT_JSONValue* jsonValue)
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

void SIM_RokokoData::parsePropsOrTrackers(const UT_JSONValue* jsonValue)
{
    if (!jsonValue)
    {
        return;
    }

    static const UT_StringRef NAME_KEY("name");
    static const UT_StringRef POSITION_KEY("position");
    static const UT_StringRef ROTATION_KEY("rotation");

    propTrackers.clear();

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
                    strcpy(objInfo.name, nameValue->getS());
                }

                objInfo.position = parsePosition(objAsMap->get(POSITION_KEY));
                objInfo.rotation = parseRotation(objAsMap->get(ROTATION_KEY));

                propTrackers.append(objInfo);
            }
        }
    }
}


UT_Vector3 SIM_RokokoData::parsePosition(const UT_JSONValue* jsonValue)
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

UT_Quaternion SIM_RokokoData::parseRotation(const UT_JSONValue* jsonValue)
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

const SIM_DopDescription *SIM_RokokoData::getRokokoDataDopDescription()
{
    static PRM_Template theTemplates[] = {
        PRM_Template()
    };
    static SIM_DopDescription theDopDescription(true,
        "hdk_rokoko_data",
        "Rokoko Data",
        "Rokoko Data",
        classname(),
        theTemplates
    );

    return &theDopDescription;
}

void SIM_RokokoData::createClientSocket()
{
    myClientSocket = new UT_RokokoClientSocket("127.0.0.1", 11111);
}

bool SIM_RokokoData::openClientSocket()
{
    if (myClientSocket->isConnected())
    {
        return true;
    }

    return myClientSocket->connect() == UT_NetSocket::UT_CONNECT_SUCCESS;
}

void SIM_RokokoData::destroyClientSocket() const
{
    if (myClientSocket)
    {
        myClientSocket->shutdown(0);
        delete myClientSocket;

        myClientSocket = nullptr;
    }
}


bool SIM_RokokoData::readData()
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

const UT_Array<PropTrackerInfo>& SIM_RokokoData::getPropTrackers() const
{
    return propTrackers;
}
