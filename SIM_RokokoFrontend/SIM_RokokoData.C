// Copyright Peter Leontev 2020

#include "SIM_RokokoData.h"

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


bool SIM_RokokoData::readSocketData(UT_JSONValue& value)
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
        if (!value.parseValue(*parser))
        {
            return false;
        }
    }

    return true;
}
