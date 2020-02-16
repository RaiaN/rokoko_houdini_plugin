// Copyright Peter Leontev 2020

#ifndef _SIM_ROKOKO_DATA_H
#define _SIM_ROKOKO_DATA_H

#include <UT/UT_IStream.h>
#include <GA/GA_Types.h>
#include <GU/GU_DetailHandle.h>
#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_Geometry.h>
#include <SIM/SIM_Data.h>
#include <SIM/SIM_DataFactory.h>
#include <SIM/SIM_DataThreadedIO.h>
#include <SIM/SIM_DopDescription.h>
#include <UT/UT_JSONParser.h>
#include <UT/UT_JSONValue.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Array.h>

#include "PropTrackerInfo.h"


class UT_RokokoClientSocket;


class SIM_RokokoData : public SIM_Geometry
{
public:
    bool readData();

    const UT_Array<PropTrackerInfo>& getPropTrackers() const;

public:
    explicit SIM_RokokoData(const SIM_DataFactory* factory);
    virtual ~SIM_RokokoData();

protected:
    void parseData(const UT_JSONValue* jsonValue);
    void parsePropsOrTrackers(const UT_JSONValue* jsonValue);
    UT_Vector3 parsePosition(const UT_JSONValue* jsonValue);
    UT_Quaternion parseRotation(const UT_JSONValue* jsonValue);

private:
    static const SIM_DopDescription* getRokokoDataDopDescription();

    void createClientSocket();
    bool openClientSocket();
    void destroyClientSocket() const;
    
private:
    mutable UT_RokokoClientSocket* myClientSocket;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(
        SIM_RokokoData,               // Our Classname
        SIM_Geometry,                 // Base type
        "hdk_Rokoko_Data",            // DOP Data Type
        getRokokoDataDopDescription() // PRM list.
    );

private:
    UT_Array<PropTrackerInfo> propTrackers;

};

#endif