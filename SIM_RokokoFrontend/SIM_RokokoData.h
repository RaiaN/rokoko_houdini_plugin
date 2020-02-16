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


class UT_RokokoClientSocket;


class SIM_RokokoData : public SIM_Geometry
{
public:
    bool readSocketData(UT_JSONValue& value);

protected:
    explicit SIM_RokokoData(const SIM_DataFactory* factory);
    virtual ~SIM_RokokoData();

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
};

#endif